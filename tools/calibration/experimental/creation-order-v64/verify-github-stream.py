from pathlib import Path
import subprocess,json,hashlib
repo='dmitrcher/rrr3d';tag='physics-suspension-v64-investigation-2026-10-12';expected_commit='0dcee9ee46ec9b6b6e07b2b731c8fd34b33a8048'
r=json.loads(subprocess.check_output(['gh','api',f'repos/{repo}/releases/tags/{tag}'],text=True));assets=[]
assert r['prerelease'] and not r['draft'] and len(r['assets'])>=2
for a in r['assets']:
 if a['name'] not in ['MotorRock-v64-Suspension-Investigation.zip','MotorRock-v64-Suspension-Investigation.sha256.txt']:continue
 p=Path('outputs')/a['name'];lh=hashlib.sha256()
 with p.open('rb') as f:
  for c in iter(lambda:f.read(1024*1024),b''):lh.update(c)
 print('streaming actual GitHub asset:',a['name'],flush=True)
 child=subprocess.Popen(['gh','release','download',tag,'--repo',repo,'--pattern',a['name'],'--output','-'],stdout=subprocess.PIPE,stderr=Path('outputs/physics-v64-download-stderr.log').open('wb'));h=hashlib.sha256();size=0
 for c in iter(lambda:child.stdout.read(1024*1024),b''):
  h.update(c);size+=len(c)
  if size%(64*1024*1024)==0:print('downloaded MiB',size//(1024*1024),flush=True)
 assert child.wait(timeout=30)==0,Path('outputs/physics-v64-download-stderr.log').read_text(errors='replace')
 assert size==a['size']==p.stat().st_size and h.hexdigest()==lh.hexdigest() and a['digest']=='sha256:'+h.hexdigest() and a['state']=='uploaded'
 assets.append({'name':a['name'],'bytes':size,'sha256':h.hexdigest(),'actual_download_matches_local':True,'download_streamed_without_disk_duplicate':True})
ref=json.loads(subprocess.check_output(['gh','api',f'repos/{repo}/git/ref/tags/{tag}'],text=True));assert ref['object']['sha']==expected_commit
remote=subprocess.check_output(['git','-C','work/sync-rrr3d','ls-remote','origin','refs/heads/macos-arm64'],text=True).split()[0];assert subprocess.run(['git','-C','work/sync-rrr3d','merge-base','--is-ancestor',expected_commit,remote]).returncode==0
record={'release_url':r['html_url'],'source_commit':expected_commit,'tag_verified_and_remote_branch_contains_checkpoint':True,'remote_branch_sha':remote,'assets':assets,'prerelease':True,'goal_complete':False}
Path('outputs/physics-v64-github-verification.json').write_text(json.dumps(record,indent=2)+'\n');print(json.dumps(record,indent=2),flush=True)
