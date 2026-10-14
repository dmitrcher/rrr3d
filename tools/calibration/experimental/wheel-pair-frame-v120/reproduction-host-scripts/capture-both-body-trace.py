from pathlib import Path
import os,json,subprocess,hashlib,gzip
r=Path.cwd();o=r/'outputs/pair-pole-v120/both-body-trace';o.mkdir();p=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration');env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.load(open(r/'work/frozen-v120-state-candidate/manifest.json'))['environment']);expected=[x['sample'] for x in map(json.loads,open(r/'outputs/pair-pole-v120/target-state/candidate-0.jsonl')) if x['event']=='calibration.sample'];captures=[]
for rep in [0,1]:
 private=o/f'run{rep}-private.jsonl';physical=o/f'run{rep}-physical.jsonl';active=dict(env,RRR3D_NATIVE_SPRING_PAIR_TRACE_FILE_V120=str(private));subprocess.run([str(p),str(physical),'list:pair-stock-pole-1'],env=active,capture_output=True,check=True,timeout=180);actual=[x['sample'] for x in map(json.loads,physical.open()) if x['event']=='calibration.sample'];assert actual==expected and len(actual)==300
 for path in [private,physical]:
  with path.open('rb') as src,gzip.open(str(path)+'.gz','wb') as dst:dst.write(src.read())
  path.unlink()
 captures.append({'repeat':rep,'all300_decimal_states_preserve_candidate':True});print(rep,'all300 states preserve final candidate',flush=True)
(o/'manifest.json').write_text(json.dumps({'scope':'Read-only actual both-body before/after spring velocities, live poses, Jacobians, axes, mass and retained impulse. Physics candidate unmodified.','probe_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'captures':captures,'goal_complete':False},indent=2)+'\n')
