from pathlib import Path
import subprocess,json,hashlib
root=Path.cwd();out=root/'outputs/native-third-border-v68';out.mkdir(exist_ok=False)
original=r'C:\MotorRock\calibration-probes\contact-pose-v45';contact=r'C:\MotorRock\calibration-probes\border-contact-v59';wheel=r'C:\MotorRock\calibration-probes\wheel-row-v56'
def guest(cmd,timeout=240):
 assert len(cmd)<3500
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=timeout)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout
hashes={}
for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',contact+r'\TraceContact.exe',wheel+r'\TraceWheel.exe']:
 hashes[p]=guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')
expected=[r['sample'] for r in map(json.loads,open('outputs/calibration-v14-source-windows.jsonl')) if r['event']=='calibration.sample' and r['detail']=='border-grounded']
assert len(expected)==600
captures=[]
for kind,folder,exe in [('contact',contact,'TraceContact.exe'),('wheel',wheel,'TraceWheel.exe')]:
 traces=[]
 for rep in range(2):
  print(kind,rep,'started',flush=True);prefix=folder+f'\\third-v68-{rep}';private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl'
  log=guest(f'set MOTORROCK_NATIVE_TRACE_STEP=63&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+'\\'+exe+' '+private+' '+original+r'\SpringProbe.exe '+physical+' list:border-grounded')
  (out/f'{kind}-{rep}.log').write_bytes(log)
  for key,path in [('private',private),('physical',physical)]:(out/f'{kind}-{rep}-{key}.jsonl').write_bytes(guest('type '+path))
  actual=[r['sample'] for r in map(json.loads,(out/f'{kind}-{rep}-physical.jsonl').open()) if r['event']=='calibration.sample'];assert actual==expected,(kind,rep,'Golden changed')
  # Private pointers and OS addresses are intentionally not repeat compared.
  captures.append({'kind':kind,'repeat':rep,'integration_counter':63,'physical_golden_exact':True,'private_sha256':hashlib.sha256((out/f'{kind}-{rep}-private.jsonl').read_bytes()).hexdigest()});print(kind,rep,'600 samples Golden exact',flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
(out/'manifest.json').write_text(json.dumps({'scope':'Read-only original Windows SDK wheel/contact traces at third border collision; executable and DLL hashes before/after unchanged.','hashes':hashes,'captures':captures,'goal_complete':False},indent=2)+'\n')
