from pathlib import Path
import subprocess,base64,json,gzip,hashlib,sys
root=Path.cwd();out=root/'outputs/controller-stabilize-v185';boxOnly=len(sys.argv)>1 and sys.argv[1]=='box-only';out=out/'box-only' if boxOnly else out;folder=r'C:\MotorRock\calibration-probes\controller-stabilize-v185';native=root/'work/controller-stabilize-v185/native';receipt=json.loads((native/'observer-build.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();assert receipt['status']=='compiled' and receipt['source_sha256']==sha(native/'stabilize-probe.cpp')
def guest(ps):
 r=subprocess.run(['prlctl','exec','Windows 11','powershell.exe','-NoProfile','-EncodedCommand',base64.b64encode(("$ErrorActionPreference='Stop';$ProgressPreference='SilentlyContinue';"+ps).encode('utf-16le')).decode()],capture_output=True,text=True,timeout=120);assert r.returncode==0,(r.returncode,r.stdout[-3000:],r.stderr[-3000:]);return r.stdout
exe=folder+r'\StabilizeProbeV185.exe';assert receipt['exe_sha256'] in guest("(Get-FileHash '"+exe+"').Hash.ToLower()")
prev=None;runs=[]
for rep in range(2):
 text=guest("Set-Location '"+folder+"'; & .\\StabilizeProbeV185.exe accepted"+str(rep)+".jsonl"+(" box-only" if boxOnly else "")+"; if ($LASTEXITCODE -ne 0) {exit $LASTEXITCODE}; Get-Content accepted"+str(rep)+".jsonl");rows=[json.loads(x) for x in text.splitlines() if x.startswith('{')];stamp=[x for x in rows if x['event']=='observer.build'];assert len(stamp)==1 and stamp[0]['source_sha256']==receipt['source_sha256'] and stamp[0]['build_id']==receipt['build_id'];poses=[x for x in rows if x['event']=='stabilize.pose'];assert len(poses)==1440;assert all((x['raw_fp_control']&0x300)==0 for x in poses)
 if prev is not None:assert prev==rows
 prev=rows
 with gzip.open(out/f'accepted-native{rep}.jsonl.gz','wt') as f:
  for x in rows:f.write(json.dumps(x,separators=(',',':'))+'\n')
 runs.append(dict(repeat=rep,cases=len(poses),sha256=hashlib.sha256(text.encode()).hexdigest()));print(rep,len(poses),'actual original StabilizeForce states',flush=True)
modules=[x for x in rows if x['event']=='runtime.module'];assert len(modules)==3
paths=[x['path'] for x in modules]+[r'C:\MotorRock\game-debug\Rock3dGame.pdb',exe];hashes=json.loads(guest('Get-FileHash -LiteralPath '+','.join("'"+p+"'" for p in paths)+' -Algorithm SHA256 | Select-Object Path,Hash | ConvertTo-Json -Compress'))
known={'Rock3dGame.dll':'4483e8f402edb5c4639609dcf3fd805386e80f1d25b1c4829a8236b40a6b8a15','Rock3dGame.pdb':'818ba6d6386ef6c4c925d16b9edf2403f05f78385e36565df493c7173432770c','PhysXCore.dll':'c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb','PhysXCooking.dll':'2e5657ae13de0a0c79c56453b8c73ba3978069d55440134873c6534bbada522b','StabilizeProbeV185.exe':receipt['exe_sha256']}
for h in hashes:
 name=h['Path'].split('\\')[-1]
 if name in known:assert h['Hash'].lower()==known[name]
(out/'native-verification.json').write_text(json.dumps(dict(runs=runs,modules=modules,hashes=hashes,repeats_identical=True,scope='Actual original GameCar constructor and StabilizeForce, real SDK actor; PC24, one dynamic body/no AI. PDB-resolved property field writes supply isolated damping/clamp/contact/clutch inputs; no solver step or wheel contact inference. Inverse inertia is derived by PC24 reciprocal of actual public local inertia. Original source/binary unchanged.',goal_complete=False),indent=2)+'\n')
