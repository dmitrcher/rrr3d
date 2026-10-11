from pathlib import Path
import os,subprocess,json,hashlib,sys,argparse
p=argparse.ArgumentParser();p.add_argument('--probe',type=Path,required=True);p.add_argument('--environment',type=Path,required=True);p.add_argument('--profile',type=Path,required=True);p.add_argument('--reference',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--flags',default='');p.add_argument('--trace-step',type=int,default=0);p.add_argument('--cases');a=p.parse_args();probe=a.probe;out=a.out;out.mkdir(parents=True,exist_ok=False)
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
base=json.loads(a.environment.read_text())['environment'];base['RRR3D_CALIBRATION_PROFILE']=str(a.profile.resolve());base['RRR3D_CONTACT_EDGE_STAGE_TRACE']='1';base['RRR3D_CONTACT_MATERIAL_STAGE_TRACE']='1';base['RRR3D_CONTACT_LOCAL_INERTIA_TRACE']='1';base.update({'RRR3D_EXPERIMENT_NATIVE_GEOMETRY_POSE':'1','RRR3D_EXPERIMENT_NATIVE_CACHE_POSE':'1','RRR3D_CONTACT_CM_RESTORE_TRACE':'1','RRR3D_CONTACT_ROWS':'1','RRR3D_CONTACT_NATIVE_TRACE_STEP':str(a.trace_step),'RRR3D_CONTACT_CACHE_INPUT_TRACE':'1','RRR3D_CONTACT_GEOMETRY_INPUT_TRACE':'1'});native=compare.load(a.reference,'windows-physx-d3d9');assert set(native)=={f'pose-v45-inverted-{i}' for i in range(6)}
if a.cases:native={n:native[n] for n in a.cases.split(',')}
report={'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'scope':'Six no-AI frozen contacts varying shape position and center of mass','captures':{}}
for mode in ['ordinary','control']:
 env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_EXPERIMENT','RRR3D_CONTACT'))};env.update(base)
 env['RRR3D_EXPERIMENT_NATIVE_FROZEN_CM_RESTORE']='1'
 env['RRR3D_EXPERIMENT_NATIVE_CALLBACK_TRIANGLE_NORMAL']='1'
 if mode=='ordinary':env['RRR3D_EXPERIMENT_NATIVE_POSITIVE_Z_FACE_ORDER']='1'
 else:env.pop('RRR3D_EXPERIMENT_NATIVE_POSITIVE_Z_FACE_ORDER',None)
 # The explicit ladder must override a manifest that already enabled v43.
 for suffix in ['INITIAL_COM','ACTOR_ORIGIN','ACTOR_OUTPUT','SHAPE_TRANSLATION','MASS_DOT_ORDER','FROZEN_CM_EXACT']:env.pop('RRR3D_EXPERIMENT_NATIVE_'+suffix,None)
 if mode=='ordinary':
  for suffix in a.flags.split(','):
   if suffix:env['RRR3D_EXPERIMENT_NATIVE_'+suffix]='1'
 else:
  for suffix in ['INITIAL_COM','ACTOR_ORIGIN','ACTOR_OUTPUT','SHAPE_TRANSLATION','MASS_DOT_ORDER','FROZEN_CM_EXACT']:env['RRR3D_EXPERIMENT_NATIVE_'+suffix]='1'
 runs=[]
 for repeat in range(2):
  dest=out/f'expanded-pose-{mode}-run{repeat}-mac.jsonl';subprocess.run([str(probe),str(dest),'list:'+','.join(native)],env=env,check=True,stdout=subprocess.DEVNULL);mac=compare.load(dest,'mac-jolt-metal');assert set(mac)==set(native);runs.append(mac)
  stats={n:{'first_difference':next((x['tick'] for x,y in zip(native[n]['rows'],mac[n]['rows']) if x!=y),None),'max_position':max(compare.norm(x['position'],y['position']) for x,y in zip(native[n]['rows'],mac[n]['rows'])),'max_velocity':max(compare.norm(x['velocity'],y['velocity']) for x,y in zip(native[n]['rows'],mac[n]['rows']))} for n in native};report['captures'][mode+str(repeat)]={'sha256':hashlib.sha256(dest.read_bytes()).hexdigest(),'environment':{k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},'cases':stats};print(mode,repeat,stats,flush=True)
 assert runs[0]==runs[1],('Expanded Mac cases not repeatable',mode)
(out/'capture-manifest.json').write_text(json.dumps(report,indent=2)+'\n')
