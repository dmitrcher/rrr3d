"""Apply combined V153–V156 on verified V152 Source and V150 research SDK."""
from pathlib import Path
import argparse,json,hashlib,shutil
p=argparse.ArgumentParser();p.add_argument('--source-root',required=True,type=Path);p.add_argument('--jolt-root',required=True,type=Path);p.add_argument('--apply',action='store_true');args=p.parse_args();here=Path(__file__).resolve().parent

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest() if p.is_file() else None
roots={'source':args.source_root,'sdk':args.jolt_root}
for group,filename in [('source','source-prerequisites.json'),('sdk','sdk-prerequisites.json')]:
 for name,expected in json.loads((here/filename).read_text()).items():
  assert sha(roots[group]/name)==expected,f'Unsupported {group} prerequisite: {name}'
changes=[]
for e in json.loads((here/'overlay.json').read_text()):
 group=e['group'];relative=Path(e['path']);assert not relative.is_absolute() and '..' not in relative.parts
 target=roots[group]/relative;overlay=here/'after'/group/relative
 assert sha(overlay)==e['after_sha256'],f'Overlay hash mismatch: {e["path"]}'
 actual=sha(target)
 if actual==e['after_sha256']:continue
 assert actual==e['before_sha256'],f'Unsupported {group} base: {e["path"]}'
 changes.append((target,overlay))
if args.apply:
 for target,overlay in changes:target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(overlay,target)
print(json.dumps(dict(applied=args.apply,files_to_change=len(changes),sdk_layout_unchanged=True,goal_complete=False)))
