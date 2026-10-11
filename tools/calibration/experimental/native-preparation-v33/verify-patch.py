from pathlib import Path
import argparse,hashlib,json,shutil,subprocess,tempfile
p=argparse.ArgumentParser();p.add_argument('--snapshots',type=Path,required=True);a=p.parse_args();root=Path(__file__).resolve().parent.parent
contract=json.loads((root/'derived-contact-error-v33-base.json').read_text())
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
with tempfile.TemporaryDirectory(prefix='motorock-v33-patch-') as temporary:
 dest=Path(temporary)
 for name,entry in contract['files'].items():
  after=a.snapshots/'after'/name;assert sha(after)==entry['after_sha256']
  before=a.snapshots/'before'/name
  if entry['before_sha256'] is None:assert not before.exists()
  else:
   assert sha(before)==entry['before_sha256'];(dest/name).parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(before,dest/name)
 subprocess.run(['patch','--batch','-p1','-i',str((root/'derived-contact-error-v33.patch').resolve())],cwd=dest,check=True,stdout=subprocess.DEVNULL)
 for name,entry in contract['files'].items():assert sha(dest/name)==entry['after_sha256']
print('All six before/after SHA values verified; clean patch reproduces every after-file exactly.')
