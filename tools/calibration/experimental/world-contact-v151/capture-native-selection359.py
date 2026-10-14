from pathlib import Path
import subprocess,json,gzip
out=Path('outputs/world-contact-v151/native-selection359');out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\control-wake-v133';observer=r'C:\MotorRock\calibration-probes\contact-selection-v148\TraceSelectionV148.exe'
def guest(cmd):
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=600);return p.stdout+p.stderr
files=[folder+r'\WorldProbeV133.exe',folder+r'\PhysXCore.dll',observer,*[folder+f'\\track{t}.words' for t in [0]]];hashes={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files}
for track,tick in [(0,359)]:
 reference=list(map(json.loads,gzip.open(f'outputs/game-control-wake-v133/native/track{track}-rep0.jsonl.gz','rt')))
 for rep in [0,1]:
  private=folder+f'\\trace{track}-{rep}-private.jsonl';physical=folder+f'\\trace{track}-{rep}-physical.jsonl';print('Native map',track,'step',tick,'rep',rep,'start',flush=True)
  log=guest('cd /d '+folder+'&& set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71='+str(tick)+'&& '+observer+' '+private+' '+folder+r'\WorldProbeV133.exe '+folder+f'\\track{track}.words '+physical);(out/f'track{track}-run{rep}.log').write_bytes(log)
  for kind,f in [('private',private),('physical',physical)]:
   data=guest('type '+f)
   if kind=='physical':assert list(map(json.loads,data.decode('utf-8-sig').splitlines()))==reference
   else:
    rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));assert any(x.get('function')=='geometry-dispatch-before' for x in rows)
   with gzip.open(out/f'track{track}-run{rep}-{kind}.jsonl.gz','wb') as z:z.write(data)
  print('Native readonly capture complete900states unchanged',track,rep,flush=True)
for f,h in hashes.items():assert guest('certutil -hashfile '+f+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(hashes=hashes,original_public_states_preserved=900,tracks=[0],repeats=2,goal_complete=False),indent=2)+'\n')
