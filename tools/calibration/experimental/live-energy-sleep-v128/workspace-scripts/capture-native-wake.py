from pathlib import Path
import subprocess,json,gzip,hashlib
r=Path.cwd();out=r/'outputs/live-sleep-v128/native-wake720';out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\world-v124';observer=r'C:\MotorRock\calibration-probes\wake-sleep-v128\TraceWorldV124.exe'
def guest(cmd):
 p=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=240)
 if p.returncode:raise RuntimeError(p.stdout.decode(errors='replace')+p.stderr.decode(errors='replace'))
 return p.stdout
reference=[json.loads(s) for s in gzip.open(r/'outputs/local-mesh-v126/native-sleep-state/track0-rep0.jsonl.gz','rt')];paths=[folder+r'\WorldProbeV124.exe',folder+r'\PhysXCore.dll',folder+r'\PhysXCooking.dll',observer,folder+r'\track0.words'];hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in paths}
for rep in [0,1]:
 private=folder+f'\\wake720-{rep}-private.jsonl';physical=folder+f'\\wake720-{rep}-physical.jsonl';print('Native wake720',rep,'start',flush=True);log=guest('cd /d '+folder+'&& set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71=720&& '+observer+' '+private+' '+folder+r'\WorldProbeV124.exe '+folder+r'\track0.words '+physical);(out/f'run{rep}.log').write_bytes(log)
 for kind,p in [('private',private),('physical',physical)]:
  data=guest('type '+p)
  if kind=='physical':assert [json.loads(s) for s in data.decode('utf-8-sig').splitlines()]==reference
  else:
   rows=[json.loads(s) for s in data.decode('utf-8-sig').splitlines()];assert any(x.get('function')=='wheel-interpolation-input' for x in rows);assert all(x['sample_tick_v71']==720 for x in rows if x.get('function'))
  with gzip.open(out/f'run{rep}-{kind}.jsonl.gz','wb') as f:f.write(data)
 print('Native wake720',rep,'all900states preserved',flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
(out/'manifest.json').write_text(json.dumps({'scope':'ActualNative firstrealmapcontactstage720 read-only observer;900publicstatesunchanged,exe/dll/fixtureSHAunchanged.','hashes':hashes,'repeats':2,'goal_complete':False},indent=2)+'\n')
