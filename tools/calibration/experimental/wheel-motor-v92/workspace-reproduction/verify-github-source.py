from pathlib import Path
import subprocess,json,hashlib
root=Path.cwd();repo=root/'work/sync-rrr3d';rel=Path('tools/calibration/experimental/wheel-motor-v92');source=repo/rel;head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
remote=json.loads(subprocess.check_output(['gh','api','repos/dmitrcher/rrr3d/git/ref/heads/macos-arm64'],text=True));assert remote['object']['sha']==head
tree=json.loads(subprocess.check_output(['gh','api','repos/dmitrcher/rrr3d/git/trees/'+head+'?recursive=1'],text=True));assert not tree.get('truncated');blobs={f['path']:f['sha'] for f in tree['tree'] if f['type']=='blob'}
count=0
for p in source.rglob('*'):
 if not p.is_file():continue
 data=p.read_bytes();sha=hashlib.sha1(b'blob '+str(len(data)).encode()+b'\0'+data).hexdigest();assert blobs[str(p.relative_to(repo))]==sha;count+=1
manifest=json.loads((source/'sha256-manifest.json').read_text())
for f in manifest['files']:
 data=(source/f['path']).read_bytes();assert len(data)==f['bytes'] and hashlib.sha256(data).hexdigest()==f['sha256']
report={'source_commit':head,'remote_branch_head_exact':True,'all_remote_git_blob_identities_exact':True,'files':count,'manifest_members':len(manifest['files']),'all_local_sha256_exact':True,'goal_complete':False};(root/'outputs/physics-v92-github-verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
