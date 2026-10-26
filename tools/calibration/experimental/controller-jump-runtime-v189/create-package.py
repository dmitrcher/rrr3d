from pathlib import Path
import gzip,hashlib,json,shutil,subprocess,zipfile
R=Path.cwd();W=R/'work/controller-jump-runtime-v189';O=R/'outputs/controller-jump-runtime-v189';B=O/'MotorRock-Mac-Jump-Runtime-v189';B.mkdir(exist_ok=True);old=R/'work/v187-release-download/expanded/MotorRock-Mac-Jump-v187';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for source,dest in [(W/'SourceJumpRuntimeProbe','SourceJumpRuntimeProbe'),(R/'work/controller-jump-v187/SourceJumpGravityProbe','SourceJumpGravityProbe'),(W/'package-verify.py','verify.py'),(O/'input.words','input.words'),(O/'compatibility-run0.jsonl.gz','compatibility-jump.jsonl.gz'),(old/'native-sdk.jsonl.gz','native-sdk.jsonl.gz'),(old/'windows-source-v14.cfg','windows-source-v14.cfg'),(old/'PROJECT-LICENSE.txt','PROJECT-LICENSE.txt'),(R/'work/controller-jump-v187/JOLT-LICENSE.txt','JOLT-LICENSE.txt'),(old/'compare-float32.py','compare-float32.py'),(old/'compare.py','compare.py')]:shutil.copy2(source,B/dest)
for name in ['SourceJumpRuntimeProbe','SourceJumpGravityProbe']:
 subprocess.run(['strip','-x',str(B/name)],check=True)
 subprocess.run(['codesign','--force','--sign','-',str(B/name)],check=True)
shutil.copytree(old/'licenses',B/'licenses',dirs_exist_ok=True)
profile=json.loads((O/'source-manifest.json').read_text())['environment'];profile={k:v for k,v in profile.items() if 'TRACE' not in k};profile.pop('RRR3D_CALIBRATION_PROFILE',None);(B/'profile.json').write_text(json.dumps(profile,indent=2)+'\n')
(B/'README.md').write_text('''# V189 Mac diagnostic: bounded jump checks

Run `python3 verify.py /absolute/output/directory` on Apple Silicon macOS13+. The package checks530 isolated original-method/step inputs twice, two saved Windows SDK airborne trajectories twice, and a disabled-flag negative control. It launches no VM and needs no original game assets.

The530 expected records come from the actual original GameCar::JumpProgress on an actual PhysX actor under CrossOver26.3.0. They are compatibility-runtime evidence, not newly recorded native Windows gold. Contact inputs in that matrix are synthetic; initial orientation is identity, four wheels are free, and there is one car/no AI. Method-call delta and the integration delta1/60 are deliberately separate. Integer bit words are compared directly; signed zero is reported under the existing equivalence policy.

The two120-step trajectories use the previously immutable Windows SDK recordings. A fresh baseline and the actual original method under CrossOver also reproduce them. That does not certify arbitrary GameCar arithmetic; V188 rejected universal compatibility-runtime equivalence.

This is an experimental diagnostic package. The ordinary application/profile is not promoted. Complete OnFixedStep, real wheel-contact transitions, continuous races, AI/collisions and graphics remain outside this acceptance. No Microsoft/NVIDIA DLLs, PDBs, SDK headers/libraries or Windows executables are distributed.

Source and evidence: https://github.com/dmitrcher/rrr3d/tree/macos-arm64/tools/calibration/experimental/controller-jump-runtime-v189
''')
files={str(p.relative_to(B)):sha(p) for p in B.rglob('*') if p.is_file() and p.name!='manifest.json' and '__pycache__' not in p.parts};manifest=dict(scope='Bounded experimental jump-method public-observable checks,530 compatibility cases and240 saved native-Windows SDK states per repetition. Not full game/native Windows GameCar acceptance.',files=files,full_goal_complete=False);(B/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
archive=O/'MotorRock-Mac-Jump-Runtime-v189.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
 for p in B.rglob('*'):
  if p.is_file() and '__pycache__' not in p.parts:z.write(p,str(p.relative_to(O)))
receipt=dict(archive=str(archive),bytes=archive.stat().st_size,sha256=sha(archive),files=len(files)+1,production_source_changed=False,full_goal_complete=False);(W/'archive-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(receipt)
