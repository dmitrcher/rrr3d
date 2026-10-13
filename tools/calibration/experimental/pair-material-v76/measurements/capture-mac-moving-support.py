from pathlib import Path
import os,json,subprocess,gzip,hashlib
root=Path.cwd();out=root/'outputs/pair-material-v76/mac-moving-support';out.mkdir(exist_ok=False);probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');assert all(v in probe.read_bytes() for v in [b'RRR3D_EXPERIMENT_NATIVE_PAIR_FRAME_V74',b'RRR3D_EXPERIMENT_NATIVE_PAIR_INERTIA_V74',b'RRR3D_EXPERIMENT_NATIVE_PAIR_MASS_V74',b'RRR3D_EXPERIMENT_NATIVE_PAIR_CLIP_V74',b'RRR3D_EXPERIMENT_NATIVE_PAIR_LOCAL_CACHE_V75',b'RRR3D_EXPERIMENT_NATIVE_PAIR_MATERIAL_TRANSPORT_V75',b'RRR3D_EXPERIMENT_NATIVE_PAIR_ERROR_V75',b'RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_INVERSE_V76',b'RRR3D_EXPERIMENT_NATIVE_PAIR_RAW_LOCAL_V76',b'RRR3D_EXPERIMENT_NATIVE_PAIR_MATERIAL_LOCAL_V76']);base=json.load(open(root/'outputs/pair-impulse-v73/broad-cache/capture-manifest.json'))['environment']
names=['pair-stock-pole-1'];expected={n:[] for n in names}
entries=[]
for name,tick in [(names[0],28)]:
 for rep in range(2):
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env['RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_MANIFOLD_V71']='1';env['RRR3D_EXPERIMENT_NATIVE_PAIR_STATIC_FRICTION_V71']='1'
  prefix=out/f'{name}-{rep}';path=prefix.with_suffix('.jsonl');prep=out/f'{name}-{rep}-prep.jsonl';wheel=out/f'{name}-{rep}-wheel.jsonl'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_SOLVER_V73']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_CACHE_V73']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_FRAME_V74']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_INERTIA_V74']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_LEVERS_V74']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_MASS_V74']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_CLIP_V74']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_MATERIAL_TRANSPORT_V75']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_LOCAL_CACHE_V75']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_ERROR_V75']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_RAW_LOCAL_V76']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_MATERIAL_LOCAL_V76']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_INVERSE_V76']='1'
  env['RRR3D_PAIR_CACHE_TRACE_V75']='1'
  env['RRR3D_PAIR_GEOMETRY_TRACE_V74']='1'
  env['RRR3D_NATIVE_PAIR_TRACE_FILE_V73']=str(out/f'{name}-{rep}-native-pair.jsonl')
  env.update(RRR3D_CONTACT_ROWS='1',RRR3D_CONTACT_POINTS='1',RRR3D_CONTACT_NATIVE_TRACE_STEP=str(tick),RRR3D_CONTACT_ITERATION_ROWS='1',RRR3D_WHEEL_PREP_TRACE_FILE_V67=str(prep),RRR3D_WHEEL_PREP_TRACE_STEP_V67=str(tick+2),RRR3D_WHEEL_SOLVER_TRACE_FILE_V62=str(wheel),RRR3D_WHEEL_SOLVER_TRACE_STEP_V62=str(tick+2))
  with (out/f'{name}-{rep}.log').open('wb') as log:subprocess.run([str(probe),str(path),'list:'+name],env=env,check=True,timeout=180,stdout=log,stderr=log)
  rows=[x for x in map(json.loads,path.open()) if x['event']=='calibration.sample'];assert len(rows)==300
  events={}
  for x in map(json.loads,path.open()):events[x['event']]=events.get(x['event'],0)+1
  entries.append({'name':name,'repeat':rep,'tick':tick,'events':events,'physical_preserves_material_candidate':False,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()});print(name,rep,'read-only',events,flush=True)
(out/'manifest.json').write_text(json.dumps({'environment':base,'captures':entries,'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'goal_complete':False},indent=2)+'\n')
