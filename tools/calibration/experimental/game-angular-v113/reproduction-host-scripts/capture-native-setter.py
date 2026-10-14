from pathlib import Path
import subprocess,json,hashlib,gzip
root=Path.cwd();out=root/'outputs/game-angular-v113/native-setter-v3';out.mkdir(parents=True,exist_ok=False);original=r'C:\MotorRock\calibration-probes\contact-pose-v45';folder=r'C:\MotorRock\calibration-probes\game-angular-v113';exe='TraceGameAngularV113.exe'
def guest(cmd,timeout=240):
 assert len(cmd)<3500
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=timeout)
 if r.returncode:raise RuntimeError(r.stdout.decode(errors='replace')+r.stderr.decode(errors='replace'))
 return r.stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',folder+'\\'+exe]}
selections=[('game-clamp-roll',0),('game-clamp-pitch',0),('game-clamp-mixed',0),('game-angular-damping',1),('game-air-pitch',4)];expected={n:[] for n,t in selections}
with (root/'outputs/calibration-v14-source-windows.jsonl').open() as f:
 for r in map(json.loads,f):
  if r['event']=='calibration.sample' and r['detail'] in expected:expected[r['detail']].append(r['sample'])
assert all(len(s)==120 for s in expected.values());captures=[]
for index,(name,tick) in enumerate(selections):
 for rep in range(2):
  tag=f'case{index}-tick{tick}-rep{rep}';prefix=folder+'\\v113-'+tag;private=prefix+'-private.jsonl';physical=prefix+'-physical.jsonl';guest('if exist '+physical+' del '+physical);guest('if exist '+private+' del '+private);print(name,tick,rep,'start',flush=True)
  log=guest(f'set MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71={tick}&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+'\\'+exe+' '+private+' '+original+r'\SpringProbe.exe '+physical+' list:'+name);(out/(tag+'.log')).write_bytes(log)
  trace=guest('type '+private);rows=[r for r in map(json.loads,trace.decode('utf-8-sig').splitlines()) if 'function' in r];assert rows and all(r.get('sample_tick_v71')==tick for r in rows)
  with gzip.open(out/(tag+'-private.jsonl.gz'),'wb') as g:g.write(trace)
  data=guest('type '+physical);actual=[r['sample'] for r in map(json.loads,data.decode('utf-8-sig').splitlines()) if r['event']=='calibration.sample'];assert actual==expected[name],(name,tick,rep,'Golden changed')
  with gzip.open(out/(tag+'-physical.jsonl.gz'),'wb') as g:g.write(data)
  captures.append({'name':name,'physical_tick':tick,'repeat':rep,'tag':tag,'golden_full_states_exact':True,'private_logical_sha256':hashlib.sha256(trace).hexdigest()});(out/'manifest.json').write_text(json.dumps({'scope':'Read-only actual Native motor arithmetic. Authoritative current tick from completed sample lines. Immutable Original EXE/DLL and all120physicalstates unchanged.','hashes':hashes,'captures':captures,'goal_complete':False},indent=2)+'\n');print(tag,'Golden exact',flush=True)
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
print('All Native original/observer hashes unchanged',flush=True)
