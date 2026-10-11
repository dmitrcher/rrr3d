from pathlib import Path
import json,struct,sys,argparse
from collections import defaultdict
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
parser=argparse.ArgumentParser(description='Verify saved v30 flat-contact diagnostic captures.')
parser.add_argument('--measurements',type=Path,required=True)
parser.add_argument('--contracts',type=Path,required=True)
args=parser.parse_args()
out=args.measurements;contracts={c['case']:c for c in json.loads(args.contracts.read_text())}
f=lambda x:struct.unpack('f',struct.pack('f',x))[0]
fv=lambda v:[f(x) for x in v]
def dot(a,b):
 p=[f(x*y) for x,y in zip(a,b)];return f(f(p[0]+p[2])+p[1])
def source_data(path):
 d=defaultdict(list);n=None;normals={}
 for line in path.open():
  r=json.loads(line)
  if r['event']=='calibration.begin':n=r['detail']
  if r['event']=='physics.contact-manifold' and r['sample']['step']==contracts[n]['first_tick']:normals[n]=r['sample']['normal']
  if r['event']=='physics.source-normal-row' and r['sample']['step']==contracts[n]['first_tick']:
   sample={**r['sample'],'normal_from_manifold':normals[n]};assert sample['body0_inverse_mass']==0 and sample['body1_inverse_mass']>0;d[n].append(sample)
 return d
def solve(rows,velocity,count,linear,k_mode='observed',dot_mode='float32',bias_mode='observed'):
 normal=fv(rows[0]['normal_from_manifold']);invM=f(rows[0]['body1_inverse_mass']);v=fv(velocity);w=[0.,0.,0.];lambdas=[0.]*8;state=[]
 def dp(a,b):return dot(a,b) if dot_mode=='float32' else f(sum(x*y for x,y in zip(a,b)))
 masses=[]
 for row in rows[:8]:
  if k_mode=='observed':k=f(row['effective_mass'])
  else:
   a=fv(row['r_cross_normal1']);b=fv(row['inverse_inertia_r_cross_normal1']);products=[x*y for x,y in zip(a,b)]
   if k_mode=='double_sum':products=[f(x) for x in products]
   k=f(1/(invM+sum(products)))
  masses.append(k)
 for it in range(count+1):
  for i,row in enumerate(rows[:8]):
   b=f(row['velocity_bias']) if it<count else f(rows[8+i]['velocity_bias']);r=fv(row['r_cross_normal1']);response=fv(row['inverse_inertia_r_cross_normal1'])
   jv=f(dp(normal,[-x for x in v])-dp(r,w));delta=f(masses[i]*f(jv-b));total=max(0.,f(lambdas[i]+delta));change=f(total-lambdas[i]);lambdas[i]=total
   if change!=0:
    for axis in range(3):
     impulse=f(f(normal[axis]*invM)*change) if linear else f(f(change*invM)*normal[axis])
     v[axis]=f(v[axis]+impulse);w[axis]=f(w[axis]+f(change*response[axis]))
  if it==count-1:state.append({'phase':'biased-final','velocity':v.copy(),'angular':w.copy(),'lambda':lambdas.copy()})
 state.append({'phase':'unbiased-final','velocity':v.copy(),'angular':w.copy(),'lambda':lambdas.copy()})
 return state
report=[]
for count in [1,2,4,8]:
 native=compare.load(out/f'reference-row-iterations{count}-run0-v30-windows.jsonl','windows-physx-d3d9')
 for mask in [0,2]:
  p=out/(f'calibration-row-arithmetic-both-mask{mask}-run0-v30-mac.jsonl' if count==4 else f'calibration-row-iterations{count}-both-mask{mask}-run0-v30-mac.jsonl')
  src=source_data(p);errors=[]
  for n,rows in src.items():
   t=contracts[n]['first_tick'];prev=native[n]['rows'][t-1]['velocity'];g=f(native[n]['meta']['gravity']);velocity=fv(prev);velocity[2]=f(velocity[2]+f(g*f(1/60)))
   got=solve(rows,velocity,count,bool(mask&2))
   for state in got:
    actual=[r for r in rows if r['phase']==state['phase']]
    assert len(actual)==8
    assert state['lambda']==[f(r['normal_lambda']) for r in actual],(count,mask,n,state['phase'],'lambda',state['lambda'],[f(r['normal_lambda']) for r in actual])
    assert state['velocity']==fv(actual[0]['velocity1']),(count,mask,n,state['phase'],'velocity',state['velocity'],fv(actual[0]['velocity1']))
    assert state['angular']==fv(actual[0]['angular1']),(count,mask,n,state['phase'],'angular',state['angular'],fv(actual[0]['angular1']))
   nr=fv(native[n]['rows'][t]['velocity'])
   entry={'iterations':count,'linear':bool(mask&2),'case':n,'mac_model_exact':True,'baseline_error':compare.norm(nr,got[-1]['velocity']),'models':{}}
   for mass in ['observed','double_sum','double_product']:
    for dm in ['float32','double_sum']:
     new=solve(rows,velocity,count,bool(mask&2),mass,dm)
     entry['models'][mass+'-'+dm]={'velocity_error':compare.norm(nr,new[-1]['velocity']),'velocity':new[-1]['velocity'],'biased_velocity':new[0]['velocity']}
   errors.append(entry)
  report+=errors
  print('Exact standalone float32 reconstruction:',count,mask,len(errors),'cases',flush=True)
(out/'physics-v30-normal-row-model-audit.json').write_text(json.dumps({'scope':'Independent arithmetic model of recorded prepared rows, not an accepted Mac physics fix. Must reproduce observed Mac biased/unbiased lambdas and velocities before Native comparisons.','cases':report,'goal_complete':False},indent=2)+'\n')
