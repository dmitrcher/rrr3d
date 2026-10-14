from pathlib import Path
import subprocess,json,hashlib
p=Path(__file__).resolve().parent
def guest(cmd):
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=30);return r.stdout+r.stderr
files=[r'C:\MotorRock\game-debug\Rock3dGame.dll',r'C:\MotorRock\game-debug\Rock3dGame.pdb',r'C:\Windows\SysWOW64\D3DX9_43.dll']
before={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files}
for rep in range(2):
 data=guest(r'cd /d C:\MotorRock\game-debug & C:\MotorRock\calibration-probes\startup-v129\StartupAuditV129.exe');(p/f'native-expanded-run{rep}.jsonl').write_bytes(data);rows=[json.loads(l) for l in data.splitlines()];assert len([r for r in rows if r['event']=='startup'])==136
 assert all(r.get('parent_is_null',True) for r in rows)
after={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files};assert before==after
assert (p/'native-expanded-run0.jsonl').read_bytes()==(p/'native-expanded-run1.jsonl').read_bytes()
(p/'native-identity-proof.json').write_text(json.dumps({'unchanged_original_files':before,'exact_repetitions':2,'directions':68,'precision_modes':[24,53],'states_per_repetition':136},indent=2)+'\n');print('expanded original functions repeatable, hashes unchanged')
