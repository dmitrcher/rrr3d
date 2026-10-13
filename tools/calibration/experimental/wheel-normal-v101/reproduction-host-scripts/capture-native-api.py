from pathlib import Path
import json,gzip,subprocess,struct
root=Path.cwd();out=root/'outputs/wheel-normal-v101/native-api';out.mkdir(exist_ok=False);folder=r'C:\MotorRock\calibration-probes\normal-v101';original=r'C:\MotorRock\calibration-probes\contact-pose-v45';names=['slope-longitudinal','slope-lateral','settle','lateral','border-front-grounded','curb-side','body-tilt-roll','pair-stock-pole-1']
def guest(cmd):return subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,check=True,timeout=180).stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',folder+r'\PhysXCore.dll',folder+r'\control-normals.exe']};assert hashes[original+r'\PhysXCore.dll'].splitlines()[1]==hashes[folder+r'\PhysXCore.dll'].splitlines()[1]
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
def physical(r):
 r=dict(r)
 for k in ['position','velocity','rotation','angular_momentum']:r[k]=list(map(bits,r[k]))
 fields={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'};r['wheels']=[{k:bits(v) if k in fields else v for k,v in w.items()} for w in r['wheels']];return r
expected={n:[] for n in names}
for r in map(json.loads,(root/'outputs/calibration-v14-source-windows.jsonl').open()):
 if r.get('event')=='calibration.sample' and r['detail'] in expected:expected[r['detail']].append(physical(r['sample']))
runs=[];norms=[]
for rep in range(2):
 path=folder+f'\\api-{rep}.jsonl';cmd='set NX_TRIANGLE_NORMAL_INPUT_TRACE_V101=1&& set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& '+folder+r'\control-normals.exe '+path+' list:'+','.join(names);(out/f'{rep}.log').write_bytes(guest(cmd));data=guest('type '+path);rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));actual={n:[] for n in names};n=[]
 for r in rows:
  if r.get('event')=='calibration.sample':actual[r['detail']].append(physical(r['sample']))
  elif r.get('event') in ['calibration.local-triangle-normal-v101','calibration.cooked-normal-array-v101','calibration.sdk-ray-normal-v101']:n.append({'case':r['detail'],'event':r['event'],'sample':r['sample']})
 assert actual==expected,('Independent control differs original Golden',rep);runs.append(actual);norms.append(n)
 with gzip.open(out/f'control-{rep}.jsonl.gz','wb') as stream:stream.write(data)
 print(rep,'Original Golden states exact',sum(map(len,actual.values())),'normal observations',len(n),flush=True)
assert runs[0]==runs[1] and norms[0]==norms[1]
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
(out/'report.json').write_text(json.dumps({'scope':'New independently compiled Windows control with read-only mesh API metadata; all selected original Golden states exact. SDK local triangle vertices/normalize, cooked normal array and direct shape ray normal.','hashes_before_after':hashes,'cases':names,'states_per_repeat':sum(map(len,expected.values())),'two_repeats_exact':True,'normal_observations':norms[0],'goal_complete':False},indent=2)+'\n')
