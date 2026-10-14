"""Apply V151 on verified V150 Source and its unchanged research Jolt SDK."""
from pathlib import Path
import argparse,json,hashlib,shutil
p=argparse.ArgumentParser();p.add_argument('--source-root',required=True,type=Path);p.add_argument('--jolt-root',required=True,type=Path);p.add_argument('--apply',action='store_true');args=p.parse_args();here=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else None
for name,expected in json.loads((here/'sdk-prerequisites.json').read_text()).items():
 assert sha(args.jolt_root/name)==expected,f'Unsupported V150 SDK: {name}'
changes=[]
for e in json.loads((here/'overlay.json').read_text()):
 target=args.source_root/e['path'];overlay=here/'after/source'/e['path']
 assert sha(overlay)==e['after_sha256'],f'Overlay hash mismatch: {e["path"]}'
 actual=sha(target)
 if actual==e['after_sha256']:continue
 assert actual==e['before_sha256'],f'Unsupported Source base: {e["path"]}'
 changes.append((target,overlay))
if args.apply:
 for target,overlay in changes:target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(overlay,target)
print(json.dumps(dict(applied=args.apply,files_to_change=len(changes),sdk_unchanged=True,goal_complete=False)))
