from pathlib import Path
import gzip, hashlib, json, os, struct, subprocess

root=Path.cwd()
out=root/'outputs/contact-bias-v109/mac-final79'
if out.exists():assert not any(out.iterdir()), 'Existing trace cannot be overwritten'
else:out.mkdir(parents=True)
frozen=root/'work/frozen-v109'
manifest=json.loads((frozen/'manifest.json').read_text())
probe=frozen/'rrr3d_physics_calibration'
assert b'RRR3D_COMBINED_TIRE_TRACE_FILE_V95' in probe.read_bytes()
name='border-grounded'
def load(path):
    if not path.exists():path=Path(str(path)+'.gz')
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
saved=load(root/'outputs/static-box-query-v107/broad/candidate-run0.jsonl')
assert len(golden)==len(saved)>10
entries=[]
for tick in [79]:
    repeats=[]
    for rep in range(2):
        tag=f'border-grounded-{tick}-{rep}'
        env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))}
        env.update(manifest['environment'])
        env['RRR3D_EXPERIMENT_NATIVE_COMBINED_TIRE_V95']='1'
        env.update(RRR3D_CONTACT_ROWS='1', RRR3D_CONTACT_POINTS='1', RRR3D_CONTACT_NATIVE_TRACE_STEP=str(tick), RRR3D_CONTACT_ITERATION_ROWS='1', RRR3D_CONTACT_MATERIAL_STAGE_TRACE='1', RRR3D_CONTACT_CACHE_INPUT_TRACE='1', RRR3D_CONTACT_LOCAL_INERTIA_TRACE='1')
        for fileflag,stepflag,kind in [
            ('RRR3D_NATIVE_TIRE_V55_TRACE','RRR3D_NATIVE_TIRE_TRACE_STEP_V86','tire'),
            ('RRR3D_WHEEL_SOLVER_TRACE_FILE_V62','RRR3D_WHEEL_SOLVER_TRACE_STEP_V62','solver'),
            ('RRR3D_WHEEL_PREP_TRACE_FILE_V67','RRR3D_WHEEL_PREP_TRACE_STEP_V67','prep')]:
            env[fileflag]=str(out/f'{tag}-{kind}.jsonl')
            env[stepflag]=str(tick+1)
        env['RRR3D_COMBINED_TIRE_TRACE_FILE_V95']=str(out/f'{tag}-combined.jsonl')
        env['RRR3D_COMBINED_TIRE_TRACE_STEP_V95']=str(tick)
        path=out/f'{tag}-physical.jsonl'
        with (out/f'{tag}.log').open('wb') as log:
            subprocess.run([str(probe),str(path),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=180)
        actual=load(path)
        assert len(actual)==len(saved), 'State count changed'
        repeats.append(actual)
        differences=[a['tick'] for a,b in zip(actual,golden) if physical(a)!=physical(b)]
        print('first difference',differences[0] if differences else None,flush=True)
        entries.append({'physical_tick':tick,'sdk_counter':tick+1,'repeat':rep,'states':len(actual),'first_difference_windows':differences[0] if differences else None,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
        print(tag,'candidate captured',len(actual),'states',flush=True)
    assert repeats[0]==repeats[1]
(out/'manifest.json').write_text(json.dumps({'scope':'V109 candidate border-grounded body contact inspection at physical78/79; uninterrupted600-step trajectory.','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'environment':dict(manifest['environment'], RRR3D_EXPERIMENT_NATIVE_COMBINED_TIRE_V95='1'),'captures':entries,'repeats_exact':True,'goal_complete':False},indent=2)+'\n')
