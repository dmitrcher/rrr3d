from pathlib import Path
import json,sys,copy
from collections import defaultdict
# Reuse functions without rerunning the driver.
model_path=Path(__file__).with_name('row_model.py');scope={'__file__':str(model_path)};code=model_path.read_text();exec(code[:code.index('report=[]')],scope)
f,fv,solve,source_data,compare,contracts=[scope[k] for k in ['f','fv','solve','source_data','compare','contracts']]
out=scope['out'];dt=f(1/60);coefficient=f(.8);report=[]
def converted(error,mode):
 if mode=='mul-div':return f(f(error*coefficient)/dt)
 if mode=='pre-scale':return f(error*f(coefficient/dt))
 if mode=='mul-invdt':return f(f(error*coefficient)*f(1/dt))
 if mode=='double':return f(error*coefficient/dt)
 raise ValueError(mode)
for count in [1,2,4,8]:
 path=out/f'reference-row-iterations{count}-run0-v30-windows.jsonl';native=compare.load(path,'windows-physx-d3d9');inputs=defaultdict(lambda:defaultdict(list))
 for line in path.open():
  r=json.loads(line);n=r['detail'];s=r.get('sample')
  if n in contracts and s and r['event'] in ['calibration.body-points','calibration.local-points'] and s['tick']==contracts[n]['first_tick']:inputs[n][r['event']].append(s)
 for mask in [0,2]:
  p=out/(f'calibration-row-arithmetic-both-mask{mask}-run0-v30-mac.jsonl' if count==4 else f'calibration-row-iterations{count}-both-mask{mask}-run0-v30-mac.jsonl');src=source_data(p);actual=compare.load(p,'mac-jolt-metal')
  for n,rows in src.items():
   t=contracts[n]['first_tick'];prev=native[n]['rows'][t-1]['velocity'];g=f(native[n]['meta']['gravity']);velocity=fv(prev);velocity[2]=f(velocity[2]+f(g*dt))
   center=fv(inputs[n]['calibration.local-points'][0]['point_frame0_position']);target=native[n]['rows'][t]
   original=solve(rows,velocity,count,bool(mask&2));position=[f(center[a]+f(original[0]['velocity'][a]*dt)) for a in range(3)]
   assert position==fv(actual[n]['rows'][t]['position'])
   entry={'iterations':count,'linear':bool(mask&2),'case':n,'source_position_reconstruction_exact':True,'baseline_velocity_error':compare.norm(fv(target['velocity']),original[-1]['velocity']),'baseline_position_error':compare.norm(fv(target['position']),position),'models':{}}
   nativepoints=inputs[n]['calibration.body-points'];assert len(nativepoints)==16
   for phase,points in [('first',nativepoints[:8]),('last',nativepoints[-8:])]:
    for mode in ['mul-div','pre-scale','mul-invdt','double']:
     for mass in ['observed','double_sum','double_product']:
      changed=copy.deepcopy(rows)
      for i,p0 in enumerate(points):
       error=f(p0['constraint_error'][0]);b=converted(error,mode)
       changed[i]['velocity_bias']=b;changed[8+i]['velocity_bias']=max(0.,b)
      pred=solve(changed,velocity,count,bool(mask&2),mass)
      position=[f(center[a]+f(pred[0]['velocity'][a]*dt)) for a in range(3)]
      ve=compare.norm(fv(target['velocity']),pred[-1]['velocity']);pe=compare.norm(fv(target['position']),position)
      entry['models'][phase+'-'+mode+'-'+mass]={'velocity_error':ve,'position_error':pe,'position':position,'velocity':pred[-1]['velocity'],'both_exact':ve==0 and pe==0}
   report.append(entry)
  print(count,mask,'bias study complete',flush=True)
stats={}
for e in report:
 for mode,values in e['models'].items():
  s=stats.setdefault(mode,{'exact_cases':0,'velocity_improved':0,'velocity_regressed':0,'position_improved':0,'position_regressed':0,'max_velocity':0.,'max_position':0.,'cases':0})
  s['cases']+=1;s['exact_cases']+=values['both_exact'];s['velocity_improved']+=values['velocity_error']<e['baseline_velocity_error'];s['velocity_regressed']+=values['velocity_error']>e['baseline_velocity_error'];s['position_improved']+=values['position_error']<e['baseline_position_error'];s['position_regressed']+=values['position_error']>e['baseline_position_error'];s['max_velocity']=max(s['max_velocity'],values['velocity_error']);s['max_position']=max(s['max_position'],values['position_error'])
(out/'physics-v30-raw-bias-model-audit.json').write_text(json.dumps({'scope':'Diagnostic model with recorded Native raw callback error. Not independently generated geometry or production parity.','models':stats,'cases':report,'goal_complete':False},indent=2)+'\n')
print(json.dumps(stats,indent=2))
