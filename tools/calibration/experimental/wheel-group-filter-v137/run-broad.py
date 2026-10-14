from pathlib import Path
import json,os,subprocess,hashlib,gzip,shutil,time
root=Path.cwd();out=root/'outputs/group-filter-v137/final-broad';out.mkdir(parents=True,exist_ok=False)
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');probe=root/'work/frozen-v137-final/rrr3d_physics_calibration'
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.load(open('work/frozen-v123-candidate/manifest.json'))['environment']);env['RRR3D_EXPERIMENT_NATIVE_PLACED_MESH_V126']='1';env['RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128']='1';env['RRR3D_EXPERIMENT_NATIVE_PAIR_LOSS_WAKE_V131']='1';env['RRR3D_EXPERIMENT_NATIVE_WAKE_RETAIN_ANCHOR_V132']='1';env['RRR3D_EXPERIMENT_NATIVE_SCENE_DISPATCH_V134']='1';env['RRR3D_EXPERIMENT_NATIVE_GROUP_FILTER_V137']='1';captures=[]
for rep in range(2):
 p=out/f'candidate-run{rep}.jsonl';t=time.monotonic();print('608-case run',rep,'start',flush=True)
 with p.with_suffix('.log').open('wb') as log:subprocess.run([str(probe),str(p),'all'],env=env,stdout=log,stderr=log,check=True,timeout=600)
 with p.open('rb') as f,gzip.open(str(p)+'.gz','wb',compresslevel=5) as z:shutil.copyfileobj(f,z)
 p.unlink();captures.append(dict(repeat=rep,elapsed_seconds=time.monotonic()-t,file=p.name+'.gz'));print('608-case run',rep,'complete; trace compressed',flush=True)
 (out/'manifest.json').write_text(json.dumps(dict(probe_sha256=hashlib.sha256(probe.read_bytes()).hexdigest(),environment={k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},captures=captures),indent=2)+'\n')
