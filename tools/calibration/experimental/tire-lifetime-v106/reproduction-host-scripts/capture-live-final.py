from pathlib import Path
import json,os,hashlib,subprocess,gzip,struct
r=Path.cwd();o=r/'outputs/tire-lifetime-v106/live-final';o.mkdir(exist_ok=False);m=json.loads((r/'work/frozen-v106/manifest.json').read_text());probe=r/'work/frozen-v106/rrr3d_physics_calibration';assert hashlib.sha256(probe.read_bytes()).hexdigest()==m['hashes']['rrr3d_physics_calibration'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(m['environment']);env.update(RRR3D_TIRE_OBJECT_TRACE_FILE_V106=str(o/'tire.jsonl'),RRR3D_WHEEL_PAIR_TRACE_FILE_V106=str(o/'pairs.jsonl'),RRR3D_WHEEL_CALLBACK_TRACE_FILE_V105=str(o/'callbacks.jsonl'));subprocess.run([str(probe),str(o/'physical.jsonl'),'bump-crossing'],env=env,capture_output=True,check=True,timeout=180)
resets=[]
for row in map(json.loads,(o/'pairs.jsonl').open()):
 for wheel,cleared in enumerate(row['resets']):
  if cleared:resets.append([row['sdk_counter_before_step'],wheel])
expected=json.loads((r/'outputs/tire-lifetime-v106/native-pair-reset-proof.json').read_text())['proof'][0]['actual_all_objects_clear_call_206f41'];assert resets==expected
orders={i:r['tire_orders'] for i,r in enumerate(map(json.loads,(o/'tire.jsonl').open()))};assert orders[20]==[1,2,3,4] and orders[57]==[5,6,3,4] and orders[74]==[5,6,7,8]
(o/'manifest.json').write_text(json.dumps({'scope':'Actual final frozenV106 independent runtime geometry and tire object lifetimes; no supplied original trajectory. Source static manager resets match actual Native events, creation ages match measured tire traversal.','probe_sha256':m['hashes']['rrr3d_physics_calibration'],'environment':m['environment'],'actual_runtime_manager_reset_events':resets,'tire_orders_at_first_contact_front_recreation_rear_recreation':{str(i):orders[i] for i in [20,57,74]},'goal_complete':False},indent=2)+'\n');print('Final runtime events equal actual Native, persistent/recreated tire ages correct.')
