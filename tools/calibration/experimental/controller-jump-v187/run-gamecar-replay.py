import os,json,subprocess,hashlib
from pathlib import Path
root=Path.cwd(); out=root/'outputs/controller-jump-v187/gamecar-event-replay';out.mkdir(parents=True,exist_ok=True)
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d')
exe=active/'build/macos-arm64-m10/Debug/RRR3d.app/Contents/MacOS/RRR3d'
assert b'RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128' in exe.read_bytes(), 'Candidate binary not built'
base=json.load(open(root/'outputs/real-map-v124/mac-actuators/manifest.json'))['environment']
profile={k:v for k,v in base.items() if k.startswith('RRR3D_EXPERIMENT') or k in ('NX_SPRING_ITERATIONS','RRR3D_CALIBRATION_PROFILE','RRR3D_SEED','RRR3D_REAL_WORLD_ACTUATORS_V124')}
profile['RRR3D_REAL_WORLD_ACTUATORS_V124']='1'
profile['RRR3D_CALIBRATION_ISOLATE_NEUTRAL_STABILIZATION']='0'
results=[]
for track in [0]:
 for mode in ('off','on0','on1'):
  log=out/(f'track{track}-'+mode+'.jsonl');
  if log.exists():continue
  env={k:v for k,v in os.environ.items() if not k.startswith('RRR3D_')};env.update(profile);env['RRR3D_EXPERIMENT_GAMECAR_CLUTCH_V186']='1';env['RRR3D_EXPERIMENT_GAMECAR_JUMP_V187']='1'
  env['RRR3D_EXPERIMENT_GAMECAR_STEER_DEADBAND_V179']='1'
  env['RRR3D_EXPERIMENT_GAMECAR_STEER_DIRECTION_V183']='1'
  env['RRR3D_EXPERIMENT_NATIVE_ACTOR_INERTIA_FRAME_V184']='1'
  env['RRR3D_EXPERIMENT_GAMECAR_STEER_MATRIX_V181']='1'
  env['RRR3D_EXPERIMENT_GAMECAR_STABILIZE_V185']='0' if mode=='off' else '1'
  env['RRR3D_STABILIZE_TRACE_FILE_V113']=str(log.with_suffix('.stabilize.jsonl'))
  env['RRR3D_EXPERIMENT_GAMECAR_STEER_V177']='1'
  env['RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173']='1'
  env['RRR3D_GAMECAR_STEERING_TRACE_FILE_V176']=str(log.with_suffix('.steering.jsonl'))
  env['RRR3D_EXPERIMENT_ORIGINAL_STARTUP_V130']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_LOSS_WAKE_V131']='1'
  env['RRR3D_EXPERIMENT_NATIVE_WAKE_RETAIN_ANCHOR_V132']='1'
  env['RRR3D_REAL_WORLD_GAMECAR_WAKE_V133']='0';env['RRR3D_REAL_WORLD_ACTUATORS_V124']='0';env['RRR3D_GAMECAR_EVENT_REPLAY_V171']='1';env['RRR3D_EXPERIMENT_GAMECAR_WAKE_V171']='1'
  env['RRR3D_EXPERIMENT_NATIVE_SCENE_DISPATCH_V134']='1'
  env['RRR3D_EXPERIMENT_NATIVE_GROUP_FILTER_V137']='1'
  env['RRR3D_EXPERIMENT_NATIVE_HYBRID_RAY_V143']='1'
  env['RRR3D_EXPERIMENT_NATIVE_LOCAL_CONTACT_V149']='1'
  env['RRR3D_EXPERIMENT_NATIVE_POINT_MATERIAL_V150']='1'
  env['RRR3D_EXPERIMENT_NATIVE_CONVERTER_FRAME_V151']='1'
  env['RRR3D_EXPERIMENT_NATIVE_WORLD_CONTACT_V151']='1'
  env['RRR3D_EXPERIMENT_NATIVE_MIXED_CONTACT_V152']='1'
  env['RRR3D_EXPERIMENT_NATIVE_SHAPE_CENTER_V153']='1'
  env['RRR3D_EXPERIMENT_NATIVE_RETAINED_DOT_V154']='1';env['RRR3D_EXPERIMENT_NATIVE_EDGE_CROSS_V155']='1';env['RRR3D_EXPERIMENT_NATIVE_CONCAVE_EDGES_V156']='1';env['RRR3D_EXPERIMENT_NATIVE_WORLD_RAY_NORMAL_V157']='1';env['RRR3D_EXPERIMENT_NATIVE_POLYGON_PLANE_V158']='1';env['RRR3D_EXPERIMENT_NATIVE_COOKED_ORDER_V159']='1';env['RRR3D_EXPERIMENT_NATIVE_SLEEP_LIFECYCLE_V160']='1'
  env['RRR3D_EXPERIMENT_NATIVE_ACTOR_REACTIVATION_V163']='1';env['RRR3D_EXPERIMENT_NATIVE_SCENE_COMPOSITION_V169']='1';env['RRR3D_EXPERIMENT_NATIVE_LIVE_MANAGER_V168']='1';env['RRR3D_EXPERIMENT_NATIVE_PAIR_RETIREMENT_V166']='1';env['RRR3D_EXPERIMENT_NATIVE_PAIR_PUBLICATION_V165']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PLANE_BROADPHASE_V164']='1'
  env.update(RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128='1',RRR3D_ENERGY_SLEEP_TRACE_FILE_V128=str(log.with_suffix('.energy.jsonl')))
  env.update(RRR3D_TRACE_FILE=str(log),RRR3D_WORLD_FIXTURE_FILE_V124=str(log.with_suffix('.words')),RRR3D_AI_DRIVER='1',RRR3D_EXPERIMENT_NATIVE_PLACED_MESH_V126='1')
  with log.with_suffix('.log').open('w') as console:
   p=subprocess.run([str(exe),'--solo-physics-replay',f'--track={track}','--car=marauder'],env=env,stdout=console,stderr=subprocess.STDOUT,timeout=120)
  assert p.returncode==0,(mode,p.returncode)
  rows=list(map(json.loads,log.read_text().splitlines()));samples=[x['sample'] for x in rows if x['event']=='solo.sample'];assert len(samples)==900
  semantic=[{'event':x['event'],'sample':x['sample']} for x in rows if x['event'] in ('solo.sample','solo.drive')]
  digest=hashlib.sha256(json.dumps(semantic,sort_keys=True).encode()).hexdigest()
  results.append(dict(track=track,mode=mode,samples=900,semantic_sha256=digest,effective_environment={k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))}))
  print(mode,900,digest,flush=True)
(out/'manifest.json').write_text(json.dumps(dict(executable_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),profile=profile,runs=results),indent=2)+'\n')

a,b=[r for r in results if r['mode'] in ('on0','on1')];assert a['semantic_sha256']==b['semantic_sha256']
traces=[list(map(json.loads,(out/f'track0-on{i}.stabilize.jsonl').read_text().splitlines())) for i in range(2)];assert traces[0]==traces[1]
inputs=[r for r in traces[0] if r['phase']=='game-input-v185'];assert len(inputs)>0
(out/'bridge-verification.json').write_text(json.dumps(dict(scope='Actual Source Session GameCar command bridge selects PC24; functional deterministic 900-step event replay. Not a full original Windows race reference.',samples=900,repeats_identical=True,gamecar_pc24_inputs=len(inputs),goal_complete=False),indent=2)+'\n')
