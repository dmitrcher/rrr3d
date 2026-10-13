from pathlib import Path
import json,gzip,subprocess,hashlib,collections
root=Path.cwd();out=root/'outputs/pair-material-v76/native-local-getter';out.mkdir(exist_ok=False);original=r'C:\MotorRock\calibration-probes\contact-pose-v45';folder=r'C:\MotorRock\calibration-probes\pair-material-v76';names=['pair-stock-oblique-0','pair-stock-weak-second-0']
def guest(cmd):
 assert len(cmd)<3500
 return subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=240,check=True).stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',folder+r'\TracePairMaterialV76.exe']};expected={n:[] for n in names}
for x in map(json.loads,open(root/'outputs/calibration-v14-source-windows.jsonl')):
 if x['event']=='calibration.sample' and x['detail'] in expected:expected[x['detail']].append(x['sample'])
records=[]
for name in names:
 for tick in ({names[0]:[39,40],names[1]:[38,39]}[name]):
  for rep in range(2):
   tag=f'v76-getter-{name}-tick{tick}-{rep}';private=folder+'\\'+tag+'-private.jsonl';physical=folder+'\\'+tag+'-physical.jsonl';guest('if exist '+physical+' del '+physical);guest('if exist '+private+' del '+private);print(tag,'start',flush=True)
   cmd=f'set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71={tick}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+r'\TracePairMaterialV76.exe '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name
   (out/(tag+'.log')).write_bytes(guest(cmd));data=guest('type '+private);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));entries=[x for x in rows if x.get('function')=='cache-validation-entry'];exits=[x for x in rows if x.get('function')=='cache-validation-exit'];assert len(entries)==len(exits)==1;assert all(x['sample_tick_v71']==tick for x in entries+exits)
   with gzip.open(out/(tag+'-private.jsonl.gz'),'wb') as f:f.write(data)
   data=guest('type '+physical);actual=[x['sample'] for x in map(json.loads,data.decode('utf-8-sig').splitlines()) if x['event']=='calibration.sample'];assert actual==expected[name]
   with gzip.open(out/(tag+'-physical.jsonl.gz'),'wb') as f:f.write(data)
   functions=dict(collections.Counter(x.get('function','event') for x in rows));records.append({'case':name,'repeat':rep,'tick':tick,'functions':functions,'all300_Golden_exact':True});(out/'manifest.json').write_text(json.dumps({'scope':'Read-only original paired-cache validation matching and old-order retention at same completed physical samples; all300Goldenstatesexact','hashes':hashes,'captures':records,'goal_complete':False},indent=2)+'\n');print(tag,'Golden exact, matched',functions.get('cache-point-matched',0),flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
print('Original hashes unchanged',flush=True)
