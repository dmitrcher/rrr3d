from pathlib import Path
import json,os,subprocess,gzip,hashlib
r=Path.cwd();o=r/'outputs/border-air-v121/mac-clamp';o.mkdir();p=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');ref=[]
for x in map(json.loads,(r/'outputs/pair-pole-v120/broad/candidate-run0.jsonl').open()):
 if x['event']=='calibration.sample' and x['detail']=='border-air-straight':ref.append(x['sample'])
for rep in [0,1]:
 private=o/f'run{rep}-private.jsonl';physical=o/f'run{rep}-physical.jsonl';env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.load(open(r/'work/frozen-v120-state-candidate/manifest.json'))['environment']);env['RRR3D_NATIVE_ANGULAR_CLAMP_TRACE_FILE_V121']=str(private);subprocess.run([str(p),str(physical),'list:border-air-straight'],env=env,capture_output=True,check=True,timeout=180);actual=[x['sample'] for x in map(json.loads,physical.open()) if x['event']=='calibration.sample'];assert actual==ref and len(actual)==600
 for path in [private,physical]:
  with path.open('rb') as src,gzip.open(str(path)+'.gz','wb') as out:out.write(src.read())
  path.unlink()
 print(rep,'all600 V120 states unchanged',flush=True)
(o/'manifest.json').write_text(json.dumps({'scope':'Read-only actual Source angular clamp inputs/output, physics unmodified, two identical600-state V120 histories.','probe_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'goal_complete':False},indent=2)+'\n')
