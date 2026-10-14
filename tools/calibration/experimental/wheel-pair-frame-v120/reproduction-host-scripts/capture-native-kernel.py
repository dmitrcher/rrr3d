from pathlib import Path
import subprocess,json,hashlib,gzip
root=Path.cwd();out=root/'outputs/pair-pole-v120/native-kernel';out.mkdir(parents=True,exist_ok=False)
original=r'C:\MotorRock\calibration-probes\contact-pose-v45'
def guest(cmd):
 assert len(cmd)<3500
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=240)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout
observers={'kernel':(r'C:\MotorRock\calibration-probes\kernel-v84','TraceKernelV84.exe')}
paths=[original+r'\SpringProbe.exe',original+r'\PhysXCore.dll']+[p+'\\'+e for p,e in observers.values()]
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in paths}
expected=[]
with open(root/'outputs/calibration-v14-source-windows.jsonl') as f:
 for r in map(json.loads,f):
  if r.get('event')=='calibration.sample' and r['detail']=='pair-stock-pole-1':expected.append(r['sample'])
assert len(expected)==300
captures=[]
for kind,tick in [('kernel',31),('kernel',32)]:
 folder,exe=observers[kind]
 for rep in range(2):
  tag=f'{kind}-pair-stock-pole-1-{tick}-{rep}';prefix=folder+'\\v120-'+tag;private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl'
  print(tag,'started',flush=True)
  log=guest(f'set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71={tick}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+'\\'+exe+' '+private+' '+original+r'\SpringProbe.exe '+physical+' list:pair-stock-pole-1')
  (out/(tag+'.log')).write_bytes(log)
  trace=guest('type '+private)
  frames=[r for r in map(json.loads,trace.decode('utf-8-sig').splitlines()) if 'function' in r]
  assert frames and all(r.get('sample_tick_v71')==tick for r in frames)
  with gzip.open(out/(tag+'-private.jsonl.gz'),'wb') as f:f.write(trace)
  data=guest('type '+physical);actual=[r['sample'] for r in map(json.loads,data.decode('utf-8-sig').splitlines()) if r.get('event')=='calibration.sample'];assert actual==expected,'Original public history changed'
  with gzip.open(out/(tag+'-physical.jsonl.gz'),'wb') as f:f.write(data)
  captures.append({'kind':kind,'case':'pair-stock-pole-1','tick':tick,'repeat':rep,'original_history_exact':True,'states':len(actual),'trace_sha256':hashlib.sha256(trace).hexdigest()})
  (out/'manifest.json').write_text(json.dumps({'hashes':hashes,'captures':captures,'goal_complete':False},indent=2)+'\n')
  print(tag,'300 Original states unchanged',flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
print('All on-disk executable hashes unchanged',flush=True)
