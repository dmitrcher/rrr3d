import argparse,hashlib,json,os,pathlib,subprocess,sys
p=argparse.ArgumentParser();p.add_argument('--probe',type=pathlib.Path,required=True);p.add_argument('--environment',type=pathlib.Path,required=True);p.add_argument('--reference',type=pathlib.Path,required=True);p.add_argument('--baseline',type=pathlib.Path,required=True);p.add_argument('--profile',type=pathlib.Path,required=True);p.add_argument('--trace-step',type=int,default=0);p.add_argument('--cases');p.add_argument('--all',action='store_true');p.add_argument('--trace',action='store_true');p.add_argument('--out',type=pathlib.Path,required=True);a=p.parse_args()
root=pathlib.Path.cwd();a.out.mkdir(exist_ok=False)
probe=a.probe.resolve()
base=json.loads(a.environment.read_text())['environment'];base['RRR3D_CALIBRATION_PROFILE']=str(a.profile.resolve());base['RRR3D_EXPERIMENT_NATIVE_MATERIAL_FRAME']='1';base['RRR3D_EXPERIMENT_NATIVE_CONTACT_INERTIA']='1';base['RRR3D_EXPERIMENT_NATIVE_TRIANGLE_PLANE']='1';base['RRR3D_EXPERIMENT_NATIVE_SOLVER_NORMAL']='1';base['RRR3D_EXPERIMENT_NATIVE_POLYGON_DEPTH']='1';base['RRR3D_EXPERIMENT_NATIVE_CACHE_FRAME']='1';base['RRR3D_EXPERIMENT_NATIVE_SOLVER_INERTIA']='1';base.update({'RRR3D_EXPERIMENT_NATIVE_CACHE_POSE':'1','RRR3D_EXPERIMENT_NATIVE_GEOMETRY_POSE':'1','RRR3D_EXPERIMENT_NATIVE_FROZEN_CM_RESTORE':'1'})
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_EXPERIMENT','RRR3D_CONTACT')) and k not in ('NX_SPRING_ITERATIONS','RRR3D_CALIBRATION_PROFILE')};env.update(base)
names=([f'frame-v25-long-p{p}-x{x}-y{y}' for p in range(12) for x in range(3) for y in range(2)] if a.all else ['frame-v25-long-p7-x1-y0','frame-v25-long-p5-x0-y0','frame-v25-long-p5-x1-y1','frame-v25-long-p4-x1-y0'])
if a.cases:
 assert not a.all;names=a.cases.split(',');assert len(set(names))==len(names) and all(n in [f'frame-v25-long-p{p}-x{x}-y{y}' for p in range(12) for x in range(3) for y in range(2)] for n in names)
if a.trace:env.update({'RRR3D_CONTACT_ROWS':'1','RRR3D_CONTACT_NATIVE_TRACE_STEP':str(a.trace_step),'RRR3D_CONTACT_LOCAL_INERTIA_TRACE':'1','RRR3D_CONTACT_GEOMETRY_INPUT_TRACE':'1','RRR3D_CONTACT_CACHE_INPUT_TRACE':'1'})
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
reference=compare.load(a.reference,'windows-physx-d3d9')
baseline=compare.load(a.baseline,'mac-jolt-metal')
def comparable(case):
 result=dict(case);result['meta']=dict(case['meta']);profile=result['meta'].pop('profile');assert '\n' in profile;result['profile_contents']=profile.split('\n',1)[1];return result
manifest={'profile_sha256':hashlib.sha256(a.profile.read_bytes()).hexdigest(),'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'environment':{k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},'cases':names,'captures':{},'candidate':'native-quaternion-pose-v43','trace_step':a.trace_step,'status':'capturing'}
for mode in ('ordinary','control'):
 if mode=='ordinary':
  for suffix in ['INITIAL_COM','ACTOR_ORIGIN','ACTOR_OUTPUT','SHAPE_TRANSLATION','MASS_DOT_ORDER','FROZEN_CM_EXACT']:env['RRR3D_EXPERIMENT_NATIVE_'+suffix]='1'
 else:
  for suffix in ['INITIAL_COM','ACTOR_ORIGIN','ACTOR_OUTPUT','SHAPE_TRANSLATION','MASS_DOT_ORDER','FROZEN_CM_EXACT']:env.pop('RRR3D_EXPERIMENT_NATIVE_'+suffix,None)
 captures=[]
 for repeat in range(2):
  dest=a.out/f'calibration-depth-{mode}-run{repeat}-v39-mac.jsonl'
  with dest.open('wb') as output:
   for start in range(0,len(names),24):
    tmp=a.out/f'tmp-{mode}-{repeat}-{start}.jsonl'
    subprocess.run([str(probe),str(tmp.resolve()),'list:'+','.join(names[start:start+24])],env=env,check=True,timeout=90,stdout=subprocess.DEVNULL)
    output.write(tmp.read_bytes());tmp.unlink()
  cases=compare.load(dest,'mac-jolt-metal');assert set(cases)==set(names)
  captures.append(cases)
  if mode=='control':assert all(comparable(cases[name])==comparable(baseline[name]) for name in names),('Disabled candidate changed prior states',repeat)
  stats={name:{'first_difference':next((x['tick'] for x,y in zip(reference[name]['rows'],cases[name]['rows']) if x!=y),None),'max_position':max(compare.norm(x['position'],y['position']) for x,y in zip(reference[name]['rows'],cases[name]['rows'])),'max_velocity':max(compare.norm(x['velocity'],y['velocity']) for x,y in zip(reference[name]['rows'],cases[name]['rows']))} for name in names}
  manifest['captures'][mode+str(repeat)]={'sha256':hashlib.sha256(dest.read_bytes()).hexdigest(),'cases':stats}
  print(mode,repeat,'exact',sum(v['first_difference'] is None for v in stats.values()),'of',len(names),'position_max',max(v['max_position'] for v in stats.values()),'velocity_max',max(v['max_velocity'] for v in stats.values()),flush=True)
 assert captures[0]==captures[1],('Mac repeats differ',mode)
manifest['status']='measured';(a.out/'capture-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
