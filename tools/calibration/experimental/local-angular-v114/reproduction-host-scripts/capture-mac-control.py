from pathlib import Path
import json,os,subprocess,hashlib
r=Path.cwd();out=r/'outputs/air-torque-v114/mac-control';out.mkdir(parents=True,exist_ok=False);expected=[];name='game-air-pitch'
for x in map(json.loads,(r/'outputs/game-angular-v113/broad-final/candidate-run0.jsonl').open()):
 if x['event']=='calibration.sample' and x['detail']==name:expected.append(x['sample'])
assert len(expected)==120;probe=r/'work/frozen-v114-telemetry/rrr3d_physics_calibration';captures=[]
for tick in [0,3,4,5]:
 rows=[]
 for rep in [0,1]:
  tag=f'tick{tick}-rep{rep}';p=out/(tag+'-physical.jsonl');trace=out/(tag+'-private.jsonl');env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'work/frozen-v114-telemetry/manifest.json').read_text())['environment']);env.update(RRR3D_AIR_TORQUE_TRACE_FILE_V114=str(trace),RRR3D_AIR_TORQUE_TRACE_STEP_V114=str(tick));subprocess.run([str(probe),str(p),'list:'+name],env=env,capture_output=True,check=True,timeout=180)
  actual=[x['sample'] for x in map(json.loads,p.open()) if x['event']=='calibration.sample'];assert actual==expected;private=list(map(json.loads,trace.open()));assert len(private)==1 and private[0]['tick']==tick;rows.append(private);captures.append({'tick':tick,'repeat':rep,'all120_v113_states_unchanged':True})
 assert rows[0]==rows[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Read-only old Source air torque input/delta/velocity, complete V113 histories unchanged.', 'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'captures':captures,'goal_complete':False},indent=2)+'\n');print('All8 telemetry captures preserve120states')
