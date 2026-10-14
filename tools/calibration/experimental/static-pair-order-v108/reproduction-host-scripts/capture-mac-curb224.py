from pathlib import Path
import json,os,gzip,subprocess,hashlib,struct
r=Path.cwd();o=r/'outputs/static-pair-order-v108/mac-curb224';o.mkdir(parents=True,exist_ok=False);m=json.loads((r/'work/frozen-v108/manifest.json').read_text());probe=r/'work/frozen-v108/rrr3d_physics_calibration';assert hashlib.sha256(probe.read_bytes()).hexdigest()==m['hashes']['rrr3d_physics_calibration'];golden=[]
for row in map(json.loads,open(r/'outputs/calibration-v14-source-windows.jsonl')):
 if row.get('event')=='calibration.sample' and row['detail']=='curb-side':golden.append(row['sample'])
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def physical(row):
 row=dict(row)
 for k in ['position','velocity','rotation','angular_momentum']:row[k]=list(map(bits,row[k]))
 f={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'};row['wheels']=[{k:bits(v) if k in f else v for k,v in wheel.items()} for wheel in row['wheels']];return row
captures=[]
for tick in [224]:
 runs=[]
 for rep in range(2):
  tag=f'curb-side-{tick}-{rep}';env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(m['environment'])
  for pathflag,stepflag,kind in [('RRR3D_NATIVE_TIRE_V55_TRACE','RRR3D_NATIVE_TIRE_TRACE_STEP_V86','tire'),('RRR3D_WHEEL_SOLVER_TRACE_FILE_V62','RRR3D_WHEEL_SOLVER_TRACE_STEP_V62','solver'),('RRR3D_WHEEL_PREP_TRACE_FILE_V67','RRR3D_WHEEL_PREP_TRACE_STEP_V67','prep')]:env[pathflag]=str(o/(tag+'-'+kind+'.jsonl'));env[stepflag]=str(tick+1)
  env.update(RRR3D_COMBINED_TIRE_TRACE_FILE_V95=str(o/(tag+'-combined.jsonl')),RRR3D_COMBINED_TIRE_TRACE_STEP_V95=str(tick))
  path=o/(tag+'-physical.jsonl');subprocess.run([str(probe),str(path),'curb-side'],env=env,capture_output=True,check=True,timeout=180);rows=[row['sample'] for row in map(json.loads,path.open()) if row.get('event')=='calibration.sample'];runs.append(list(map(physical,rows)));assert len(rows)==len(golden)==600;diff=[a['tick'] for a,b in zip(rows,golden) if physical(a)!=physical(b)];assert not diff
  captures.append({'physical_tick':tick,'sdk_counter':tick+1,'repeat':rep,'states':len(rows),'first_difference_Windows':diff[0] if diff else None});print(tag,'600 states all Native exact',flush=True)
 assert runs[0]==runs[1]
(o/'manifest.json').write_text(json.dumps({'scope':'Actual live frozenV108 complete curb600 Native-equivalent trajectories. At224 two independent private traces. No reference state fed to runtime.','probe_sha256':m['hashes']['rrr3d_physics_calibration'],'environment':m['environment'],'captures':captures,'repeats_exact':True,'goal_complete':False},indent=2)+'\n')
