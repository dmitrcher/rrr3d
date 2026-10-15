from pathlib import Path
import json,os,subprocess,hashlib,gzip,shutil,time
root=Path.cwd();out=root/'outputs/controller-stabilize-v185/final-broad';out.mkdir(parents=True,exist_ok=False)
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');probe=active/'build/macos-arm64-m10/Debug/rrr3d_physics_calibration'
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.load(open('work/frozen-v123-candidate/manifest.json'))['environment']);env['RRR3D_EXPERIMENT_NATIVE_PLACED_MESH_V126']='1';env['RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128']='1';env['RRR3D_EXPERIMENT_NATIVE_PAIR_LOSS_WAKE_V131']='1';env['RRR3D_EXPERIMENT_NATIVE_WAKE_RETAIN_ANCHOR_V132']='1';env['RRR3D_EXPERIMENT_NATIVE_SCENE_DISPATCH_V134']='1';env['RRR3D_EXPERIMENT_NATIVE_GROUP_FILTER_V137']='1';env['RRR3D_EXPERIMENT_NATIVE_HYBRID_RAY_V143']='1';env['RRR3D_EXPERIMENT_NATIVE_LOCAL_CONTACT_V149']='1';env['RRR3D_EXPERIMENT_NATIVE_POINT_MATERIAL_V150']='1';env['RRR3D_EXPERIMENT_NATIVE_CONVERTER_FRAME_V151']='1';env['RRR3D_EXPERIMENT_NATIVE_WORLD_CONTACT_V151']='1';env['RRR3D_EXPERIMENT_NATIVE_MIXED_CONTACT_V152']='1';env['RRR3D_EXPERIMENT_NATIVE_SHAPE_CENTER_V153']='1';env['RRR3D_EXPERIMENT_NATIVE_RETAINED_DOT_V154']='1';env['RRR3D_EXPERIMENT_NATIVE_EDGE_CROSS_V155']='1';env['RRR3D_EXPERIMENT_NATIVE_CONCAVE_EDGES_V156']='1';env['RRR3D_EXPERIMENT_NATIVE_WORLD_RAY_NORMAL_V157']='1';env['RRR3D_EXPERIMENT_NATIVE_POLYGON_PLANE_V158']='1';env['RRR3D_EXPERIMENT_NATIVE_COOKED_ORDER_V159']='1';env['RRR3D_EXPERIMENT_NATIVE_SLEEP_LIFECYCLE_V160']='1';env['RRR3D_EXPERIMENT_NATIVE_ACTOR_REACTIVATION_V163']='1';env['RRR3D_EXPERIMENT_NATIVE_PLANE_BROADPHASE_V164']='1';env['RRR3D_EXPERIMENT_NATIVE_PAIR_PUBLICATION_V165']='1';env['RRR3D_EXPERIMENT_NATIVE_PAIR_RETIREMENT_V166']='1';env['RRR3D_EXPERIMENT_NATIVE_LIVE_MANAGER_V168']='1';env['RRR3D_EXPERIMENT_NATIVE_SCENE_COMPOSITION_V169']='1';env['RRR3D_EXPERIMENT_GAMECAR_STEER_V177']='1';env['RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173']='1';env['RRR3D_EXPERIMENT_GAMECAR_WAKE_V171']='1';env['RRR3D_EXPERIMENT_GAMECAR_STEER_DEADBAND_V179']='1';env['RRR3D_EXPERIMENT_GAMECAR_STEER_MATRIX_V181']='1';env['RRR3D_EXPERIMENT_GAMECAR_STEER_DIRECTION_V183']='1';env['RRR3D_EXPERIMENT_NATIVE_ACTOR_INERTIA_FRAME_V184']='1';env['RRR3D_EXPERIMENT_GAMECAR_STABILIZE_V185']='1';captures=[]
from concurrent.futures import ThreadPoolExecutor,as_completed
probe_sha=hashlib.sha256(probe.read_bytes()).hexdigest()
def capture(rep):
 assert hashlib.sha256(probe.read_bytes()).hexdigest()==probe_sha
 p=out/f'candidate-run{rep}.jsonl';t=time.monotonic();print('608-case run',rep,'start',flush=True)
 with p.with_suffix('.log').open('wb') as log:subprocess.run([str(probe),str(p),'all'],env=env,stdout=log,stderr=log,check=True,timeout=1200)
 with p.open('rb') as f,gzip.open(str(p)+'.gz','wb',compresslevel=5) as z:shutil.copyfileobj(f,z)
 p.unlink();assert hashlib.sha256(probe.read_bytes()).hexdigest()==probe_sha;print('608-case run',rep,'complete; trace compressed',flush=True)
 return dict(repeat=rep,elapsed_seconds=time.monotonic()-t,file=p.name+'.gz')
with ThreadPoolExecutor(max_workers=2) as executor:
 futures=[executor.submit(capture,rep) for rep in range(2)]
 for future in as_completed(futures):
  captures.append(future.result());(out/'manifest.json').write_text(json.dumps(dict(probe_sha256=probe_sha,parallel_independent_processes=2,environment={k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},captures=sorted(captures,key=lambda r:r['repeat'])),indent=2)+'\n')
