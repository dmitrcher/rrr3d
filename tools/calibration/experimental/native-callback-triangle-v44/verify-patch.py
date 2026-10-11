import argparse,hashlib,json,pathlib,subprocess,tempfile,shutil
p=argparse.ArgumentParser();p.add_argument('--out',type=pathlib.Path,required=True);a=p.parse_args();here=pathlib.Path(__file__).resolve().parent
base=json.loads((here.parent/'native-callback-triangle-v44-base.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
with tempfile.TemporaryDirectory(prefix='motorrock-v41-patch-') as tmp:
 root=pathlib.Path(tmp)
 for name,h in base['files'].items():
  old=here/'before'/name;new=here/name;assert sha(new)==h['after_sha256'];dest=root/name;dest.parent.mkdir(parents=True,exist_ok=True)
  if h['before_sha256'] is not None:assert sha(old)==h['before_sha256'];shutil.copyfile(old,dest)
 subprocess.run(['git','apply','--check',str(here.parent/'native-callback-triangle-v44.patch')],cwd=root,check=True)
 subprocess.run(['git','apply',str(here.parent/'native-callback-triangle-v44.patch')],cwd=root,check=True)
 for name,h in base['files'].items():assert sha(root/name)==h['after_sha256'],name
 a.out.write_text(json.dumps({'files_checked':len(base['files']),'patch_applies':True,'all_after_sources_byte_exact':True},indent=2)+'\n');print('Two-file patch applies and reproduces the candidate sources byte for byte.')
