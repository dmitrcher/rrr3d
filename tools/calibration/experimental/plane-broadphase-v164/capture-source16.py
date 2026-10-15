from pathlib import Path
import os,json,subprocess
out=Path('outputs/plane-broadphase-v164/source-track16').resolve();out.mkdir(parents=True,exist_ok=False)
base=json.load(open('outputs/startup-import-v130/source-pair-loss-fixed/manifest.json'))['profile']
for track,tick in [(16,420)]:
 env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env.update(RRR3D_EXPERIMENT_ORIGINAL_STARTUP_V130='1',RRR3D_EXPERIMENT_NATIVE_PAIR_LOSS_WAKE_V131='1',RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128='1',RRR3D_EXPERIMENT_NATIVE_PLACED_MESH_V126='1')
 env['RRR3D_TIRE_ANCHOR_TRACE_FILE_V91']=str(out/f'track{track}-anchor.jsonl')
 env['RRR3D_TIRE_ANCHOR_TRACE_ALL_V132']='1'
 env['RRR3D_REAL_WORLD_GAMECAR_WAKE_V133']='1'
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
 env['RRR3D_EXPERIMENT_NATIVE_ACTOR_REACTIVATION_V163']='1';env['RRR3D_EXPERIMENT_NATIVE_PLANE_BROADPHASE_V164']='1'
 env['RRR3D_CONTACT_ORDER_TRACE_FILE_V79']=str(out/'initial-order.jsonl')
 env['RRR3D_SLEEP_MANAGER_TRACE_FILE_V160']=str(out/'managers.jsonl')
 env['RRR3D_CALLBACK_DISPATCH_TRACE_FILE_V108']=str(out/'dispatch.jsonl')
 env['RRR3D_WHEEL_CALLBACK_TRACE_FILE_V105']=str(out/'callbacks.jsonl')
 env['RRR3D_CONTACT_NATIVE_TRACE_STEP']=str(tick)
 env['RRR3D_CONTACT_ROWS']='1'
 env['RRR3D_CONTACT_ITERATION_ROWS']='1'
 for flag in ['RRR3D_CONTACT_GEOMETRY_INPUT_TRACE','RRR3D_CONTACT_EDGE_STAGE_TRACE','RRR3D_CONTACT_MATERIAL_STAGE_TRACE','RRR3D_CONTACT_CACHE_INPUT_TRACE']:env[flag]='1'
 env['RRR3D_CONTACT_FORCE_TRACE_FILE_V62']=str(out/'forces.jsonl')
 env['RRR3D_SCENE_DISPATCH_TRACE_FILE_V134']=str(out/'first-manager.jsonl')
 env['RRR3D_EXPERIMENT_NATIVE_WAKE_RETAIN_ANCHOR_V132']='1'
 env['RRR3D_NATIVE_TIRE_V55_TRACE']=str(out/f'track{track}-tire.jsonl')
 env['RRR3D_NATIVE_TIRE_TRACE_STEP_V86']=str(tick+1)
 env['RRR3D_SLEEP_TRACE_FILE_V83']=str(out/f'track{track}-sleep.jsonl')
 env['RRR3D_TRACE_FILE']=str(out/f'track{track}-physical.jsonl');env['RRR3D_SLEEP_POSE_TRACE_FILE_V127']=str(out/f'track{track}-pose.jsonl')
 for kind,version in [('PREP','V67'),('SOLVER','V62')]:
  env[f'RRR3D_WHEEL_{kind}_TRACE_FILE_{version}']=str(out/f'track{track}-{kind.lower()}.jsonl');env[f'RRR3D_WHEEL_{kind}_TRACE_STEP_{version}']=str(tick+1)
 with (out/f'track{track}-console.log').open('w') as log:subprocess.run(['/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/RRR3d.app/Contents/MacOS/RRR3d','--solo-physics-replay',f'--track={track}','--car=marauder'],env=env,stdout=log,stderr=log,check=True,timeout=90)
