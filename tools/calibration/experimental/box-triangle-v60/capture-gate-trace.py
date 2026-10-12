from pathlib import Path
import os,json,subprocess,hashlib
root=Path.cwd();out=root/'outputs/triangle-gate-trace-v60';out.mkdir(exist_ok=False)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_spring_probe')
base={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};base.update(json.load(open(root/'outputs/wheel-row-broad-v56/capture-manifest.json'))['environment'])

for name,tick in [('pose-v45-inverted-0',1),('body-slide-x',12)]:
 for rep in range(2):
  env=base.copy();env['RRR3D_EXPERIMENT_NATIVE_BOX_TRIANGLE_SAT_V60']=str(rep);env.update({'RRR3D_CONTACT_ROWS':'1','RRR3D_CONTACT_NATIVE_TRACE_STEP':str(tick),'RRR3D_CONTACT_LOCAL_INERTIA_TRACE':'1','RRR3D_CONTACT_GEOMETRY_INPUT_TRACE':'1','RRR3D_CONTACT_CACHE_INPUT_TRACE':'1','RRR3D_CONTACT_MATERIAL_STAGE_TRACE':'1','RRR3D_CONTACT_POINTS':'1'})
  with (out/f'{name}-{tick}-{rep}.log').open('wb') as log:subprocess.run([str(probe),str(out/f'{name}-{tick}-{rep}.jsonl'),'list:'+name],env=env,check=True,timeout=120,stdout=log,stderr=log)
(out/'manifest.json').write_text(json.dumps({'environment':base,'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'scope':'Read-only diagnostic joint prototype; not an accepted correction'},indent=2))
