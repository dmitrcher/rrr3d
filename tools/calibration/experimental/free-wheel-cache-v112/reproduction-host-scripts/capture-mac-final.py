from pathlib import Path
import json, os, subprocess, hashlib, importlib.util
spec=importlib.util.spec_from_file_location("words",Path.cwd()/"work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py");words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
r=Path.cwd();out=r/'outputs/free-wheel-v112/mac-final';out.mkdir(parents=True,exist_ok=False)
probe=r/'work/frozen-v112-final/rrr3d_physics_calibration'
name='mobility/podushka/engine1/wheel-drive'
expected=[]
with (r/'outputs/calibration-v14-source-windows.jsonl').open() as f:
 for x in map(json.loads,f):
  if x['event']=='calibration.sample' and x['detail']==name:expected.append(x['sample'])
assert len(expected)==600
captures=[]
for tick in [507,508,509]:
 traces=[]
 for rep in range(2):
  tag=f'tick{tick}-rep{rep}';p=out/(tag+'-physical.jsonl');trace=out/(tag+'-motor.jsonl')
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'work/frozen-v112-final/manifest.json').read_text())['environment'])
  env['RRR3D_FREE_WHEEL_TRACE_FILE_V112']=str(trace);env['RRR3D_FREE_WHEEL_TRACE_STEP_V112']=str(tick)
  with (out/(tag+'.log')).open('wb') as log:subprocess.run([str(probe),str(p),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=180)
  actual=[x['sample'] for x in map(json.loads,p.open()) if x['event']=='calibration.sample'];assert list(map(words.physical,actual))==list(map(words.physical,expected))
  rows=list(map(json.loads,trace.open()));assert len(rows)==4 and all(x['tick']==tick for x in rows);traces.append(rows)
  captures.append({'tick':tick,'repeat':rep,'original600_Native_equivalent_states':True});print(tag,rows,flush=True)
 assert traces[0]==traces[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Actual final V112 live cached input and motor result; original600-step Native observables match for each trace.', 'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'captures':captures,'goal_complete':False},indent=2)+'\n')
