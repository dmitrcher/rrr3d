from pathlib import Path
import json,hashlib,shutil,subprocess,tempfile
p=Path(__file__).resolve().parent;root=p.parent.parent
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=active/'build/macos-arm64-m10/_deps/rrr3d_jolt-src'
with tempfile.TemporaryDirectory(prefix='v186-installer-') as tmp:
 roots={g:Path(tmp)/g for g in ['source','sdk']}
 for group,filename in [('source','source-prerequisites.json'),('sdk','sdk-prerequisites.json')]:
  for rel in json.load(open(p/filename)):
   dst=roots[group]/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2((active if group=='source' else sdk)/rel,dst)
 for e in json.load(open(p/'overlay.json')):
  if e['before_sha256'] is None:continue
  src=p/'before'/e['group']/e['path'];dst=roots[e['group']]/e['path'];dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(src,dst)
 args=['python3',str(p/'install-guarded.py'),'--source-root',str(roots['source']),'--jolt-root',str(roots['sdk']),'--apply']
 runs=[json.loads(subprocess.check_output(args)) for i in range(2)];assert [r['files_to_change'] for r in runs]==[6,0]
 rel=next(iter(json.load(open(p/'source-prerequisites.json'))));(roots['source']/rel).write_text('unsupported')
 before={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for r in roots.values() for f in r.rglob('*') if f.is_file()}
 result=subprocess.run(args,capture_output=True);assert result.returncode!=0
 assert before=={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for r in roots.values() for f in r.rglob('*') if f.is_file()}
 (p/'installer-proof.json').write_text(json.dumps(dict(runs=runs,unknown_prerequisite_rejected_before_any_source_or_sdk_mutation=True),indent=2)+'\n');print('Guarded combined overlay6then0; unknown prerequisite rejected without mutation')
