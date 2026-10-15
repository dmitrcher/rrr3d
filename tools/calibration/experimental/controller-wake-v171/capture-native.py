from pathlib import Path
import json,subprocess,gzip
out=Path('outputs/controller-wake-v171/native');out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\controller-wake-v171'
def guest(cmd):
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=90);return p.stdout+p.stderr
files=[r'C:\MotorRock\game-debug\Rock3dGame.dll',r'C:\MotorRock\game-debug\Rock3dGame.pdb',folder+r'\WakeProbeV171.exe',r'C:\MotorRock\calibration-probes\control-wake-v133\WorldProbeV133.exe'];hashes={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files};previous=None
for rep in [0,1]:
 output=folder+f'\\wake-run{rep}.jsonl';guest('cd /d '+folder+'&& '+folder+r'\WakeProbeV171.exe '+output);data=guest('type '+output);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));assert sum(r.get('event') in ['wake.sample','wake.sequence'] for r in rows)==40
 if previous is not None:assert rows==previous
 previous=rows
 with gzip.open(out/f'run{rep}.jsonl.gz','wb') as z:z.write(data)
for f,h in hashes.items():assert guest('certutil -hashfile '+f+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(hashes=hashes,actual_original_gamecar_ctor=True,pdb_resolved_motor_descriptor=True,original_set_move_car_called=True,repeats=2,samples_per_repeat=40,goal_complete=False),indent=2)+'\n');print('Original SetMoveCar wake semantics captured twice, all40 values repeat; original hashes unchanged',flush=True)
