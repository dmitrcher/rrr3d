from pathlib import Path
import json,os,subprocess,gzip,hashlib
root=Path.cwd();out=root/'outputs/wheel-motor-v92/mac-control';out.mkdir(parents=True,exist_ok=False);probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');name='mobility/dirtdevil/wheel1/wheel-drive';expected=[]
with (root/'outputs/tire-anchor-v91/broad/candidate-run0.jsonl').open() as f:
 for r in map(json.loads,f):
  if r['event']=='calibration.sample' and r['detail']==name:expected.append(r['sample'])
assert len(expected)==600;captures=[]
for tick in [119,120,121]:
 traces=[]
 for rep in range(2):
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'work/frozen-v91/manifest.json').read_text())['environment']);tag=f'tick{tick}-rep{rep}';p=out/(tag+'-physical.jsonl');trace=out/(tag+'-motor.jsonl');env['RRR3D_WHEEL_MOTOR_TRACE_FILE_V92']=str(trace);env['RRR3D_WHEEL_MOTOR_TRACE_STEP_V92']=str(tick)
  with (out/(tag+'.log')).open('wb') as log:subprocess.run([str(probe),str(p),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=180)
  actual=[r['sample'] for r in map(json.loads,p.open()) if r['event']=='calibration.sample'];assert actual==expected,'Read-only trace changed V91 history';rows=[json.loads(s) for s in trace.open()];assert len(rows)==4 and all(r['physical_tick']==tick for r in rows);traces.append(rows);captures.append({'tick':tick,'repeat':rep,'physical600_states_exact_v91':True});print(tag,'history unchanged',flush=True)
 assert traces[0]==traces[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Read-only actual Mac motor inputs/output. Physical history exact V91.','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'captures':captures,'goal_complete':False},indent=2)+'\n')
