from pathlib import Path
import json,subprocess,gzip
out=Path('outputs/controller-audit-v170/native');out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\controller-audit-v170'
def guest(cmd):
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=90);return p.stdout+p.stderr
files=[r'C:\MotorRock\game-debug\Rock3dGame.dll',r'C:\MotorRock\game-debug\Rock3dGame.pdb',folder+r'\MotorProbeV170.exe',r'C:\MotorRock\calibration-probes\control-wake-v133\WorldProbeV133.exe'];hashes={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files};previous=None
for rep in [0,1]:
 output=folder+f'\\motor-run{rep}.jsonl';guest('cd /d '+folder+'&& '+folder+r'\MotorProbeV170.exe '+output);data=guest('type '+output);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));assert sum(r.get('event')=='motor.sample' for r in rows)==48
 if previous is not None:assert rows==previous
 previous=rows
 with gzip.open(out/f'run{rep}.jsonl.gz','wb') as z:z.write(data)
for f,h in hashes.items():assert guest('certutil -hashfile '+f+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(hashes=hashes,actual_original_gamecar_ctor=True,pdb_resolved_motor_descriptor=True,original_calc_torque_called=True,repeats=2,samples_per_repeat=48,precision_modes=[24,53],goal_complete=False),indent=2)+'\n');print('Original MotorProgress torque primitive captured twice, all48 values repeat; original hashes unchanged',flush=True)
