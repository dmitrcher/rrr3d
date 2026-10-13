from pathlib import Path
import subprocess,json,hashlib
root=Path.cwd();repo=root/'work/sync-rrr3d';prefix='tools/calibration/experimental/tire-order-report-v87/';folder=repo/prefix
sha=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
remote=subprocess.check_output(['git','ls-remote','origin','refs/heads/macos-arm64'],cwd=repo,text=True).split()[0];assert remote==sha
api=json.loads(subprocess.check_output(['gh','api',f'repos/dmitrcher/rrr3d/git/trees/{sha}?recursive=1'],cwd=repo));assert not api.get('truncated');tree={f['path']:f['sha'] for f in api['tree'] if f['type']=='blob' and f['path'].startswith(prefix)}
m=json.load(open(folder/'source-manifest.json'));entries=[]
for p in sorted(folder.rglob('*')):
 if not p.is_file():continue
 data=p.read_bytes();key=str(p.relative_to(repo));blob=hashlib.sha1(b'blob '+str(len(data)).encode()+b'\0'+data).hexdigest();assert tree.get(key)==blob,key;entries.append({'path':key,'sha256':hashlib.sha256(data).hexdigest(),'git_blob':blob})
assert len(entries)==len(tree)
for f in m['files']:assert hashlib.sha256((folder/f['path']).read_bytes()).hexdigest()==f['sha256']
report={'commit':sha,'branch':'macos-arm64','remote_head_exact':True,'remote_tree_files':len(entries),'source_manifest_files':len(m['files']),'all_files_match_remote_blob':True,'all_source_manifest_sha256_exact':True,'url':f'https://github.com/dmitrcher/rrr3d/commit/{sha}','entries':entries,'goal_complete':False};(root/'outputs/physics-v87-github-verification.json').write_text(json.dumps(report,indent=2)+'\n');print(sha,len(entries),'remote files exact; SHA256 source manifest exact')
state=root/'DEVELOPMENT_STATE.md';state.write_text(state.read_text()+f'\nV87 GitHub source/evidence synchronization VERIFIED: macos-arm64 {sha}, all{len(entries)}remoteGitblobs matchactualfiles andall{len(m["files"])}SHA256manifest entries exact. outputs/physics-v87-github-verification.json. No newfullgameinstaller; normalinstalledbaseline remainsV73.\n')
