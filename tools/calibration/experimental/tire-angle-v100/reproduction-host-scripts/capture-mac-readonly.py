from pathlib import Path
import gzip,json,os,subprocess,struct,hashlib
root=Path.cwd();out=root/'outputs/tire-angle-v100/mac-readonly';out.mkdir(exist_ok=False)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');envbase=json.loads((root/'work/frozen-v99/manifest.json').read_text())['environment']
selections=[('border-front-grounded',149),('border-front-grounded',148),('border-front-grounded',150),('slope-longitudinal',19),('slope-lateral',19),('border-grounded',61),('curb-side',13),('lateral',253)]
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def physical(r):
 r=dict(r)
 for k in ['position','velocity','rotation','angular_momentum']:r[k]=list(map(bits,r[k]))
 fields={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'};r['wheels']=[{k:bits(v) if k in fields else v for k,v in w.items()} for w in r['wheels']];return r
names={n for n,t in selections};expected={n:[] for n in names}
with (root/'outputs/creation-order-v99/broad/candidate-run0.jsonl').open() as f:
 for x in map(json.loads,f):
  if x.get('event')=='calibration.sample' and x['detail'] in expected:expected[x['detail']].append(physical(x['sample']))
captures=[]
for name,tick in selections:
 runs=[]
 for rep in range(2):
  tag=f'{name}-{tick}-{rep}';p=out/f'{tag}-physical.jsonl';angle=out/f'{tag}-angle.jsonl';env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(envbase);env.update(RRR3D_TIRE_ANGLE_TRACE_FILE_V100=str(angle),RRR3D_TIRE_ANGLE_TRACE_STEP_V100=str(tick))
  with (out/f'{tag}.log').open('wb') as f:subprocess.run([str(probe),str(p),'list:'+name],env=env,stdout=f,stderr=f,check=True,timeout=180)
  rows=[physical(x['sample']) for x in map(json.loads,p.open()) if x.get('event')=='calibration.sample'];assert rows==expected[name],('Read-only trace changed physics',name,tick,rep)
  inputs=list(map(json.loads,angle.open()));assert inputs and all(x['physical_tick']==tick for x in inputs);runs.append(inputs)
  captures.append({'case':name,'tick':tick,'repeat':rep,'all_accepted_V99_states_exact':True,'states':len(rows),'angle_calls':len(inputs),'physical_sha256':hashlib.sha256(p.read_bytes()).hexdigest()});print(tag,'V99 states exact',len(rows),'inputs',len(inputs),flush=True)
 assert runs[0]==runs[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Read-only Mac relative/lateral/cosine/angle raw-word capture; all uninterrupted trajectories exactly accepted V99','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'environment':envbase,'captures':captures,'angle_repeats_exact':True,'goal_complete':False},indent=2)+'\n')
