from pathlib import Path
import subprocess,json,hashlib,gzip,os
root=Path.cwd();out=root/'outputs/spring-force-v115/mac';out.mkdir(parents=True,exist_ok=False)
frozen=root/'work/frozen-v114-final';manifest=json.loads((frozen/'manifest.json').read_text());probe=frozen/'rrr3d_physics_calibration'
assert hashlib.sha256(probe.read_bytes()).hexdigest()==manifest['hashes'][probe.name]
expected=[]
with open(root/'outputs/air-torque-v114/broad-final/candidate-run0.jsonl') as f:
 for r in map(json.loads,f):
  if r.get('event')=='calibration.sample' and r['detail']=='spring-soft':expected.append(r['sample'])
assert len(expected)==600
captures=[]
for tick in [45,46]:
 for rep in range(2):
  tag=f'spring-soft-{tick}-{rep}';env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(manifest['environment'])
  for pathvar,stepvar,kind in [('RRR3D_WHEEL_SOLVER_TRACE_FILE_V62','RRR3D_WHEEL_SOLVER_TRACE_STEP_V62','solver'),('RRR3D_WHEEL_PREP_TRACE_FILE_V67','RRR3D_WHEEL_PREP_TRACE_STEP_V67','prep'),('RRR3D_COMBINED_TIRE_TRACE_FILE_V95','RRR3D_COMBINED_TIRE_TRACE_STEP_V95','combined')]:
   env[pathvar]=str(out/(tag+'-'+kind+'.jsonl'));env[stepvar]=str(tick if kind=='combined' else tick+1)
  path=out/(tag+'-physical.jsonl')
  with (out/(tag+'.log')).open('wb') as f:subprocess.run([str(probe),str(path),'list:spring-soft'],env=env,check=True,stdout=f,stderr=f,timeout=180)
  actual=[r['sample'] for r in map(json.loads,path.open()) if r.get('event')=='calibration.sample'];assert actual==expected,'V114 public history changed'
  captures.append({'tick':tick,'repeat':rep,'v114_history_exact':True,'states':len(actual)})
  for p in out.glob(tag+'*.jsonl'):
   with gzip.open(str(p)+'.gz','wb') as f:f.write(p.read_bytes())
   p.unlink()
  (out/'manifest.json').write_text(json.dumps({'probe_sha256':manifest['hashes'][probe.name],'captures':captures,'goal_complete':False},indent=2)+'\n');print(tag,'600 V114 states unchanged',flush=True)
