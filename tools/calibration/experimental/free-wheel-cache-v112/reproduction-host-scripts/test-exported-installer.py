from pathlib import Path
import json,shutil,tempfile,hashlib,subprocess,sys
root=Path.cwd();folder=root/'work/sync-rrr3d/tools/calibration/experimental/free-wheel-cache-v112';m=json.loads((folder/'base.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else None
with tempfile.TemporaryDirectory(prefix='motorrock-v112-installer-') as d:
 base=Path(d);roots={loc:base/loc for loc in ['sdk','adapter']}
 for f in m['files']:
  if f['before_sha256'] is not None:
   target=roots[f['location']]/f['path'];target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(folder/'before'/f['location']/f['path'],target)
 cmd=[sys.executable,str(folder/'apply-experiment.py'),'--sdk-root',str(roots['sdk']),'--adapter-root',str(roots['adapter'])]
 for state,extra in [('after',[]),('after',[]),('before',['--restore']),('before',['--restore'])]:
  subprocess.run(cmd+extra,check=True);assert all(sha(roots[f['location']]/f['path'])==f[state+'_sha256'] for f in m['files'])
 last=m['files'][-1];bad=roots[last['location']]/last['path'];bad.parent.mkdir(parents=True,exist_ok=True);bad.write_text('wrong base')
 observed={str(p):sha(p) for p in base.rglob('*') if p.is_file()};r=subprocess.run(cmd,capture_output=True,text=True);assert r.returncode!=0 and {str(p):sha(p) for p in base.rglob('*') if p.is_file()}==observed
 print('Apply/restore/idempotence/all-files-before-mutation wrong-last-file rejection passed')
(root/'outputs/physics-v112-guarded-installer-test.json').write_text(json.dumps({'apply':True,'restore':True,'idempotent':True,'wrong_last_file_no_mutations':True,'files':len(m['files']),'goal_complete':False},indent=2)+'\n')
