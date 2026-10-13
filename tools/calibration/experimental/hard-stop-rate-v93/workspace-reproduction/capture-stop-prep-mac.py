from pathlib import Path
import json, os, subprocess, hashlib, gzip, struct, sys
root=Path.cwd()
mode=sys.argv[1]
assert mode in ('readonly','candidate')
out=root/f'outputs/contact-solver-v93/mac-{mode}-stop'
out.mkdir(parents=True,exist_ok=True)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration')
selections=[('mobility/marauder/wheel1/wheel-drive',22),('mobility/marauder/wheel1/brake-moving',22),('mobility/manticora/wheel3/lateral',23),('wheel-drive',24)]
names=[n for n,t in selections]
def load(path):
    result={n:[] for n in names}
    opener=gzip.open if str(path).endswith('.gz') else open
    with opener(path,'rt') as stream:
        for row in map(json.loads,stream):
            if row.get('event')=='calibration.sample' and row['detail'] in result:
                result[row['detail']].append(row['sample'])
    return result
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def physical(row):
    result=dict(row)
    for k in ['position','velocity','rotation','angular_momentum']:result[k]=list(map(bits,result[k]))
    fields={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'}
    result['wheels']=[{k:bits(v) if k in fields else v for k,v in w.items()} for w in result['wheels']]
    return result
expected=load(root/'outputs/wheel-motor-v92/broad/candidate-run0.jsonl')
golden=load(root/'outputs/calibration-v14-source-windows.jsonl')
entries=[]
for index,(name,tick) in enumerate(selections):
    repeats=[]; private=[]
    for rep in range(2):
        env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))}
        env.update(json.loads((root/'work/frozen-v92/manifest.json').read_text())['environment'])
        if mode=='candidate':env['RRR3D_EXPERIMENT_NATIVE_HARD_STOP_RATE_V93']='1'
        tag=f'{index}-{tick}-{rep}'
        trace=out/f'{tag}-stop.jsonl'
        env['RRR3D_HARD_STOP_PREP_TRACE_FILE_V93']=str(trace)
        env['RRR3D_HARD_STOP_PREP_TRACE_STEP_V93']=str(tick+1)
        for fileflag,stepflag,kind in [
            ('RRR3D_NATIVE_TIRE_V55_TRACE','RRR3D_NATIVE_TIRE_TRACE_STEP_V86','tire'),
            ('RRR3D_WHEEL_SOLVER_TRACE_FILE_V62','RRR3D_WHEEL_SOLVER_TRACE_STEP_V62','solver'),
            ('RRR3D_WHEEL_PREP_TRACE_FILE_V67','RRR3D_WHEEL_PREP_TRACE_STEP_V67','prep')]:
            env[fileflag]=str(out/f'{tag}-{kind}.jsonl');env[stepflag]=str(tick+1)
        p=out/f'{tag}-physical.jsonl'
        if not p.exists():
            with (out/f'{tag}.log').open('wb') as log:
                subprocess.run([str(probe),str(p),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=180)
        actual=load(p)[name];assert len(actual)==600
        if mode=='readonly':assert list(map(physical,actual))==list(map(physical,expected[name])), 'Telemetry changed V92'
        rows=list(map(json.loads,trace.open())) if trace.exists() else []
        assert len(rows) in (0,4) and all(r['sdk_counter']==tick+1 for r in rows)
        repeats.append(actual);private.append(rows)
        differences=[a['tick'] for a,b in zip(actual,golden[name]) if physical(a)!=physical(b)]
        entries.append({'name':name,'tick':tick,'sdk_counter':tick+1,'repeat':rep,'states':600,'preserves_v92_float32':mode=='readonly','first_difference_windows':differences[0] if differences else None,'physical_sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
        print(name,tick,rep,'first Windows difference',differences[0] if differences else None,flush=True)
    assert repeats[0]==repeats[1] and private[0]==private[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Actual live stop preparation with identical scenario inputs. No injected row/body state.','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'mode':mode,'captures':entries,'repeats_exact':True,'goal_complete':False},indent=2)+'\n')
