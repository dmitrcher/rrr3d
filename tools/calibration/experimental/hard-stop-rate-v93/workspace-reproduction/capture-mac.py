from pathlib import Path
import gzip, hashlib, json, os, struct, subprocess

root=Path.cwd()
out=root/'outputs/contact-solver-v93/mac-kernel'
out.mkdir(parents=True,exist_ok=False)
frozen=root/'work/frozen-v92'
manifest=json.loads((frozen/'manifest.json').read_text())
probe=frozen/'rrr3d_physics_calibration'
assert hashlib.sha256(probe.read_bytes()).hexdigest()==manifest['hashes'][probe.name]
name='mobility/marauder/wheel1/wheel-drive'
def load(path):
    opener=gzip.open if str(path).endswith('.gz') else open
    with opener(path,'rt') as stream:
        return [r['sample'] for r in map(json.loads,stream) if r.get('event')=='calibration.sample' and r['detail']==name]
def bits(value):
    return struct.unpack('<I',struct.pack('<f',value))[0]
def physical(row):
    row=dict(row)
    for key in ['position','velocity','rotation','angular_momentum']:
        row[key]=list(map(bits,row[key]))
    floats={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'}
    row['wheels']=[{k:bits(v) if k in floats else v for k,v in wheel.items()} for wheel in row['wheels']]
    return row
golden=load(root/'outputs/calibration-v14-source-windows.jsonl')
saved=load(root/'outputs/wheel-motor-v92/broad/candidate-run0.jsonl')
assert len(golden)==len(saved)==600
entries=[]
for tick in [21,22]:
    repeats=[]
    for rep in range(2):
        tag=f'marauder-wheel1-drive-{tick}-{rep}'
        env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))}
        env.update(manifest['environment'])
        for fileflag,stepflag,kind in [
            ('RRR3D_NATIVE_TIRE_V55_TRACE','RRR3D_NATIVE_TIRE_TRACE_STEP_V86','tire'),
            ('RRR3D_WHEEL_SOLVER_TRACE_FILE_V62','RRR3D_WHEEL_SOLVER_TRACE_STEP_V62','solver'),
            ('RRR3D_WHEEL_PREP_TRACE_FILE_V67','RRR3D_WHEEL_PREP_TRACE_STEP_V67','prep')]:
            env[fileflag]=str(out/f'{tag}-{kind}.jsonl')
            env[stepflag]=str(tick+1)
        path=out/f'{tag}-physical.jsonl'
        with (out/f'{tag}.log').open('wb') as log:
            subprocess.run([str(probe),str(path),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=180)
        actual=load(path)
        assert list(map(physical,actual))==list(map(physical,saved)), 'Read-only trace changed accepted V92 trajectory'
        repeats.append(actual)
        differences=[a['tick'] for a,b in zip(actual,golden) if physical(a)!=physical(b)]
        assert differences[0]==22
        entries.append({'physical_tick':tick,'sdk_counter':tick+1,'repeat':rep,'states':len(actual),'preserves_v92_float32':True,'first_difference_windows':22,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
        print(tag,'600 states preserve V92; first Windows difference22',flush=True)
    assert repeats[0]==repeats[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Read-only V92 Marauder early contact solver inspection. SDK prep/solve counter=physical tick+1. No V93 source edits.','probe_sha256':manifest['hashes'][probe.name],'environment':manifest['environment'],'captures':entries,'repeats_exact':True,'goal_complete':False},indent=2)+'\n')
