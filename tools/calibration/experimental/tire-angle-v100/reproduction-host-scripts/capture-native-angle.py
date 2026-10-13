from pathlib import Path
import gzip,json,subprocess,hashlib
root=Path.cwd();out=root/'outputs/tire-angle-v100/native-angle';out.mkdir(exist_ok=False)
original=r'C:\MotorRock\calibration-probes\contact-pose-v45';folder=r'C:\MotorRock\calibration-probes\angle-v100';exe='TraceAngleV100.exe'
selections=[('border-front-grounded',149),('border-front-grounded',148),('border-front-grounded',150),('slope-longitudinal',19),('slope-lateral',19),('border-grounded',61),('curb-side',13),('lateral',253)]
def guest(cmd):
 assert len(cmd)<3500
 return subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=240).stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',folder+'\\'+exe]}
names={n for n,t in selections};expected={n:[] for n in names}
for x in map(json.loads,(root/'outputs/calibration-v14-source-windows.jsonl').open()):
 if x.get('event')=='calibration.sample' and x['detail'] in expected:expected[x['detail']].append(x['sample'])
captures=[]
for name,tick in selections:
 for rep in range(2):
  tag=f'{name}-{tick}-{rep}';prefix=folder+'\\v100-'+tag;private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl';print(tag,'start',flush=True)
  cmd=f'set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71={tick}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+'\\'+exe+' '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name
  (out/f'{tag}.log').write_bytes(guest(cmd));data=guest('type '+private);r=list(map(json.loads,data.decode('utf-8-sig').splitlines()));calls=[x for x in r if x.get('function')=='tire-angle-argument-ready'];stored=[x for x in r if x.get('function')=='tire-angle-stored'];assert len(calls)==len(stored)>0 and all(x['sample_tick_v71']==tick for x in calls+stored)
  with gzip.open(out/f'{tag}-private.jsonl.gz','wb') as f:f.write(data)
  data=guest('type '+physical);actual=[x['sample'] for x in map(json.loads,data.decode('utf-8-sig').splitlines()) if x.get('event')=='calibration.sample'];assert actual==expected[name],('Original Windows changed',tag)
  with gzip.open(out/f'{tag}-physical.jsonl.gz','wb') as f:f.write(data)
  captures.append({'case':name,'physical_tick':tick,'repeat':rep,'actual_asin_calls':len(calls),'original_Golden_states_exact':True,'states':len(actual),'physical_logical_sha256':hashlib.sha256(data).hexdigest()});(out/'manifest.json').write_text(json.dumps({'scope':'Original Windows read-only angle argument RVA14b4a0 and stored result RVA14b4ad, authoritative physical tick from after-solve sample count. Original EXE/DLL immutable.','hashes':hashes,'captures':captures,'goal_complete':False},indent=2)+'\n');print(tag,'Golden states exact',len(actual),'calls',len(calls),flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
print('Original EXE/DLL and new observer hashes unchanged',flush=True)
