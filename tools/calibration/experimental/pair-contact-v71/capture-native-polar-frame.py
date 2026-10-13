from pathlib import Path
import subprocess,json,hashlib,gzip
root=Path.cwd();out=root/'outputs/pair-contact-v71/native-polar-frame';out.mkdir(exist_ok=False)
original=r'C:\MotorRock\calibration-probes\contact-pose-v45';contact=r'C:\MotorRock\calibration-probes\frame-trace-v71';wheel=contact
def guest(cmd,timeout=240):
 assert len(cmd)<3500
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=timeout)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',contact+r'\TraceContactFrameV71.exe',wheel+r'\TraceWheelFrameV71.exe']}
names=['pair-pole-oblique-0','pair-vertical-weak-peer-0','pair-weak-second-0'];expected={n:[] for n in names}
for r in map(json.loads,open('outputs/calibration-v14-source-windows.jsonl')):
 if r['event']=='calibration.sample' and r['detail'] in expected:expected[r['detail']].append(r['sample'])
captures=[]
for kind,folder,exe,name,counter in [('contact',contact,'TraceContactFrameV71.exe',names[0],37),('contact',contact,'TraceContactFrameV71.exe',names[1],38),('contact',contact,'TraceContactFrameV71.exe',names[2],38)]:
 for rep in range(2):
  print(kind,name,rep,'started',flush=True);prefix=folder+f'\\v71-frame-{name}-{rep}';private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl'
  log=guest(f'set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71={counter}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+'\\'+exe+' '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name)
  tag=f'{kind}-{name}-{rep}';(out/(tag+'.log')).write_bytes(log)
  trace=guest('type '+private)
  frame_rows=[r for r in map(json.loads,trace.decode('utf-8-sig').splitlines()) if 'function' in r]
  assert frame_rows and all(r.get('sample_tick_v71')==counter for r in frame_rows),(name,rep,'wrong frame or empty trace')
  with gzip.open(out/(tag+'-private.jsonl.gz'),'wb') as stream:stream.write(trace)
  data=guest('type '+physical);actual=[r['sample'] for r in map(json.loads,data.decode('utf-8-sig').splitlines()) if r['event']=='calibration.sample'];assert actual==expected[name],(name,rep,'Golden changed')
  with gzip.open(out/(tag+'-physical.jsonl.gz'),'wb') as stream:stream.write(data)
  captures.append({'kind':kind,'name':name,'repeat':rep,'sample_tick':counter,'physical_golden_exact':True,'private_logical_sha256':hashlib.sha256(trace).hexdigest()});print(tag,'300 samples Golden exact',flush=True)
  (out/'manifest.json').write_text(json.dumps({'scope':'Original Native read-only debugger traces. Source AFTER_SOLVE sample count provides an authoritative current physical tick. No calculation from the integrator counter. Original exe and DLL unchanged; same fixed physics inputs and all300samples Golden exact.','hashes':hashes,'captures':captures,'goal_complete':False},indent=2)+'\n')
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
print('Native hashes unchanged',flush=True)
