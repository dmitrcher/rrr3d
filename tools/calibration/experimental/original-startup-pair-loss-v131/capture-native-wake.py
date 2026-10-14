from pathlib import Path
import subprocess,json,gzip
out=Path('outputs/startup-import-v130/native-wake64');out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\startup-import-v130';observer=r'C:\MotorRock\calibration-probes\startup-wake-v130\TraceWakeV130.exe'
def guest(cmd):
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=180);return p.stdout+p.stderr
reference=[json.loads(s) for s in gzip.open('outputs/startup-import-v130/native-final/track64-rep0.jsonl.gz','rt')];files=[folder+r'\WorldProbeV130Cleanup.exe',folder+r'\PhysXCore.dll',observer,folder+r'\track64.words'];hashes={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files}
for rep in [0,1]:
 private=folder+f'\\wake64-{rep}-private.jsonl';physical=folder+f'\\wake64-{rep}-physical.jsonl';print('Track64 original energy capture',rep,'start',flush=True);log=guest('cd /d '+folder+'&& set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71=40&& '+observer+' '+private+' '+folder+r'\WorldProbeV130Cleanup.exe '+folder+r'\track64.words '+physical);(out/f'run{rep}.log').write_bytes(log)
 for kind,f in [('private',private),('physical',physical)]:
  data=guest('type '+f)
  if kind=='physical':assert [json.loads(s) for s in data.decode('utf-8-sig').splitlines()]==reference
  with gzip.open(out/f'run{rep}-{kind}.jsonl.gz','wb') as z:z.write(data)
 print('captured original 0..110 with all900 public states unchanged',flush=True)
for f,h in hashes.items():assert guest('certutil -hashfile '+f+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(hashes=hashes,original_public_states_preserved=900,repeats=2,goal_complete=False),indent=2)+'\n')
