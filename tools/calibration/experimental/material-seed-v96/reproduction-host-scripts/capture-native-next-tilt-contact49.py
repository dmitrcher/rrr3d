from pathlib import Path
import json,gzip,subprocess,hashlib
root=Path.cwd();out=root/'outputs/contact-mass-v96/native-next-tilt-contact49';out.mkdir(exist_ok=False)
original=r'C:\MotorRock\calibration-probes\contact-pose-v45';folder=r'C:\MotorRock\calibration-probes\frame-trace-v71';names=['body-tilt-roll']
def guest(cmd):
 assert len(cmd)<3500
 return subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=240,check=True).stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',folder+r'\TraceContactFrameV71.exe']};expected={n:[] for n in names}
for x in map(json.loads,open(root/'outputs/calibration-v14-source-windows.jsonl')):
 if x['event']=='calibration.sample' and x['detail'] in expected:expected[x['detail']].append(x['sample'])
records=[]
for name in names:
 for rep in range(2):
  tag=f'v96-tilt-contact49-{name}-{rep}';private=folder+'\\'+tag+'-private.jsonl';physical=folder+'\\'+tag+'-physical.jsonl'
  guest('if exist '+physical+' del '+physical);guest('if exist '+private+' del '+private)
  cmd='set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71=49&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+r'\TraceContactFrameV71.exe '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name
  print(tag,'start',flush=True);(out/(tag+'.log')).write_bytes(guest(cmd));data=guest('type '+private);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));entries=[x for x in rows if x.get('function')=='solver-iteration-entry'];assert entries and all(x['sample_tick_v71']==49 for x in entries)
  with gzip.open(out/(tag+'-private.jsonl.gz'),'wb') as f:f.write(data)
  data=guest('type '+physical);actual=[x['sample'] for x in map(json.loads,data.decode('utf-8-sig').splitlines()) if x['event']=='calibration.sample'];assert actual==expected[name]
  with gzip.open(out/(tag+'-physical.jsonl.gz'),'wb') as f:f.write(data)
  records.append({'case':name,'repeat':rep,'tick':49,'kernel_rows':len(entries),'all_Golden_states_exact':True,'states':len(actual)});(out/'manifest.json').write_text(json.dumps({'scope':'Original Windows body-tilt-roll physical49, immutable EXE/DLL, all180savedstates exact Golden.','hashes':hashes,'captures':records,'goal_complete':False},indent=2)+'\n');print(tag,'Golden exact',len(entries),'kernel rows',flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
print('Original hashes unchanged',flush=True)
