from pathlib import Path
import json,os,subprocess,hashlib,time
root=Path.cwd();out=root/'outputs/pair-impulse-v73/broad-cache';out.mkdir(exist_ok=False);build=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10');probe=build/'Debug/rrr3d_physics_calibration';assert all(k in probe.read_bytes() for k in [b'RRR3D_EXPERIMENT_NATIVE_WHEEL_VEHICLE_QUERY_V70',b'RRR3D_EXPERIMENT_NATIVE_WHEEL_ROW_INERTIA_V56',b'RRR3D_EXPERIMENT_NATIVE_TIRE_REPORT_V56',b'RRR3D_EXPERIMENT_NATIVE_WHEEL_ROW_FRAME_V57',b'RRR3D_EXPERIMENT_NATIVE_TIRE_COLD_V57']);env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.load(open(root/'outputs/pair-contact-v72/broad/capture-manifest.json'))['environment']);env['RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_CACHE_V73']='1';env.pop('RRR3D_CONTACT_POINTS',None)
def digest(p):
 h=hashlib.sha256()
 with p.open('rb') as stream:
  for chunk in iter(lambda:stream.read(1024*1024),b''):h.update(chunk)
 return h.hexdigest()
entries=[]
for rep in range(2):
 print('broad',rep,'start',flush=True);p=out/f'candidate-run{rep}.jsonl';t=time.monotonic()
 with (out/f'candidate-run{rep}.log').open('wb') as log:subprocess.run([str(probe),str(p),'all'],env=env,stdout=log,stderr=log,check=True,timeout=600)
 entries.append({'file':p.name,'elapsed_seconds':time.monotonic()-t,'bytes':p.stat().st_size,'sha256':digest(p)});(out/'capture-manifest.json').write_text(json.dumps({'scope':'Fresh broad 608-case matrix; not certified by prior isolation gates.','probe_sha256':digest(probe),'environment':{k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},'captures':entries,'goal_complete':False},indent=2)+'\n');print('broad',rep,'complete',entries[-1],flush=True)
