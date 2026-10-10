#!/usr/bin/env python3
"""Compare completed, identically commanded calibration runs against Windows."""
import argparse
import csv
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


def load(path, expected):
    cases = {}
    for number, line in enumerate(Path(path).read_text(encoding='utf-8-sig').splitlines(), 1):
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
        except (KeyError, TypeError, ValueError) as e:
            raise ValueError(f'{path}:{number}: {e}') from e
    if not cases: raise ValueError('No calibration cases')
    for name, case in cases.items():
        if not case['end'] or len(case['rows']) != case['meta']['steps']:
            raise ValueError(f'{name}: incomplete capture')
    return cases


def norm(a, b): return math.sqrt(sum((x-y)**2 for x,y in zip(a,b)))
def rms(values): return math.sqrt(sum(v*v for v in values)/len(values))

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
            dot = abs(sum(u*v for u,v in zip(x['rotation'], y['rotation'])))
            r['rotation_error_radians'] = 2*math.acos(min(1.0, dot / math.sqrt(sum(u*u for u in x['rotation'])*sum(v*v for v in y['rotation']))))
            r['wheel_speed_error'] = rms([u['angular_speed']-v['angular_speed'] for u,v in zip(x['wheels'],y['wheels'])])
            r['wheel_contact_mismatch'] = sum(u['contact'] != v['contact'] for u,v in zip(x['wheels'],y['wheels']))
            r['wheel_height_error'] = rms([u['height']-v['height'] for u,v in zip(x['wheels'],y['wheels']) if u['contact'] and v['contact']]) if any(u['contact'] and v['contact'] for u,v in zip(x['wheels'],y['wheels'])) else 0
            for platform,s in [('windows',x),('mac',y)]:
                for field in ('position','velocity'):
                    for axis,value in zip('xyz',s[field]): r[f'{platform}_{field}_{axis}'] = value
            errors.append(r)
        fields = ('position_error','velocity_error','rotation_error_radians','wheel_speed_error','angular_momentum_error')
        result[name] = {'samples':len(errors),'rms':{f:rms([e[f] for e in errors]) for f in fields},'maximum':{f:max(e[f] for e in errors) for f in fields},'wheel_contact_mismatch_fraction':sum(e['wheel_contact_mismatch'] for e in errors)/(len(errors)*4),'windows_final':left['rows'][-1],'mac_final':right['rows'][-1]}
        details.extend(errors)
    report = {'schema':1, 'reference':'Windows PhysX 2.8.4', 'candidate':'Mac production Jolt vehicle adapter', 'reference_sha256':hashlib.sha256(Path(reference).read_bytes()).hexdigest(),'candidate_sha256':hashlib.sha256(Path(candidate).read_bytes()).hexdigest(),'candidate_profiles': sorted({c['meta']['profile'] for c in b.values()}),'comparable_scenario':True,'cases':result,'limitations':['Controlled box/four-wheel fixture tests solver and vehicle adapter; these do not certify whole-game equivalence, map collisions, AI, damage, weapons or rendering.','Fixed solver steps remove frame-rate dependence; repeat captures are still required to measure solver variance.','No automatic global game parameter changes; candidate profile must be validated on held-out cases and real races.']}
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
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('windows');p.add_argument('mac');p.add_argument('--out',required=True);args=p.parse_args()
    try:
        r=compare(args.windows,args.mac,args.out)
        print(json.dumps({name:c['rms'] for name,c in r['cases'].items()},indent=2))
    except (OSError,ValueError) as e:p.exit(2,str(e)+'\n')
if __name__=='__main__':main()
