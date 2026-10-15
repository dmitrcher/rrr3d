from pathlib import Path
import tempfile,json,shutil,hashlib,subprocess
root=Path.cwd();active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=active/'build/macos-arm64-m10/_deps/rrr3d_jolt-src';stages=['controller-steering-matrix-v181','controller-steering-normalize-v182','controller-steering-direction-v183','actor-inertia-frame-v184'];base=root/'work'/stages[0]
sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest() if f.exists() else None
with tempfile.TemporaryDirectory(prefix='motorrock-v184-chain-') as tmp:
 roots={g:Path(tmp)/g for g in ['source','sdk']}
 for group,file in [('source','source-prerequisites.json'),('sdk','sdk-prerequisites.json')]:
  for name,h in json.load(open(base/file)).items():
   src=(active if group=='source' else sdk)/name;assert sha(src)==h,name;dst=roots[group]/name;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(src,dst)
 for e in json.load(open(base/'overlay.json')):
  if e['before_sha256'] is None:continue
  src=base/'before'/e['group']/e['path'];dst=roots[e['group']]/e['path'];dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(src,dst)
 report=[]
 for stage,count in zip(stages,[10,1,6,1]):
  args=['python3',str(root/'work'/stage/'install-guarded.py'),'--source-root',str(roots['source']),'--jolt-root',str(roots['sdk']),'--apply'];first=json.loads(subprocess.check_output(args));second=json.loads(subprocess.check_output(args));assert first['files_to_change']==count and second['files_to_change']==0;report.append(dict(stage=stage,first=first,second=second))
 final={}
 for stage in stages:
  for e in json.load(open(root/'work'/stage/'overlay.json')):final[(e['group'],e['path'])]=e['after_sha256']
 assert all(sha(roots[g]/name)==h and sha((active if g=='source' else sdk)/name)==h for (g,name),h in final.items())
 (root/'work/actor-inertia-frame-v184/chain-proof.json').write_text(json.dumps(dict(stages=report,final_matches_active_source_and_sdk=True,goal_complete=False),indent=2)+'\n');print('Complete181->182->183->184 chain exact, immediate idempotence, final active files match')
