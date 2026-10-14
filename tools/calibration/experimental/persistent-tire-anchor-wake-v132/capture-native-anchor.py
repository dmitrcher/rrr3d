from pathlib import Path
import subprocess,json,gzip
out=Path('outputs/post-impulse-v132/native-anchor-fixed');out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\startup-import-v130';observer=r'C:\MotorRock\calibration-probes\post-anchor-v132\TraceAnchorV132.exe'
def guest(cmd):
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=180);return p.stdout+p.stderr
files=[folder+r'\WorldProbeV130Cleanup.exe',folder+r'\PhysXCore.dll',observer,folder+r'\track48.words',folder+r'\track16.words'];hashes={f:guest('certutil -hashfile '+f+' SHA256').decode() for f in files}
for track,tick in [(48,781),(16,787)]:
 reference=list(map(json.loads,gzip.open(f'outputs/startup-import-v130/native-final/track{track}-rep0.jsonl.gz','rt')))
 for rep in [0,1]:
  private=folder+f'\\trace{track}-{rep}-private.jsonl';physical=folder+f'\\trace{track}-{rep}-physical.jsonl';print('Native map',track,'step',tick,'rep',rep,'start',flush=True)
  log=guest('cd /d '+folder+'&& set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71='+str(tick)+'&& '+observer+' '+private+' '+folder+r'\WorldProbeV130Cleanup.exe '+folder+f'\\track{track}.words '+physical);(out/f'track{track}-run{rep}.log').write_bytes(log)
  for kind,f in [('private',private),('physical',physical)]:
   data=guest('type '+f)
   if kind=='physical':assert list(map(json.loads,data.decode('utf-8-sig').splitlines()))==reference
   else:
    rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));assert any(x.get('function')=='tire-anchor-gate' for x in rows)
   with gzip.open(out/f'track{track}-run{rep}-{kind}.jsonl.gz','wb') as z:z.write(data)
  print('Native readonly capture complete900states unchanged',track,rep,flush=True)
for f,h in hashes.items():assert guest('certutil -hashfile '+f+' SHA256').decode()==h
(out/'manifest.json').write_text(json.dumps(dict(hashes=hashes,original_public_states_preserved=900,tracks=[48,16],repeats=2,goal_complete=False),indent=2)+'\n')
