from pathlib import Path
import json,os,subprocess,gzip,struct,math,hashlib
root=Path.cwd();out=root/'outputs/hard-stop-error-v86/mac-hard-stop';out.mkdir(parents=True,exist_ok=False)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration')
base=json.loads((root/'work/frozen-v84/manifest.json').read_text())['environment'];assert b'RRR3D_EXPERIMENT_NATIVE_LONGITUDINAL_SLIP_V82' in probe.read_bytes()
names=['mobility/devildriver/engine3/lateral']
def load(p):
 d={n:[] for n in names};f=gzip.open(p,'rt') if str(p).endswith('.gz') else p.open()
 with f:
  for r in map(json.loads,f):
   if r.get('event')=='calibration.sample' and r['detail'] in d:d[r['detail']].append(r['sample'])
 assert all(len(r)>0 for r in d.values());return d
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def physical(r):
 r=dict(r)
 for k in ['position','velocity','rotation','angular_momentum']:r[k]=list(map(bits,r[k]))
 fields=['angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse']
 r['wheels']=[{k:bits(v) if k in fields else v for k,v in w.items()} for w in r['wheels']];return r
ref=load(root/'outputs/calibration-v14-source-windows.jsonl');saved=load(root/'outputs/normal-force-v84/broad/candidate-run0.jsonl')
entries=[];reports={}
for mode in ['force']:
 runs=[]
 for rep in range(2):
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base)
  p=out/f'{mode}-{rep}.jsonl'
  env['RRR3D_EXPERIMENT_NATIVE_TIRE_JV_V85']='1';env['RRR3D_EXPERIMENT_NATIVE_TIRE_STEP_V85']='1';env['RRR3D_EXPERIMENT_NATIVE_HARD_STOP_FRAME_V66']='1';env['RRR3D_EXPERIMENT_NATIVE_HARD_STOP_SOLVE_V68']='1';env['RRR3D_EXPERIMENT_NATIVE_HARD_STOP_ERROR_V86']='1'
  env['RRR3D_WHEEL_SOLVER_TRACE_FILE_V62']=str(out/f'{mode}-{rep}-solver.jsonl');env['RRR3D_WHEEL_SOLVER_TRACE_STEP_V62']='25'
  env['RRR3D_WHEEL_PREP_TRACE_FILE_V67']=str(out/f'{mode}-{rep}-prep.jsonl');env['RRR3D_WHEEL_PREP_TRACE_STEP_V67']='25'
  with (out/f'{mode}-{rep}.log').open('wb') as log:subprocess.run([str(probe),str(p),'list:'+','.join(names)],env=env,check=True,stdout=log,stderr=log,timeout=180)
  runs.append(load(p));entries.append({'mode':mode,'repeat':rep,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()});print(mode,rep,'complete',flush=True)
 assert runs[0]==runs[1]
 if mode=='control':assert runs[0]==saved,'Default changed V83 state'
 reports[mode]={}
 for n in names:
  a,b=ref[n],runs[0][n];dif=[x['tick'] for x,y in zip(a,b) if physical(x)!=physical(y)]
  reports[mode][n]={'first_difference':dif[0] if dif else None,'difference_ticks':dif,'position_max':max(math.dist(x['position'],y['position']) for x,y in zip(a,b)),'velocity_max':max(math.dist(x['velocity'],y['velocity']) for x,y in zip(a,b)),'angular_momentum_max':max(math.dist(x['angular_momentum'],y['angular_momentum']) for x,y in zip(a,b))}
  print(mode,n,{k:v for k,v in reports[mode][n].items() if k!='difference_ticks'},flush=True)
 (out/'report.json').write_text(json.dumps({'cases':reports,'two_repeats_exact':True,'control_matches_v83':True,'goal_complete':False},indent=2)+'\n')
(out/'manifest.json').write_text(json.dumps({'environment':base,'candidate_changes':{'RRR3D_EXPERIMENT_NATIVE_FORCE_DOMAIN_V84':'1'},'captures':entries,'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'goal_complete':False},indent=2)+'\n')
