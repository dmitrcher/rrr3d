from pathlib import Path
import json,os,subprocess,hashlib,gzip,struct
root=Path.cwd();out=root/'outputs/tire-anchor-v91/mac-anchor-control';out.mkdir(parents=True,exist_ok=False);probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');names=['mobility/devildriver/engine3/lateral']
def load(p):
 if not p.exists():p=p.with_suffix(p.suffix+'.gz')
 with gzip.open(p,'rt') if str(p).endswith('.gz') else p.open() as f:return [r['sample'] for r in map(json.loads,f) if r.get('event')=='calibration.sample' and r['detail'] in names]
expected=load(root/'outputs/contact-order-v90/mac-kernel58/force-0.jsonl');assert len(expected)==600
entries=[];traces=[]
for rep in range(2):
 env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'work/frozen-v90/manifest.json').read_text())['environment']);p=out/f'control-{rep}.jsonl';trace=out/f'control-{rep}-anchor.jsonl';env['RRR3D_TIRE_ANCHOR_TRACE_FILE_V91']=str(trace)
 with (out/f'control-{rep}.log').open('wb') as log:subprocess.run([str(probe),str(p),'list:'+','.join(names)],env=env,check=True,stdout=log,stderr=log,timeout=180)
 assert load(p)==expected,'Read-only trace changed physical history';traces.append([json.loads(s) for s in trace.open()]);entries.append({'repeat':rep,'physical_all600_exact_v90':True,'trace_records':len(traces[-1]),'physical_sha256':hashlib.sha256(p.read_bytes()).hexdigest()});print(entries[-1],flush=True)
assert traces[0]==traces[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Optional read-only Mac trace; physical history exact V90.','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'two_repeats_exact':True,'environment':{k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_')) and k!='RRR3D_TIRE_ANCHOR_TRACE_FILE_V91'},'captures':entries,'goal_complete':False},indent=2)+'\n')
