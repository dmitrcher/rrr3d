from pathlib import Path
import json,os,gzip,subprocess,struct,hashlib
root=Path.cwd();out=root/'outputs/wheel-normal-v101/mac-input';out.mkdir(exist_ok=False)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');base=json.loads((root/'work/frozen-v100/manifest.json').read_text())['environment'];names=['slope-longitudinal','slope-lateral','settle'];selections=[('slope-longitudinal',19),('slope-lateral',19),('settle',25)];bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
def physical(r):
 r=dict(r)
 for k in ['position','velocity','rotation','angular_momentum']:r[k]=list(map(bits,r[k]))
 fields={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'};r['wheels']=[{k:bits(v) if k in fields else v for k,v in w.items()} for w in r['wheels']];return r
expected={n:[] for n in names}
for r in map(json.loads,(root/'outputs/tire-angle-v100/broad/candidate-run0.jsonl').open()):
 if r.get('event')=='calibration.sample' and r['detail'] in expected:expected[r['detail']].append(physical(r['sample']))
captures=[]
for name,tick in selections:
 runs=[]
 for rep in range(2):
  tag=f'{name}-{tick}-{rep}';p=out/f'{tag}-physical.jsonl';trace=out/f'{tag}-normal.jsonl';env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env.update(RRR3D_WHEEL_NORMAL_TRACE_FILE_V101=str(trace),RRR3D_WHEEL_NORMAL_TRACE_STEP_V101=str(tick))
  with (out/f'{tag}.log').open('wb') as log:subprocess.run([str(probe),str(p),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=180)
  actual=[physical(r['sample']) for r in map(json.loads,p.open()) if r.get('event')=='calibration.sample'];assert actual==expected[name],('Trace changed V100 physics',tag);records=list(map(json.loads,trace.open()));assert records and all(x['physical_tick']==tick for x in records);runs.append(records)
  captures.append({'case':name,'tick':tick,'repeat':rep,'all_V100_states_exact':True,'states':len(actual),'normal_records':len(records),'physical_sha256':hashlib.sha256(p.read_bytes()).hexdigest()});print(tag,'V100 entire history exact',len(actual),'normal records',len(records),flush=True)
 assert runs[0]==runs[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Read-only actual Mac local/source triangle vertices and current Jolt normal; entire V100 histories unchanged','environment':base,'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'captures':captures,'repeats_exact':True,'goal_complete':False},indent=2)+'\n')
