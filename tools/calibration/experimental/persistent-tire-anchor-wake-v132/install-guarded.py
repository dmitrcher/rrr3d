"""Apply the V130/V131 two-file overlay on the verified V128 source base."""
from pathlib import Path
import argparse,json,hashlib,shutil
p=argparse.ArgumentParser();p.add_argument('--source-root',required=True,type=Path);p.add_argument('--apply',action='store_true');args=p.parse_args();here=Path(__file__).resolve().parent
manifest=json.loads((here/'source-change.json').read_text())['files'];changes=[]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest() if path.exists() else None
for name,entry in manifest.items():
 target=args.source_root/name;overlay=here/'after/source'/name
 assert sha(overlay)==entry['after_sha256'],f'Overlay hash mismatch: {name}'
 actual=sha(target)
 if actual==entry['after_sha256']:continue
 assert actual==entry['before_sha256'],f'Unsupported source base: {name}'
 changes.append((target,overlay))
if args.apply:
 for target,overlay in changes:
  target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(overlay,target)
print(json.dumps({'applied':args.apply,'files_to_change':len(changes),'goal_complete':False}))
