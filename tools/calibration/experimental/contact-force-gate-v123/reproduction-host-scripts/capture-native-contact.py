from pathlib import Path
import json,gzip,subprocess,hashlib
r=Path.cwd();o=r/'outputs/tiny-contact-v123/native-contact';o.mkdir(parents=True,exist_ok=False);original=r'C:\MotorRock\calibration-probes\contact-pose-v45';folder=r'C:\MotorRock\calibration-probes\frame-trace-v71';selections=[('skin-tilted-triangle-angle3-skin2-thin1',13),('skin-tilted-triangle-angle3-skin2-thin1',14)]
def guest(cmd):return subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=240).stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',folder+r'\TraceContactFrameV71.exe']};reference={n:[] for n,t in selections}
for row in map(json.loads,open(r/'outputs/calibration-v14-source-windows.jsonl')):
 if row.get('event')=='calibration.sample' and row['detail'] in reference:reference[row['detail']].append(row['sample'])
captures=[]
for name,tick in selections:
 for rep in range(2):
  tag=f'v123-{name}-{tick}-{rep}';private=folder+'\\'+tag+'-private.jsonl';physical=folder+'\\'+tag+'-physical.jsonl';guest('if exist '+physical+' del '+physical);guest('if exist '+private+' del '+private);print(tag,'start',flush=True);log=guest(f'set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71={tick}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+r'\TraceContactFrameV71.exe '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name);(o/(tag+'.log')).write_bytes(log);data=guest('type '+private);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));assert all(row['sample_tick_v71']==tick for row in rows if row.get('function'));assert any(row.get('function')=='material-inputs-ready' for row in rows)
  with gzip.open(o/(tag+'-private.jsonl.gz'),'wb') as dst:dst.write(data)
  data=guest('type '+physical);actual=[row['sample'] for row in map(json.loads,data.decode('utf-8-sig').splitlines()) if row.get('event')=='calibration.sample'];assert actual==reference[name]
  with gzip.open(o/(tag+'-physical.jsonl.gz'),'wb') as dst:dst.write(data)
  captures.append({'case':name,'physical_tick':tick,'repeat':rep,'states':len(actual),'all_original_Golden_states_exact':True});(o/'manifest.json').write_text(json.dumps({'hashes':hashes,'captures':captures,'goal_complete':False},indent=2)+'\n');print(tag,'Golden exact',flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
