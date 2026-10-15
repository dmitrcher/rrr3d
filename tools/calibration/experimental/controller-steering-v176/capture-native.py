from pathlib import Path
import subprocess,base64,gzip,json
root=Path.cwd();out=root/'outputs/controller-steering-v176';out.mkdir(exist_ok=True)
def guest(ps):
 r=subprocess.run(['prlctl','exec','Windows 11','powershell.exe','-NoProfile','-EncodedCommand',base64.b64encode(("$ProgressPreference='SilentlyContinue';"+ps).encode('utf-16le')).decode()],capture_output=True,text=True,timeout=90);assert r.returncode==0,(r.stdout,r.stderr);return r.stdout
paths=[r'C:\MotorRock\game-debug\Rock3dGame.dll',r'C:\MotorRock\game-debug\Rock3dGame.pdb',r'C:\MotorRock\calibration-probes\controller-steering-v176\SteeringProbeV176.exe'];expected=['4483e8f402edb5c4639609dcf3fd805386e80f1d25b1c4829a8236b40a6b8a15','818ba6d6386ef6c4c925d16b9edf2403f05f78385e36565df493c7173432770c',json.load(open(root/'work/controller-steering-v176/native/observer-build.json'))['exe_sha256']]
query='Get-FileHash '+','.join("'"+p+"'" for p in paths)+' -Algorithm SHA256 | Select-Object Path,Hash | ConvertTo-Json -Compress';before=json.loads(guest(query));assert [x['Hash'].lower() for x in before]==expected;previous=None
for rep in range(2):
 text=guest("Set-Location 'C:\\MotorRock\\calibration-probes\\controller-steering-v176'; & .\\SteeringProbeV176.exe accepted"+str(rep)+".jsonl; if ($LASTEXITCODE -ne 0) {exit $LASTEXITCODE};Get-Content accepted"+str(rep)+".jsonl");rows=[json.loads(x) for x in text.splitlines() if x.startswith('{')];assert len(rows)==15;assert all(x['before']==x['after_call']==x['after_step'] for x in rows)
 if previous is not None:assert rows==previous
 previous=rows
 with gzip.open(out/f'native{rep}.jsonl.gz','wt') as f:
  for row in rows:f.write(json.dumps(row,separators=(',',':'))+'\n')
after=json.loads(guest(query));assert before==after;(out/'native-verification.json').write_text(json.dumps(dict(cases_per_repeat=15,repeats=2,identical=True,original_physical_angles_preserved=True,hashes_before_after=after),indent=2)+'\n');print('15 actual Windows cases identical twice; immutable original hashes verified before/after')
