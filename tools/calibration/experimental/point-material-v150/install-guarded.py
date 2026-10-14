"""Apply V150 only to the verified V149 Source and research Jolt SDK."""
from pathlib import Path
import argparse,json,hashlib,shutil
p=argparse.ArgumentParser()
p.add_argument('--source-root',required=True,type=Path)
p.add_argument('--jolt-root',required=True,type=Path)
p.add_argument('--apply',action='store_true')
args=p.parse_args();here=Path(__file__).resolve().parent
changes=[]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else None
for entry in json.loads((here/'overlay.json').read_text()):
 group,name=entry['group'],entry['path']
 target=(args.source_root if group=='source' else args.jolt_root)/name
 overlay=here/'after'/group/name
 assert sha(overlay)==entry['after_sha256'],f'Overlay hash mismatch: {group}/{name}'
 actual=sha(target)
 if actual==entry['after_sha256']:continue
 assert actual==entry['before_sha256'],f'Unsupported base: {group}/{name}'
 changes.append((target,overlay))
# Validate every input before changing any file.
if args.apply:
 for target,overlay in changes:
  target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(overlay,target)
print(json.dumps(dict(applied=args.apply,files_to_change=len(changes),requires_full_abi_rebuild=True,goal_complete=False)))
