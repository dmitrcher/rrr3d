from pathlib import Path
import subprocess,gzip,json,hashlib
out=Path('outputs/cooked-contact-v145-initialized');out.mkdir(parents=True,exist_ok=False)
f=r'C:\MotorRock\calibration-probes\cooked-contact-v145';fixture=r'C:\MotorRock\calibration-probes\control-wake-v133\track0.words'
def guest(cmd):
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=120,check=True);return p.stdout+p.stderr
files=[f+'\\'+n for n in ['PhysXCore.dll','PhysXCooking.dll','WorldProbeMetaV145.exe']]+[fixture]
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode() for p in files}
ref=list(map(json.loads,gzip.open('outputs/game-control-wake-v133/native/track0-rep0.jsonl.gz','rt')))
metas=[]
for rep in [0,1]:
 print('Metadata capture',rep,flush=True)
 physical=f+f'\\physical{rep}.jsonl';meta=f+f'\\metadata{rep}.jsonl'
 (out/f'run{rep}.log').write_bytes(guest('cd /d '+f+'&& set MOTORROCK_COOKED_CONTACT_METADATA_V145='+meta+'&& WorldProbeMetaV145.exe '+fixture+' '+physical))
 data=guest('type '+physical);assert list(map(json.loads,data.decode('utf-8-sig').splitlines()))==ref
 with gzip.open(out/f'physical{rep}.jsonl.gz','wb') as z:z.write(data)
 data=guest('type '+meta);metas.append(list(map(json.loads,data.decode('utf-8-sig').splitlines())));assert metas[-1];(out/f'metadata{rep}.jsonl').write_bytes(data)
 print('900 public states preserved',flush=True)
assert metas[0]==metas[1]
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(hashes=hashes,repeats=2,public_states_per_repeat=900,public_output_unchanged=True,metadata_after_all_simulation=True,metadata_repeat_exact=True,goal_complete=False),indent=2)+'\n')
