from pathlib import Path
import json,gzip,subprocess,hashlib
root=Path.cwd();out=root/'outputs/pair-contact-v72/native-box-refined';out.mkdir(exist_ok=True)
original=r'C:\MotorRock\calibration-probes\contact-pose-v45';folder=r'C:\MotorRock\calibration-probes\box-frame-v72'
def guest(cmd):
 assert len(cmd)<3500
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=240,check=True);return r.stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',folder+r'\TraceBoxFrameV72.exe']}
names=['pair-same-head-0','pair-stock-head-0','pair-pole-oblique-0','pair-pole-0','pair-weak-second-0','pair-tilted-first-0','pair-tilted-second-0'];expected={n:[] for n in names}
for x in map(json.loads,open(root/'outputs/calibration-v14-source-windows.jsonl')):
 if x['event']=='calibration.sample' and x['detail'] in expected:expected[x['detail']].append(x['sample'])
records=json.load(open(out/'manifest.json'))['captures'] if (out/'manifest.json').exists() else []
for name,tick in [(names[0],38),(names[1],38),(names[2],37),(names[2],36),(names[3],37),(names[4],38),(names[5],38),(names[6],38)]:
 for rep in range(2):
  if any(x['case']==name and x['tick']==tick and x['repeat']==rep for x in records):continue
  tag=f'refined-{name}-tick{tick}-{rep}';private=folder+'\\'+tag+'-private.jsonl';physical=folder+'\\'+tag+'-physical.jsonl';print(tag,'start',flush=True)
  guest('if exist '+physical+' del '+physical)
  guest('if exist '+private+' del '+private)
  cmd=f'set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71={tick}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+r'\TraceBoxFrameV72.exe '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name
  (out/(tag+'.log')).write_bytes(guest(cmd));p=guest('type '+private);r=list(map(json.loads,p.decode('utf-8-sig').splitlines()));box=[x for x in r if x.get('function','').startswith('box-')]
  assert all(x['sample_tick_v71']==tick for x in box)
  assert box or (name=='pair-pole-oblique-0' and tick==36)
  with gzip.open(out/(tag+'-private.jsonl.gz'),'wb') as f:f.write(p)
  data=guest('type '+physical);actual=[x['sample'] for x in map(json.loads,data.decode('utf-8-sig').splitlines()) if x['event']=='calibration.sample'];assert actual==expected[name]
  with gzip.open(out/(tag+'-physical.jsonl.gz'),'wb') as f:f.write(data)
  records.append({'case':name,'tick':tick,'repeat':rep,'golden_physics_exact':True,'private_logical_sha256':hashlib.sha256(p).hexdigest(),'functions':{kind:sum(x['function']==kind for x in box) for kind in set(x['function'] for x in box)}})
  (out/'manifest.json').write_text(json.dumps({'scope':'Read-only original Native box SAT inputs/overlaps/cache selection, synchronized by completed physical samples. All300physicalstates preserve independent original Golden.','hashes':hashes,'captures':records,'goal_complete':False},indent=2)+'\n');print(tag,'Golden exact',flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
print('Alloriginalhashesunchanged',flush=True)
