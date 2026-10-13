from pathlib import Path
import subprocess,json,gzip,hashlib
root=Path.cwd();out=root/'outputs/box-face-v97/native-polygons-upper';out.mkdir(parents=True,exist_ok=False)
original=r'C:\MotorRock\calibration-probes\contact-pose-v45';folder=r'C:\MotorRock\calibration-probes\polygon-v97'
def guest(cmd):
 assert len(cmd)<3500
 return subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=240).stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',folder+r'\TracePolygonV97.exe']};expected={n:[] for n in ['body-tilt-roll','body-tilt-pitch']}
for r in map(json.loads,(root/'outputs/calibration-v14-source-windows.jsonl').open()):
 if r.get('event')=='calibration.sample' and r['detail'] in expected:expected[r['detail']].append(r['sample'])
entries=[]
for name,tick in [('body-tilt-roll',145)]:
 for rep in range(2):
  tag=f'v97-{name}-{tick}-{rep}';private=folder+'\\'+tag+'-private.jsonl';physical=folder+'\\'+tag+'-physical.jsonl'
  cmd=f'set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71={tick}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+r'\TracePolygonV97.exe '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name
  print(tag,'start',flush=True);(out/(tag+'.log')).write_bytes(guest(cmd));data=guest('type '+private);records=list(map(json.loads,data.decode('utf-8-sig').splitlines()));polys=[r for r in records if r.get('function')=='polygon-input-entry'];assert polys and all(r['sample_tick_v71']==tick for r in polys)
  with gzip.open(out/(tag+'-private.jsonl.gz'),'wb') as f:f.write(data)
  data=guest('type '+physical);actual=[r['sample'] for r in map(json.loads,data.decode('utf-8-sig').splitlines()) if r.get('event')=='calibration.sample'];assert actual==expected[name]
  with gzip.open(out/(tag+'-physical.jsonl.gz'),'wb') as f:f.write(data)
  entries.append({'case':name,'physical_tick':tick,'repeat':rep,'polygons':len(polys),'states':len(actual),'all_golden_exact':True});print(tag,'Golden exact',len(actual),'polygoncalls',len(polys),flush=True)
  (out/'manifest.json').write_text(json.dumps({'scope':'Original Windows polygon inputs, read-only observer, all retained full histories Golden exact','hashes_before':hashes,'captures':entries,'goal_complete':False},indent=2)+'\n')
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
m=json.loads((out/'manifest.json').read_text());m['hashes_after_equal']=True;(out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');print('Original EXE/DLL and observer hashes unchanged',flush=True)
