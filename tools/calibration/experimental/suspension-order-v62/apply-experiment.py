"""Install or restore exact default-off investigation sources; reject other bases."""
from pathlib import Path
import argparse,json,hashlib,shutil
p=argparse.ArgumentParser();p.add_argument('--sdk-root',type=Path,required=True);p.add_argument('--adapter-root',type=Path,required=True);p.add_argument('--restore',action='store_true');args=p.parse_args();root=Path(__file__).resolve().parent;m=json.loads((root/'base.json').read_text());actions=[]
for f in m['files']:
 target=(args.sdk_root if f['location']=='sdk' else args.adapter_root)/f['path'];expected=f['after_sha256'] if args.restore else f['before_sha256'];desired=f['before_sha256'] if args.restore else f['after_sha256'];actual=hashlib.sha256(target.read_bytes()).hexdigest() if target.exists() else None
 if actual==desired:continue
 if actual!=expected:raise SystemExit('Source differs from the recorded base: '+str(target))
 source=root/('before' if args.restore else 'after')/f['location']/f['path']
 if desired is not None and hashlib.sha256(source.read_bytes()).hexdigest()!=desired:raise SystemExit('Invalid snapshot: '+str(source))
 actions.append((target,source,desired))
for target,source,desired in actions:
 if desired is None:target.unlink()
 else:target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target)
print('Restored' if args.restore else 'Installed',len(actions),'recorded files. Rebuild required. All v61/v62 experiment flags remain off by default.')
