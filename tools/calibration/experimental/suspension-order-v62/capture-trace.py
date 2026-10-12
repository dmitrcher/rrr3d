from pathlib import Path
import os,json,subprocess,hashlib
root=Path.cwd();out=root/'outputs/material-friction-trace-v62';out.mkdir(exist_ok=False)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration')
base={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};base.update(json.load(open(root/'outputs/lost-tires-broad-v59/capture-manifest.json'))['environment'])
base.update({'RRR3D_EXPERIMENT_NATIVE_BOX_TRIANGLE_SAT_V60':'1','RRR3D_EXPERIMENT_NATIVE_PRE_SOLVE_CAP_V60':'1','RRR3D_EXPERIMENT_NATIVE_CONTACT_PAIR_ORDER_V60':'1','RRR3D_EXPERIMENT_NATIVE_MESH_INVERSE_TRANSLATION_V61':'1','RRR3D_EXPERIMENT_NATIVE_RAW_WORLD_FRAME_V61':'1','RRR3D_EXPERIMENT_NATIVE_HULL_STATIC_FRAME_V61':'1','RRR3D_EXPERIMENT_NATIVE_PRE_CALLBACK_FRICTION_V62':'1'})
for tick in [61,62]:
 for rep in range(2):
  env=base.copy();env.update({'RRR3D_CONTACT_ROWS':'1','RRR3D_CONTACT_NATIVE_TRACE_STEP':str(tick),'RRR3D_CONTACT_LOCAL_INERTIA_TRACE':'1','RRR3D_CONTACT_GEOMETRY_INPUT_TRACE':'1','RRR3D_CONTACT_CACHE_INPUT_TRACE':'1','RRR3D_CONTACT_MATERIAL_STAGE_TRACE':'1','RRR3D_CONTACT_POINTS':'1'})
  with (out/f'{tick}-{rep}.log').open('wb') as log:subprocess.run([str(probe),str(out/f'{tick}-{rep}.jsonl'),'list:border-grounded'],env=env,check=True,timeout=120,stdout=log,stderr=log)
(out/'manifest.json').write_text(json.dumps({'environment':{k:v for k,v in base.items() if k.startswith(('RRR3D_','NX_','MOTORROCK_'))},'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'scope':'Read-only diagnostic joint prototype; not an accepted correction'},indent=2))
