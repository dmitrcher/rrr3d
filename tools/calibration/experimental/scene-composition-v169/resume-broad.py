from pathlib import Path
import json,os,subprocess,time,gzip,shutil,hashlib
root=Path.cwd();out=root/'outputs/scene-composition-v169/final-broad';m=json.load(open(out/'manifest.json'));probe=root/'work/frozen-v169-candidate/rrr3d_physics_calibration';assert hashlib.sha256(probe.read_bytes()).hexdigest()==m['probe_sha256'];assert len(m['captures'])==1
assert hashlib.sha256((out/'candidate-run0.jsonl.gz').read_bytes()).hexdigest()==json.load(open(out/'interrupted-repeat1.json'))['repeat0_sha256']
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(m['environment']);p=out/'candidate-run1.jsonl';t=time.monotonic()
with (out/'candidate-run1-resumed.log').open('wb') as log:subprocess.run([str(probe),str(p),'all'],env=env,stdout=log,stderr=log,check=True,timeout=1200)
with p.open('rb') as a,gzip.open(str(p)+'.gz','wb',compresslevel=5) as z:shutil.copyfileobj(a,z)
p.unlink();m['captures'].append(dict(repeat=1,elapsed_seconds=time.monotonic()-t,file=p.name+'.gz',resumed_after_interruption=True));(out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');print('Repeated608 full; original completed repeat0 unchanged',flush=True)
