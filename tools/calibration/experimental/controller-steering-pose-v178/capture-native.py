from pathlib import Path
import subprocess,base64,json,gzip,hashlib
p=Path('outputs/controller-steering-pose-v178');p.mkdir(exist_ok=True);folder=r'C:\MotorRock\calibration-probes\controller-steering-pose-v178'
def guest(ps):
 r=subprocess.run(['prlctl','exec','Windows 11','powershell.exe','-NoProfile','-EncodedCommand',base64.b64encode(("$ProgressPreference='SilentlyContinue';"+ps).encode('utf-16le')).decode()],capture_output=True,text=True,timeout=90);assert r.returncode==0,(r.returncode,r.stdout,r.stderr);return r.stdout
previous=None;runs=[]
for rep in range(2):
 text=guest("Set-Location '"+folder+"'; & .\\SteeringPoseProbeV178.exe accepted"+str(rep)+".jsonl; if ($LASTEXITCODE -ne 0) {exit $LASTEXITCODE};Get-Content accepted"+str(rep)+".jsonl");rows=[json.loads(x) for x in text.splitlines() if x.startswith('{')];samples=[x for x in rows if x['event']=='steering.pose'];modules=[x for x in rows if x['event']=='runtime.module'];assert len(samples)==45 and len(modules)==4;assert all(x['contacts']==15 for x in samples)
 if previous is not None:assert rows==previous
 previous=rows
 with gzip.open(p/f'accepted-native{rep}.jsonl.gz','wt') as f:
  for row in rows:f.write(json.dumps(row,separators=(',',':'))+'\n')
 runs.append(dict(repeat=rep,cases=45,all_four_wheels_contact=True));print('Native',rep,45,'contact steering cases',flush=True)
paths=[x['path'] for x in modules]+[r'C:\MotorRock\game-debug\Rock3dGame.pdb',folder+r'\SteeringPoseProbeV178.exe'];literals=','.join("'"+x.replace("'","''")+"'" for x in paths);hashes=json.loads(guest('Get-FileHash -LiteralPath '+literals+' -Algorithm SHA256 | Select-Object Path,Hash | ConvertTo-Json -Compress'));known={'Rock3dGame.dll':'4483e8f402edb5c4639609dcf3fd805386e80f1d25b1c4829a8236b40a6b8a15','Rock3dGame.pdb':'818ba6d6386ef6c4c925d16b9edf2403f05f78385e36565df493c7173432770c','PhysXCore.dll':'c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb','PhysXCooking.dll':'2e5657ae13de0a0c79c56453b8c73ba3978069d55440134873c6534bbada522b','SteeringPoseProbeV178.exe':json.load(open('work/controller-steering-pose-v178/native/observer-build.json'))['exe_sha256']}
for h in hashes:
 name=h['Path'].split('\\')[-1]
 if name in known:assert known[name]==h['Hash'].lower(),h
(p/'native-verification.json').write_text(json.dumps(dict(runs=runs,actual_loaded_modules=modules,hashes=hashes,repeats_identical=True,scope='Real original WheelsProgress and fresh actual SDK scene per case, one dynamic body, four owned original wheels; direct manual steering, zero motor/brake, gravity/damping zero, matching physical contact. Every case starts from fresh scene timing; prior reused-scene pose comparisons are retained as rejected setup evidence.',goal_complete=False),indent=2)+'\n')
(p/'inputs.words').write_text(''.join(' '.join(str(x[k]) for k in ['case','speed_word','input_word','maximum_word'])+'\n' for x in samples))
