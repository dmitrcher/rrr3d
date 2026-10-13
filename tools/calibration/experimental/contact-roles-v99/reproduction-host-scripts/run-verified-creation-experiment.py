from pathlib import Path
import json,gzip,subprocess,hashlib,struct
root=Path.cwd();w=root/'work/creation-order-v99';o=root/'outputs/creation-order-v99-verified';o.mkdir(exist_ok=False)
folder=r'C:\MotorRock\calibration-probes\creation-order-v99';original=r'C:\MotorRock\calibration-probes\contact-pose-v45';names=['border-front-grounded','border-grounded','border-front-air-straight','body-tilt-roll']
def guest(cmd):
 assert len(cmd)<3500
 return subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],check=True,capture_output=True,timeout=240).stdout
hashes={p:guest('certutil -hashfile '+p+' SHA256').decode(errors='replace') for p in [original+r'\SpringProbe.exe',original+r'\PhysXCore.dll',folder+r'\PhysXCore.dll',folder+r'\control-inventory.exe',folder+r'\before-car-only.exe']};assert hashes[original+r'\PhysXCore.dll'].splitlines()[1].strip()==hashes[folder+r'\PhysXCore.dll'].splitlines()[1].strip()
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
def physical(s):
 s=dict(s)
 for k in ['position','velocity','rotation','angular_momentum']:s[k]=list(map(bits,s[k]))
 floats={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'};s['wheels']=[{k:bits(v) if k in floats else v for k,v in x.items()} for x in s['wheels']];return s
expected={n:[] for n in names};mac={n:[] for n in names}
for target,p in [(expected,root/'outputs/calibration-v14-source-windows.jsonl'),(mac,root/'outputs/solver-compose-v98/broad/candidate-run0.jsonl')]:
 for r in map(json.loads,open(p)):
  if r.get('event')=='calibration.sample' and r['detail'] in target:target[r['detail']].append(physical(r['sample']))
reports={};allruns={};inventories={}
for mode in ['control-inventory','before-car-only']:
 runs=[]
 for rep in range(2):
  p=folder+f'\\{mode}-{rep}.jsonl';cmd='set NX_SPRING_ITERATIONS=4&& set NX_WHEEL_AXES=1&& set NX_ANGULAR_MOMENTUM_TRACE=1&& set NX_CONTACT_QUAT_TRACE=1&& set NX_LOCAL_POINT_TRACE=1&& '+folder+'\\'+mode+'.exe '+p+' list:'+','.join(names);print(mode,rep,'start',flush=True);(o/f'{mode}-{rep}.log').write_bytes(guest(cmd));data=guest('type '+p);assert data.strip().startswith(b'{'),data[:500]
  with gzip.open(o/f'{mode}-{rep}.jsonl.gz','wb') as z:z.write(data)
  rows=list(map(json.loads,data.decode('utf-8-sig').splitlines()));states={n:[] for n in names};inv={x['detail']:x['sample'] for x in rows if x.get('event')=='calibration.scene-inventory'};assert len(inv)==4;inventories[mode]=inv
  for r in rows:
   if r.get('event')=='calibration.sample' and r['detail'] in states:states[r['detail']].append(physical(r['sample']))
  assert all(len(states[n])==len(expected[n]) for n in names),{n:len(states[n]) for n in names};runs.append(states);print(mode,rep,'captured',sum(map(len,states.values())),flush=True)
  if mode=='control-inventory':assert states==expected,('Control build differs from immutable Golden',[(n,next((i for i,(a,b) in enumerate(zip(states[n],expected[n])) if a!=b),None)) for n in names])
 assert runs[0]==runs[1];allruns[mode]=runs[0];reports[mode]={}
 for name in names:
  dif=[i for i,(a,b) in enumerate(zip(runs[0][name],expected[name])) if a!=b];dmac=[i for i,(a,b) in enumerate(zip(runs[0][name],mac[name])) if a!=b];reports[mode][name]={'first_difference_original':dif[0] if dif else None,'difference_ticks_original':dif,'first_difference_MacV98':dmac[0] if dmac else None,'difference_ticks_MacV98':dmac}
for n in names:
 assert inventories['control-inventory'][n]['actors']==inventories['before-car-only'][n]['actors'];assert sorted((x['dynamic'],x['shapes'],x['id']) for x in inventories['control-inventory'][n]['items'])==sorted((x['dynamic'],x['shapes'],x['id']) for x in inventories['before-car-only'][n]['items'])
for p,h in hashes.items():assert guest('certutil -hashfile '+p+' SHA256').decode(errors='replace')==h
report={'scope':'Separate Windows control and only-obstacle-created-before-car source variant; original reference immutable. Fresh1980states four fixed-input cases x2repeats x2modes.','scene_actor_shape_population_exact':True,'scene_inventories':inventories,'control_all_original_Golden_states_exact':True,'each_mode_repeats_exact':True,'hashes_before_after_exact':hashes,'results':reports,'goal_complete':False};(o/'report.json').write_text(json.dumps(report,indent=2)+'\n');print('control exact Golden; both variants repeated and hashes unchanged',flush=True)
