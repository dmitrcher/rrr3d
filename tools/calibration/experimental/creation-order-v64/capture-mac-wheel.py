from pathlib import Path
import os,json,subprocess,hashlib
out=Path('outputs/mac-creation-order-trace-v64');out.mkdir(exist_ok=False)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration')
m=json.load(open('outputs/suspension-order-focused-v62/capture-manifest.json'));base={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};base.update(m['environment']);base.update(next(c['changes'] for c in m['captures'] if c['mode']=='joint'));base['RRR3D_EXPERIMENT_NATIVE_ROW_CREATION_ORDER_V64']='1'
expected=[r['sample'] for r in map(json.loads,open('outputs/creation-order-focused-v64/candidate_v64-run0.jsonl')) if r['event']=='calibration.sample' and r['detail']=='border-grounded']
for step in [63,64]:
 traces=[]
 for rep in range(2):
  path=out/f'{step}-{rep}.jsonl';trace=out/f'{step}-{rep}-wheel.jsonl';env=dict(base,RRR3D_WHEEL_SOLVER_TRACE_FILE_V62=str(trace.resolve()),RRR3D_WHEEL_SOLVER_TRACE_STEP_V62=str(step))
  with (out/f'{step}-{rep}.log').open('wb') as log:subprocess.run([str(probe),str(path),'list:border-grounded'],env=env,check=True,timeout=120,stdout=log,stderr=log)
  physical=[r['sample'] for r in map(json.loads,path.open()) if r['event']=='calibration.sample'];assert physical==expected
  traces.append([json.loads(l) for l in trace.open()])
 assert traces[0]==traces[1]
 print(step,len(traces[0]),'read-only physical preserved, repeats exact',flush=True)
(out/'manifest.json').write_text(json.dumps({'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'environment':{k:v for k,v in base.items() if k.startswith(('RRR3D_','NX_','MOTORROCK_'))},'physical_preserved':True,'two_repeats_exact':True},indent=2)+'\n')
