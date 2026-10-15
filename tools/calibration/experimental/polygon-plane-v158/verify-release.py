import zipfile,json,hashlib,os,subprocess,sys,importlib.util
from pathlib import Path
root=Path.cwd();download=root/'work/v158-release-download';archive=download/'MotorRock-v158-Projection-Plane-Investigation.zip';sha=hashlib.sha256(archive.read_bytes()).hexdigest();assert sha==json.load(open('work/polygon-plane-v158/archive-receipt.json'))['sha256'];assert sha==archive.with_suffix(archive.suffix+'.sha256').read_text().split()[0];expanded=download/'expanded';expanded.mkdir(exist_ok=False)
with zipfile.ZipFile(archive) as z:
 manifest={'files':json.loads(z.read('manifest.sha256.json'))}
 for name,digest in manifest['files'].items():assert hashlib.sha256(z.read(name)).hexdigest()==digest,name
 z.extractall(expanded)
probe=expanded/'mac/rrr3d_physics_calibration';probe.chmod(0o755);frozen=json.load(open('work/frozen-v158-candidate/manifest.json'));assert hashlib.sha256(probe.read_bytes()).hexdigest()==frozen['sha256']
change=json.loads((expanded/'source/overlay.json').read_text())
for entry in change:assert hashlib.sha256((expanded/'source/after'/entry['group']/entry['path']).read_bytes()).hexdigest()==entry['after_sha256']
assert (expanded/'windows/WorldProbeV133.exe').read_bytes()==(root/'work/game-control-wake-v133/native/WorldProbeV133.exe').read_bytes()
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.load(open(expanded/'environment.json')));env['RRR3D_CALIBRATION_PROFILE']=str(expanded/'profile.cfg');capture=download/'sleep-settle.jsonl'
subprocess.run([str(probe),str(capture),'list:sleep-settle'],env=env,capture_output=True,check=True,timeout=90)
expected=[]
with open('outputs/calibration-v14-source-windows.jsonl') as f:
 for line in f:
  if '"detail":"sleep-settle"' in line:
   x=json.loads(line)
   if x['event']=='calibration.sample':expected.append(x['sample'])
actual=[x['sample'] for x in map(json.loads,capture.open()) if x['event']=='calibration.sample'];assert len(expected)==len(actual)==600
spec=importlib.util.spec_from_file_location('words',root/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
assert all(words.physical(a)==words.physical(b) for a,b in zip(expected,actual))
report=dict(actual_github_download_sha256=sha,archive_members_verified=len(manifest['files']),downloaded_binary_matches_frozen_final=True,fresh_downloaded_probe_original_reference_F32_exact=600,goal_complete=False);(root/'outputs/physics-v158-github-release-verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
