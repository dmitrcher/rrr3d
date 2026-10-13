from pathlib import Path
import subprocess,json,hashlib,gzip
root=Path.cwd();out=root/'outputs/pair-contact-v70/native-corrected';out.mkdir(exist_ok=False)
original=r'C:\MotorRock\calibration-probes\contact-pose-v45';contact=r'C:\MotorRock\calibration-probes\border-contact-v59';wheel=r'C:\MotorRock\calibration-probes\wheel-row-v56'
def guest(cmd,timeout=240):
 assert len(cmd)<3500
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=timeout)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',contact+r'\TraceContact.exe',wheel+r'\TraceWheel.exe']}
names=['pair-stock-pole-1','pair-same-head-0','pair-stock-head-0'];expected={n:[] for n in names}
for r in map(json.loads,open('outputs/calibration-v14-source-windows.jsonl')):
 if r['event']=='calibration.sample' and r['detail'] in expected:expected[r['detail']].append(r['sample'])
captures=[]
for kind,folder,exe,name,counter in [('wheel',wheel,'TraceWheel.exe',names[0],55),('contact',contact,'TraceContact.exe',names[1],76),('contact',contact,'TraceContact.exe',names[2],76)]:
 for rep in range(2):
  print(kind,name,rep,'started',flush=True);prefix=folder+f'\\v70-corrected-{name}-{rep}';private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl'
  log=guest(f'set MOTORROCK_NATIVE_TRACE_STEP={counter}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+'\\'+exe+' '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name)
  tag=f'{kind}-{name}-{rep}';(out/(tag+'.log')).write_bytes(log)
  trace=guest('type '+private)
  with gzip.open(out/(tag+'-private.jsonl.gz'),'wb') as stream:stream.write(trace)
  data=guest('type '+physical);actual=[r['sample'] for r in map(json.loads,data.decode('utf-8-sig').splitlines()) if r['event']=='calibration.sample'];assert actual==expected[name],(name,rep,'Golden changed')
  with gzip.open(out/(tag+'-physical.jsonl.gz'),'wb') as stream:stream.write(data)
  captures.append({'kind':kind,'name':name,'repeat':rep,'counter':counter,'physical_golden_exact':True,'private_logical_sha256':hashlib.sha256(trace).hexdigest()});print(tag,'300 samples Golden exact',flush=True)
  (out/'manifest.json').write_text(json.dumps({'scope':'Original Native read-only debugger traces. Two dynamic actors: integrator ticks count bodies, not frames. Wheel counter55 corresponds resulting tick28; contact counter75 corresponds tick38 (target76 retains75/76).','hashes':hashes,'captures':captures,'goal_complete':False},indent=2)+'\n')
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
print('Native hashes unchanged',flush=True)
