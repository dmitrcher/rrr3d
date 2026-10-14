#!/usr/bin/env python3
"""Apply the V128 research overlay only to the exact V127 source/SDK bases."""
import argparse,hashlib,json,shutil
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--sdk',type=Path,required=True);args=p.parse_args();here=Path(__file__).resolve().parent;manifest=json.loads((here/'source-change.json').read_text());checks=[]
for key,record in manifest['files'].items():
 tree,rel=key.split('/',1);root=args.source if tree=='source' else args.sdk;target=root/rel;overlay=here/'after'/key
 assert hashlib.sha256(overlay.read_bytes()).hexdigest()==record['after'],key
 if record['before'] is None:assert not target.exists(),f'new file already exists: {target}'
 else:assert hashlib.sha256(target.read_bytes()).hexdigest()==record['before'],f'base does not match: {target}'
 checks.append((target,overlay))
for target,overlay in checks:target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(overlay,target)
print('5 exact source/SDK overlays installed; rebuild research targets with the full parity profile. Production promotion not certified.')
