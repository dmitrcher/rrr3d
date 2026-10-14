from pathlib import Path
import json,os,subprocess,importlib.util
r=Path.cwd();o=r/'outputs/body-pitch-v119/factorial';o.mkdir(exist_ok=False);f=r/'work/frozen-v119-friction-candidate';m=json.loads((f/'manifest.json').read_text());spec=importlib.util.spec_from_file_location('words',r/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
def load(path):return [x['sample'] for x in map(json.loads,path.open()) if x['event']=='calibration.sample' and x['detail']=='body-tilt-pitch']
golden=load(r/'outputs/calibration-v14-source-windows.jsonl');control=load(r/'outputs/body-pitch-v119/focused-friction/control-0.jsonl');cache=load(r/'outputs/body-pitch-v119/focused/candidate-0.jsonl');report={};flags=['RRR3D_EXPERIMENT_NATIVE_CACHE_WORLD_POINT_V119','RRR3D_EXPERIMENT_NATIVE_FRICTION_MASS_ORDER_V119']
for label,enabled in [('off',[]),('cache-only',[flags[0]]),('friction-only',[flags[1]]),('both',flags)]:
 runs=[]
 for rep in [0,1]:
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(m['environment'])
  for k in flags:env.pop(k,None)
  env.update({k:'1' for k in enabled});p=o/f'{label}-{rep}.jsonl';subprocess.run([str(f/'rrr3d_physics_calibration'),str(p),'list:body-tilt-pitch'],env=env,capture_output=True,check=True,timeout=120);runs.append(load(p))
 assert len(runs[0])==180 and runs[0]==runs[1]
 if label=='off':assert runs[0]==control
 if label=='cache-only':assert runs[0]==cache
 dif=[i for i,(a,b) in enumerate(zip(runs[0],golden)) if words.physical(a)!=words.physical(b)];report[label]={'first_difference':dif[0] if dif else None,'difference_steps':len(dif),'full_states':180,'repeats_exact':True};print(label,report[label],flush=True)
assert report['off']['first_difference']==166 and report['cache-only']['first_difference']==175 and report['friction-only']['first_difference']==166 and report['both']['first_difference'] is None
(o/'report.json').write_text(json.dumps({'scope':'Two independent default-off fixes; factorial actual uninterrupted180-step trajectories and two repeats each, frozen final candidate. Flags off preserve complete V118 decimal history, cache-only preserves intermediate candidate. Same original initial conditions and commands; no resets or tuning.','modes':report,'goal_complete':False},indent=2)+'\n')
