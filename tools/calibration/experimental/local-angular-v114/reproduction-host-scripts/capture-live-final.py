from pathlib import Path
import json,os,subprocess,hashlib,gzip,struct,importlib.util
r=Path.cwd();out=r/'outputs/air-torque-v114/live-final';out.mkdir(parents=True,exist_ok=False);name='game-air-pitch';reference=[]
spec=importlib.util.spec_from_file_location('words',r/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
for x in map(json.loads,(r/'outputs/calibration-v14-source-windows.jsonl').open()):
 if x['event']=='calibration.sample' and x['detail']==name:reference.append(words.physical(x['sample']))
assert len(reference)==120;probe=r/'work/frozen-v114-final/rrr3d_physics_calibration';captures=[]
f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0];bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];sf=lambda v:f(bits(v))
for index,tick in enumerate([0,3,4,5]):
 for rep in [0,1]:
  tag=f'tick{tick}-rep{rep}';p=out/(tag+'-physical.jsonl');trace=out/(tag+'-private.jsonl');env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'work/frozen-v114-final/manifest.json').read_text())['environment']);env.update(RRR3D_AIR_TORQUE_TRACE_FILE_V114=str(trace),RRR3D_AIR_TORQUE_TRACE_STEP_V114=str(tick));subprocess.run([str(probe),str(p),'list:'+name],env=env,capture_output=True,check=True,timeout=180)
  actual=[words.physical(x['sample']) for x in map(json.loads,p.open()) if x['event']=='calibration.sample'];assert actual==reference
  private=list(map(json.loads,trace.open()));assert len(private)==1;row=private[0]['words'];npath=next((r/'outputs/air-torque-v114/native').glob(f'case{index}-tick{tick}-rep{rep}-private*'));native=list(map(json.loads,gzip.open(npath,'rt')));entry=next(x for x in native if x.get('function')=='local-torque-entry');rotation=next(x for x in native if x.get('function')=='rotation-input');world=next(x for x in native if x.get('function')=='rotation-result')['world_vector']['words'];integrated=next(x for x in native if x.get('function')=='integrator-tick')['integrator_body']['words'][22:25]
  assert row[:4]==rotation['rotation']['words'] and row[7:10]==rotation['local_vector']['words']
  before=entry['body_interface']['words'][55:58];zero_differences=[]
  for i,(a,b) in enumerate(zip(row[4:7],before)):
   if a!=b:assert a&0x7fffffff==0 and b&0x7fffffff==0;zero_differences.append(i)
  dt=f(row[10]);delta=[bits(sf(f(x)*dt)) for x in world];assert row[11:14]==delta
  added=[bits(sf(f(x)+f(y))) for x,y in zip(before,delta)]
  assert list(map(f,row[14:17]))==list(map(f,added))
  drag=sf(1-sf(sf(.05)*dt));assert [bits(sf(f(x)*drag)) for x in added]==integrated
  captures.append({'tick':tick,'repeat':rep,'all120_Native_observables_exact':True,'pose_and_local_acceleration_words_exact':True,'incoming_omega_values_exact':True,'private_signed_zero_axis_differences':zero_differences,'delta_words_exact':True,'pre_drag_velocity_values_exact':True,'Native_integrator_velocity_words_exact':True});print(tag,'all120states and same-input delta/integrator exact',flush=True)
(out/'manifest.json').write_text(json.dumps({'scope':'Actual live final Source vs actual unchanged original torque transformation and integrator checkpoints. Signed-zero ABI differences separately reported; no Native states feed runtime.', 'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'captures':captures,'goal_complete':False},indent=2)+'\n')
