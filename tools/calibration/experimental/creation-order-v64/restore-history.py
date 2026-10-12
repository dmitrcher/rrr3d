"""Restore one exact archived file to its original workspace-relative path."""
from pathlib import Path
import argparse,json,zipfile,hashlib,os
p=argparse.ArgumentParser();p.add_argument('archive',type=Path);p.add_argument('member');p.add_argument('--workspace',type=Path,required=True);a=p.parse_args()
with zipfile.ZipFile(a.archive) as z:
 m=json.loads(z.read('manifest.json'));entry=next((e for e in m['files'] if e['member']==a.member),None)
 if entry is None:raise SystemExit('Member not listed in manifest')
 if entry.get('environment_sanitized'):raise SystemExit('Metadata was sanitized for publication and is not an exact host-environment restore')
 rel=Path(entry['original_relative_path']);root=a.workspace.resolve();target=(root/rel).resolve()
 if rel.is_absolute() or not target.is_relative_to(root):raise SystemExit('Unsafe destination')
 if target.exists():
  h=hashlib.sha256()
  with target.open('rb') as f:
   for c in iter(lambda:f.read(1024*1024),b''):h.update(c)
  if h.hexdigest()==entry['sha256']:print('Already restored:',target);raise SystemExit(0)
  raise SystemExit('Destination contains different data; refusing to overwrite')
 target.parent.mkdir(parents=True,exist_ok=True);tmp=target.with_name(target.name+'.restore-tmp');h=hashlib.sha256();size=0
 try:
  with z.open(a.member) as src,tmp.open('xb') as dst:
   for c in iter(lambda:src.read(1024*1024),b''):dst.write(c);h.update(c);size+=len(c)
  if size!=entry['bytes'] or h.hexdigest()!=entry['sha256']:raise ValueError('Hash mismatch')
  os.replace(tmp,target)
 except BaseException:
  if tmp.exists():tmp.unlink()
  raise
 print('Restored and verified:',target)
