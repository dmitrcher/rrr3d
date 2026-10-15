from pathlib import Path
import subprocess,json,gzip
out=Path('outputs/wheel-bounds-v161/native-proxy');out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\wheel-bounds-v161c';original=r'C:\MotorRock\calibration-probes\control-wake-v133';exe=folder+r'\WorldProbeProxyBoundsV161c.exe'
def guest(cmd):
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=180)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout+r.stderr
files=[exe,folder+r'\PhysXCore.dll',folder+r'\PhysXCooking.dll',original+r'\WorldProbeV133.exe'];hashes={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files};captures=[]
for track in [16,64]:
 reference=list(map(json.loads,gzip.open(f'outputs/game-control-wake-v133/native/track{track}-rep0.jsonl.gz','rt')))
 for rep in [0,1]:
  prefix=folder+f'\\bounds{track}-run{rep}';physical=prefix+'-physical.jsonl';bounds=prefix+'-bounds.jsonl';print('Original API bounds',track,rep,'start',flush=True)
  (out/f'track{track}-run{rep}.log').write_bytes(guest('cd /d '+folder+'&& set MOTORROCK_BOUNDS_METADATA_V161='+bounds+'&& '+exe+' '+original+f'\\track{track}.words '+physical))
  for kind,name in [('physical',physical),('bounds',bounds)]:
   data=guest('type '+name);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()))
   if kind=='physical':assert rows==reference,('Unvalidated getter helper changed original physics',track,rep)
   else:
    assert len(rows)==4500 and [(r['tick'],r['shape']) for r in rows]==[(t,s) for t in range(900) for s in range(5)]
    prior=list(map(json.loads,gzip.open(f'outputs/wheel-bounds-v161/native/track{track}-run{rep}-bounds.jsonl.gz','rt')));assert [r['words'] for r in rows]==[r['words'] for r in prior]
   with gzip.open(out/f'track{track}-run{rep}-{kind}.jsonl.gz','wb') as z:z.write(data)
  captures.append(dict(track=track,repeat=rep,original900states_preserved=True,public_world_bounds_records=4500));print('Original900states preserved,4500bounds captured',track,rep,flush=True)
for f,h in hashes.items():assert guest('certutil -hashfile '+f+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(hashes=hashes,captures=captures,scope='Separate initialized SDK/game helper; immutable133binary unchanged; public getters only, no original probes/SDK/reference mutation.',goal_complete=False),indent=2)+'\n')
