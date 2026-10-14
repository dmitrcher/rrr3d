from pathlib import Path
import subprocess,json,hashlib,gzip
root=Path.cwd();out=root/'outputs/wheel-length-v116/native';out.mkdir(parents=True,exist_ok=False)
original=r'C:\MotorRock\calibration-probes\contact-pose-v45';observer=r'C:\MotorRock\calibration-probes\length-v116'
def guest(cmd):
 assert len(cmd)<3500
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=300)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout
paths=[original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',observer+r'\TraceLengthV116.exe'];hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in paths}
names=['travel-long','radius-small'];expected={n:[] for n in names}
with open(root/'outputs/calibration-v14-source-windows.jsonl') as f:
 for r in map(json.loads,f):
  if r.get('event')=='calibration.sample' and r['detail'] in expected:expected[r['detail']].append(r['sample'])
assert all(len(v)==600 for v in expected.values())
captures=[]
for name in names:
 for rep in range(2):
  tag=f'{name}-{rep}';prefix=observer+'\\v116-'+tag;private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl';print(tag,'full600 getter trace started',flush=True)
  log=guest('set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71=&& set MOTORROCK_NATIVE_TRACE_STEP=&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+observer+r'\TraceLengthV116.exe '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name);(out/(tag+'.log')).write_bytes(log)
  trace=guest('type '+private);rows=[r for r in map(json.loads,trace.decode('utf-8-sig').splitlines()) if r.get('function')=='wheel-contact-getter-return'];assert rows and {r['sample_tick_v71'] for r in rows}>=set(range(600)),'Missing physical ticks'
  with gzip.open(out/(tag+'-private.jsonl.gz'),'wb') as f:f.write(trace)
  data=guest('type '+physical);actual=[r['sample'] for r in map(json.loads,data.decode('utf-8-sig').splitlines()) if r.get('event')=='calibration.sample'];assert actual==expected[name],'Original history changed'
  with gzip.open(out/(tag+'-physical.jsonl.gz'),'wb') as f:f.write(data)
  captures.append({'case':name,'repeat':rep,'original_history_exact':True,'states':len(actual),'contact_getter_returns':len(rows),'trace_sha256':hashlib.sha256(trace).hexdigest()});(out/'manifest.json').write_text(json.dumps({'hashes':hashes,'captures':captures,'goal_complete':False},indent=2)+'\n');print(tag,'Original600 history preserved',flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
print('Original executable, DLL and observer on-disk hashes unchanged',flush=True)
