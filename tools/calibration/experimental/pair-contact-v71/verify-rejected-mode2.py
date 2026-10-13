from pathlib import Path
import os,json,subprocess,gzip,hashlib
root=Path.cwd();out=root/'outputs/pair-contact-v71/mac-rejected-mode2-frame';out.mkdir(exist_ok=False);probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');base=json.load(open(root/'outputs/pair-contact-v70/broad/capture-manifest.json'))['environment']
names=['pair-pole-oblique-0','pair-stock-head-0','pair-vertical-weak-peer-0'];expected={n:[] for n in names}
for r in map(json.loads,open(root/'outputs/pair-contact-v71/material-focused/pair_static-0.jsonl')):
 if r['event']=='calibration.sample' and r['detail'] in expected:expected[r['detail']].append(r['sample'])
entries=[]
for name,tick in [(names[0],37),(names[1],38),(names[2],38)]:
 for rep in range(1):
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env['RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_MANIFOLD_V71']='2';env['RRR3D_EXPERIMENT_NATIVE_PAIR_STATIC_FRICTION_V71']='1'
  prefix=out/f'{name}-{rep}';path=prefix.with_suffix('.jsonl');prep=out/f'{name}-{rep}-prep.jsonl';wheel=out/f'{name}-{rep}-wheel.jsonl'
  env.update(RRR3D_CONTACT_ROWS='1',RRR3D_CONTACT_POINTS='1',RRR3D_CONTACT_NATIVE_TRACE_STEP=str(tick),RRR3D_CONTACT_ITERATION_ROWS='1',RRR3D_WHEEL_PREP_TRACE_FILE_V67=str(prep),RRR3D_WHEEL_PREP_TRACE_STEP_V67=str(tick+2),RRR3D_WHEEL_SOLVER_TRACE_FILE_V62=str(wheel),RRR3D_WHEEL_SOLVER_TRACE_STEP_V62=str(tick+2))
  with (out/f'{name}-{rep}.log').open('wb') as log:subprocess.run([str(probe),str(path),'list:'+name],env=env,check=True,timeout=180,stdout=log,stderr=log)
  rows=[x for x in map(json.loads,path.open()) if x['event']=='calibration.sample'];assert [x['sample'] for x in rows]==expected[name]
  events={}
  for x in map(json.loads,path.open()):events[x['event']]=events.get(x['event'],0)+1
  entries.append({'name':name,'repeat':rep,'tick':tick,'events':events,'physical_preserves_rejected_unrestricted_candidate':True,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()});print(name,rep,'read-only',events,flush=True)
(out/'manifest.json').write_text(json.dumps({'environment':base,'captures':entries,'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'goal_complete':False},indent=2)+'\n')
