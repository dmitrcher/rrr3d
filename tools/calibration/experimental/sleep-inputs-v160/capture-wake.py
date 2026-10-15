from pathlib import Path
import subprocess,json,gzip
out=Path('outputs/sleep-inputs-v160/native-wake656-v160');out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\control-wake-v133';observer=r'C:\MotorRock\calibration-probes\wake-inputs-v160\TraceWakeV160.exe'
def guest(cmd):
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=600)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout+r.stderr
files=[folder+r'\WorldProbeV133.exe',folder+r'\PhysXCore.dll',observer,folder+r'\track64.words'];hashes={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files};reference=list(map(json.loads,gzip.open('outputs/game-control-wake-v133/native/track64-rep0.jsonl.gz','rt')))
for rep in [0,1]:
 prefix=folder+f'\\wake656-v160-run{rep}';private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl';print('Read-only original energy0..656',rep,'start',flush=True)
 (out/f'run{rep}.log').write_bytes(guest('cd /d '+folder+'&& set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71=656&& '+observer+' '+private+' '+folder+r'\WorldProbeV133.exe '+folder+r'\track64.words '+physical))
 for kind,name in [('private',private),('physical',physical)]:
  data=guest('type '+name);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()))
  if kind=='physical':assert rows==reference
  else:assert any(x.get('function')=='energy-sleep-entry' and x.get('sample_tick_v71')==656 for x in rows)
  with gzip.open(out/f'run{rep}-{kind}.jsonl.gz','wb') as z:z.write(data)
 print('Original900states preserved',rep,flush=True)
for f,h in hashes.items():assert guest('certutil -hashfile '+f+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(hashes=hashes,immutable_public_states_preserved=900,repeats=2,goal_complete=False),indent=2)+'\n')
