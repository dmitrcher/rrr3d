from pathlib import Path
import gzip,hashlib,json,os,subprocess,time
R=Path.cwd();W=R/'work/controller-jump-runtime-v189';O=R/'outputs/controller-jump-runtime-v189';EXE=W/'SourceJumpRuntimeProbe';B=json.loads((W/'source-build.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();assert sha(EXE)==B['executable_sha256'] and sha(W/'source-probe.cpp')==B['source_sha256']
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');assert all(sha(active/p)==h for p,h in B['libraries'].items())
base=json.loads((R/'outputs/controller-clutch-v186/final-broad/manifest.json').read_text())['environment'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env['RRR3D_EXPERIMENT_GAMECAR_JUMP_V187']='1';env['RRR3D_EXPERIMENT_NATIVE_ACTOR_INERTIA_FRAME_V184']='1'
f=json.loads((O/'cases.json').read_text());assert sha(O/'input.words')==f['input_sha256'];runs=[]
for rep in range(2):
 dst=O/f'source-run{rep}.jsonl';dst.unlink(missing_ok=True);t=time.monotonic()
 with (W/f'source-run{rep}.log').open('w') as log:subprocess.run([str(EXE),str(O/'input.words'),str(dst)],env=env,stdout=log,stderr=log,check=True,timeout=180)
 raw=dst.read_bytes();rows=[json.loads(s) for s in raw.splitlines()];assert len(rows)==f['count'] and [x['case'] for x in rows]==list(range(f['count']));runs.append(dict(repeat=rep,states=len(rows),raw_sha256=hashlib.sha256(raw).hexdigest(),elapsed_seconds=time.monotonic()-t));(O/f'source-run{rep}.jsonl.gz').write_bytes(gzip.compress(raw,mtime=0));print('Source',rep,len(rows),'states',flush=True)
(O/'source-manifest.json').write_text(json.dumps(dict(scope='Actual Source GameCar command plus Jolt step; synthetic contact input, one method-call delta independent from fixed integration delta1/60',build=B,environment={k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},runs=runs,vm_started=False,goal_complete=False),indent=2)+'\n')
