from pathlib import Path
import json,os,subprocess,importlib.util,gzip,struct,hashlib
r=Path.cwd();out=r/'outputs/game-angular-v113/live-final';out.mkdir(parents=True,exist_ok=False)
spec=importlib.util.spec_from_file_location('words',r/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
cases=[('game-clamp-roll',0,'native-setter-v3',0),('game-clamp-pitch',0,'native-setter-v3',1),('game-clamp-mixed',0,'native-setter-v3',2),('game-angular-damping',1,'native-setter-v3',3),('game-clamp-roll',10,'native-euler',0),('game-clamp-pitch',31,'native-euler',1),('game-clamp-mixed',4,'native-euler',2)]
ref={n:[] for n,t,f,i in cases}
for x in map(json.loads,(r/'outputs/calibration-v14-source-windows.jsonl').open()):
 if x['event']=='calibration.sample' and x['detail'] in ref:ref[x['detail']].append(words.physical(x['sample']))
assert all(len(v)==120 for v in ref.values());probe=r/'work/frozen-v113-api-final/rrr3d_physics_calibration';checks=[];bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
for name,tick,folder,index in cases:
 for rep in [0,1]:
  tag=f'{name}-{tick}-{rep}';p=out/(tag+'-physical.jsonl');trace=out/(tag+'-private.jsonl');env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'work/frozen-v113-api-final/manifest.json').read_text())['environment']);env.update(RRR3D_STABILIZE_TRACE_FILE_V113=str(trace),RRR3D_STABILIZE_TRACE_STEP_V113=str(tick))
  subprocess.run([str(probe),str(p),'list:'+name],env=env,capture_output=True,check=True,timeout=180)
  actual=[words.physical(x['sample']) for x in map(json.loads,p.open()) if x['event']=='calibration.sample'];assert actual==ref[name]
  rows=list(map(json.loads,trace.open()));assert len(rows)==3
  npath=next((r/'outputs/game-angular-v113'/folder).glob(f'case{index}-tick{tick}-rep{rep}-private*'));native=list(map(json.loads,gzip.open(npath,'rt')))
  getter=next(x for x in native if x.get('function')=='momentum-getter-entry');prepared=next(x for x in native if x.get('function')=='momentum-setter-prepared');applied=next(x for x in native if x.get('function')=='momentum-setter-result')
  body=getter['body_interface']['words'];assert rows[0]['words'][:10]==body[48:52]+body[121:124]+body[55:58],(name,tick,rep,'incoming')
  assert rows[1]['words'][10:13]==prepared['setter_momentum']['words'],(name,tick,rep,'momentum')
  assert rows[2]['words'][:4]==prepared['body_interface']['words'][48:52],(name,tick,rep,'pose')
  assert rows[2]['words'][7:10]==applied['setter_velocity']['words'],(name,tick,rep,'velocity')
  checks.append({'case':name,'tick':tick,'repeat':rep,'all120_Native_observables_exact':True,'all10_incoming_pose_inertia_velocity_words_exact':True,'world_momentum_words_exact':True,'installed_pose_words_exact':True,'setter_velocity_words_exact':True});print(tag,'incoming/momentum/pose/velocity and120states exact',flush=True)
(out/'manifest.json').write_text(json.dumps({'scope':'Actual live Source vs actual immutable original Windows getter/setter and Euler checkpoints; no states feed runtime.', 'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'captures':checks,'goal_complete':False},indent=2)+'\n')
