#!/usr/bin/env python3
"""Compare completed, identically commanded calibration runs against Windows."""
import argparse
import csv
import gzip
import hashlib
import json
import math
from pathlib import Path


def finite(value):
    if isinstance(value, bool) or value is None or isinstance(value, str):
        return
    if isinstance(value, (int, float)):
        if not math.isfinite(value):
            raise ValueError('Non-finite telemetry')
    elif isinstance(value, dict):
        for v in value.values(): finite(v)
    elif isinstance(value, list):
        for v in value: finite(v)
    else:
        raise ValueError('Unsupported telemetry value')


def capture_path(path):
    path = Path(path)
    compressed = path.with_name(path.name + '.gz')
    return compressed if not path.exists() and compressed.exists() else path


def open_capture(path, mode='rt'):
    path = capture_path(path)
    options = {'encoding': 'utf-8-sig'} if 't' in mode else {}
    return gzip.open(path, mode, **options) if path.suffix == '.gz' else path.open(mode, **options)


def capture_sha256(path):
    """Hash logical capture bytes, preserving the original hash after compression."""
    digest = hashlib.sha256()
    with open_capture(path, 'rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def load(path, expected):
    cases = {}
    with open_capture(path) as stream:
        for number, line in enumerate(stream, 1):
            if not line.strip(): continue
            try:
                row = json.loads(line)
                finite(row)
                if row['platform'] != expected:
                    raise ValueError('Wrong reference/candidate platform')
                name, event = row['detail'], row['event']
                if event == 'calibration.begin':
                    if name in cases: raise ValueError('Duplicate case')
                    meta = row['sample']
                    if meta['schema'] != 1 or meta['sample_phase'] != 'after_solve':
                        raise ValueError('Unsupported schema or sample phase')
                    if meta['dt'] <= 0 or not isinstance(meta['steps'], int) or meta['steps'] <= 0:
                        raise ValueError('Invalid step contract')
                    cases[name] = {'meta': meta, 'rows': [], 'end': False}
                elif event == 'calibration.sample':
                    case = cases[name]
                    s = row['sample']
                    if type(s['tick']) is not int or case['end'] or s['tick'] != len(case['rows']):
                        raise ValueError('Missing/duplicate/out-of-order ticks')
                    if abs(s['time'] - (s['tick']+1)*case['meta']['dt']) > 1e-6:
                        raise ValueError('Invalid sample time')
                    for field, size in [('position', 3), ('velocity', 3), ('angular_momentum', 3), ('rotation', 4)]:
                        if len(s[field]) != size or any(type(v) not in (int,float) for v in s[field]): raise ValueError('Wrong vector size/type')
                    if len(s['wheels']) != 4: raise ValueError('Wrong wheel count')
                    if abs(sum(v*v for v in s['rotation']) - 1) > .01:
                        raise ValueError('Invalid rotation quaternion')
                    for w in s['wheels']:
                        if not isinstance(w['contact'], bool): raise ValueError('Invalid contact flag')
                        float(w['height']); float(w['angular_speed'])
                    case['rows'].append(s)
                elif event == 'calibration.end':
                    case = cases[name]
                    if case['end']: raise ValueError('Duplicate end')
                    case['end'] = True
                elif event in ('calibration.engine-description', 'calibration.peer-description'):
                    case = cases[name]
                    if event in case: raise ValueError('Duplicate engine description')
                    description = row['sample']
                    if not isinstance(description, dict) or type(description.get('mass')) not in (int,float) or description['mass'] <= 0:
                        raise ValueError('Invalid engine mass description')
                    for field in ('inertia','com'):
                        values = description.get(field)
                        if not isinstance(values,list) or len(values) != 3 or any(type(v) not in (int,float) for v in values):
                            raise ValueError('Invalid engine vector description')
                    case[event] = description
            except (KeyError, TypeError, ValueError) as e:
                raise ValueError(f'{path}:{number}: {e}') from e
    if not cases: raise ValueError('No calibration cases')
    for name, case in cases.items():
        if not case['end'] or len(case['rows']) != case['meta']['steps']:
            raise ValueError(f'{name}: incomplete capture')
        meta = case['meta']
        if expected == 'windows-physx-d3d9' and meta['fixture'] == 'source-vehicle-matrix-v7-sdk' and 'pair' in meta:
            # NxShapeDesc::operator= did not copy the peer's base wheel fields
            # in pre-v14 captures. Intended metadata alone cannot certify them.
            primary = case.get('calibration.engine-description')
            peer = case.get('calibration.peer-description')
            if primary is None or peer is None:
                raise ValueError(f'{name}: legacy Windows pair reference lacks actual peer mass properties; recapture with the corrected v14 stand')
            for description, mass in [(primary, meta['mass']), (peer, meta['pair']['peer_mass'])]:
                if not math.isclose(description['mass'], mass, rel_tol=1e-6, abs_tol=1e-5):
                    raise ValueError(f'{name}: actual actor mass differs from fixture')
                if len(description['inertia']) != 3 or any(v <= 0 for v in description['inertia']):
                    raise ValueError(f'{name}: invalid actual actor inertia')
                if len(description['com']) != 3 or norm(description['com'], meta['fixture_parameters']['center_of_mass']) > 1e-5:
                    raise ValueError(f'{name}: actual actor COM differs from fixture')
            ratio = peer['mass'] / primary['mass']
            if any(not math.isclose(y, x*ratio, rel_tol=2e-6, abs_tol=1e-5) for x,y in zip(primary['inertia'],peer['inertia'])):
                raise ValueError(f'{name}: peer inertia differs from the identical chassis scaled by mass; check wheel-description copy construction')
    return cases


def norm(a, b): return math.sqrt(sum((x-y)**2 for x,y in zip(a,b)))
def rms(values): return math.sqrt(sum(v*v for v in values)/len(values))

def rotation_error(a, b):
    # The vector part of conj(a)*b retains tiny rotations that acos(dot)
    # rounds to zero. atan2 also handles q/-q and non-unit stored quaternions.
    ax, ay, az, aw = a
    bx, by, bz, bw = b
    vector = (aw*bx-bw*ax-ay*bz+az*by,
              aw*by-bw*ay-az*bx+ax*bz,
              aw*bz-bw*az-ax*by+ay*bx)
    dot = sum(u*v for u, v in zip(a, b))
    return 2*math.atan2(math.hypot(*vector), abs(dot))

def compare(reference, candidate, out=None):
    a = load(reference, 'windows-physx-d3d9')
    b = load(candidate, 'mac-jolt-metal')
    if a.keys() != b.keys(): raise ValueError('Case sets differ')
    result, details = {}, []
    for name, left in a.items():
        right = b[name]
        ma = {k:v for k,v in left['meta'].items() if k != 'profile'}
        mb = {k:v for k,v in right['meta'].items() if k != 'profile'}
        if ma != mb: raise ValueError(f'{name}: fixture/units/timestep mismatch')
        errors = []
        for x,y in zip(left['rows'], right['rows']):
            if x['command'] != y['command']: raise ValueError(f'{name}:{x["tick"]}: different commands')
            r = {'case': name, 'tick': x['tick'], 'time': x['time']}
            for field in ('position', 'velocity', 'angular_momentum'):
                r[field+'_error'] = norm(x[field], y[field])
                for axis,u,v in zip('xyz',x[field],y[field]):
                    r[field+'_'+axis+'_error'] = abs(u-v)
            r['rotation_error_radians'] = rotation_error(x['rotation'], y['rotation'])
            r['wheel_speed_error'] = rms([u['angular_speed']-v['angular_speed'] for u,v in zip(x['wheels'],y['wheels'])])
            r['wheel_contact_mismatch'] = sum(u['contact'] != v['contact'] for u,v in zip(x['wheels'],y['wheels']))
            r['wheel_height_error'] = rms([u['height']-v['height'] for u,v in zip(x['wheels'],y['wheels']) if u['contact'] and v['contact']]) if any(u['contact'] and v['contact'] for u,v in zip(x['wheels'],y['wheels'])) else 0
            r['suspension_length_error'] = rms([u.get('suspension_length',0)-v.get('suspension_length',0) for u,v in zip(x['wheels'],y['wheels']) if u['contact'] and v['contact']]) if any(u['contact'] and v['contact'] for u,v in zip(x['wheels'],y['wheels'])) else 0
            for platform,s in [('windows',x),('mac',y)]:
                for field in ('position','velocity'):
                    for axis,value in zip('xyz',s[field]): r[f'{platform}_{field}_{axis}'] = value
            errors.append(r)
        fields = ('position_error','velocity_error','rotation_error_radians','wheel_speed_error','angular_momentum_error','suspension_length_error','wheel_height_error') + tuple(field+'_'+axis+'_error' for field in ('position','velocity','angular_momentum') for axis in 'xyz')
        result[name] = {'samples':len(errors),'rms':{f:rms([e[f] for e in errors]) for f in fields},'maximum':{f:max(e[f] for e in errors) for f in fields},'wheel_contact_mismatch_fraction':sum(e['wheel_contact_mismatch'] for e in errors)/(len(errors)*4),'windows_final':left['rows'][-1],'mac_final':right['rows'][-1]}
        details.extend(errors)
    report = {'schema':1, 'reference':'Windows PhysX 2.8.4', 'candidate':'Mac production Jolt vehicle adapter', 'reference_sha256':capture_sha256(reference),'candidate_sha256':capture_sha256(candidate),'candidate_profiles': sorted({c['meta']['profile'] for c in b.values()}),'comparable_scenario':True,'cases':result,'limitations':['Controlled tests include all 17 original chassis and each positive workshop tire function. Equal Nm/wheel bypasses gearing/AI; individual presets do not certify every combined loadout or complete map.','Fixed solver steps remove frame-rate dependence; repeat captures are still required to measure solver variance.','Raw engine slip/normal-force fields use backend conventions and must not be compared as identical observables. Remaining contact/transient differences are recorded, not hidden by the aggregate score.']}
    report['rotation_metric'] = '2*atan2(norm(vector(conj(q_windows)*q_mac)), abs(dot(q_windows,q_mac)))'
    remaining = [name for name,c in result.items()
                 if any(value != 0 for value in c['maximum'].values())
                 or c['wheel_contact_mismatch_fraction'] != 0]
    report['exact_parity'] = {'complete': not remaining,
                              'cases_remaining': remaining,
                              'cases_checked': len(result)}
    if out:
        out = Path(out); out.mkdir(parents=True,exist_ok=True)
        (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
        with (out/'samples.csv').open('w',newline='') as f:
            writer=csv.DictWriter(f,fieldnames=list(details[0]));writer.writeheader();writer.writerows(details)
        lines=['# Физическая калибровка: Windows → Mac','', '| Сценарий | RMS координат | RMS скорости | RMS вращения (рад) | Несовпадение контактов |','|---|---:|---:|---:|---:|']
        for name,c in result.items(): lines.append(f'| {name} | {c["rms"]["position_error"]:.6g} | {c["rms"]["velocity_error"]:.6g} | {c["rms"]["rotation_error_radians"]:.6g} | {c["wheel_contact_mismatch_fraction"]:.1%} |')
        lines += ['', 'Единицы координат/скоростей — исходные единицы мира и секунды. Полный формат входов и ограничения — в `report.json`.','']
        (out/'report.md').write_text('\n'.join(lines))
    return report


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('windows');p.add_argument('mac');p.add_argument('--out',required=True);p.add_argument('--require-exact',action='store_true',help='Exit 1 if any comparable physical observable still differs');args=p.parse_args()
    try:
        r=compare(args.windows,args.mac,args.out)
        print(json.dumps({name:c['rms'] for name,c in r['cases'].items()},indent=2))
        if args.require_exact and not r['exact_parity']['complete']:
            p.exit(1, 'Physical differences remain in '+str(len(r['exact_parity']['cases_remaining']))+' cases\n')
    except (OSError,ValueError) as e:p.exit(2,str(e)+'\n')
if __name__=='__main__':main()
