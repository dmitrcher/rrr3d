from pathlib import Path
import json,itertools,sys,struct,math
from collections import defaultdict
model_path=Path(__file__).with_name('row_model.py');scope={'__file__':str(model_path)};code=model_path.read_text();exec(code[:code.index('report=[]')],scope)
f,fv,source_data,compare,contracts=[scope[k] for k in ['f','fv','source_data','compare','contracts']]
out=scope['out'];dt=f(1/60);fixtures=[]
for count in [1,2,4,8]:
 p=out/f'reference-row-iterations{count}-run0-v30-windows.jsonl';native=compare.load(p,'windows-physx-d3d9');inputs=defaultdict(lambda:defaultdict(list))
 for line in p.open():
  r=json.loads(line);n=r['detail'];s=r.get('sample')
  if n in contracts and s and r['event'] in ['calibration.body-points','calibration.local-points','calibration.point-force'] and s['tick']==contracts[n]['first_tick']:inputs[n][r['event']].append(s)
 src=source_data(out/('calibration-row-arithmetic-both-mask0-run0-v30-mac.jsonl' if count==4 else f'calibration-row-iterations{count}-both-mask0-run0-v30-mac.jsonl'))
 for n,rows in src.items():
  t=contracts[n]['first_tick'];c=native[n];velocity=fv(c['rows'][t-1]['velocity']);velocity[2]=f(velocity[2]+f(f(c['meta']['gravity'])*dt));points=inputs[n]['calibration.body-points'];assert len(points)==16
  assert fv(inputs[n]['calibration.point-force'][0]['normal'])==fv(rows[0]['normal_from_manifold'])
  fixtures.append({'name':n,'iterations':count,'velocity':velocity,'center':fv(inputs[n]['calibration.local-points'][0]['point_frame0_position']),
                   'rows':rows,'error':[f(p['constraint_error'][0]) for p in points[:8]],'target_velocity':fv(c['rows'][t]['velocity']),'target_position':fv(c['rows'][t]['position'])})
def predict(x,mass_mode,bias_mode,dot_mode,update_mode,linear_mode,angular_mode):
 N=fv(x['rows'][0]['normal_from_manifold'])
 def dot(a,b):
  p=[f(a[i]*b[i]) for i in range(3)]
  if dot_mode=='xzy':return f(f(p[0]+p[2])+p[1])
  if dot_mode=='xyz':return f(f(p[0]+p[1])+p[2])
  return f(sum(a[i]*b[i] for i in range(3)))
 invM=f(x['rows'][0]['body1_inverse_mass']);r=[fv(row['r_cross_normal1']) for row in x['rows'][:8]];response=[fv(row['inverse_inertia_r_cross_normal1']) for row in x['rows'][:8]]
 k=[];bias=[]
 for i,row in enumerate(x['rows'][:8]):
  if mass_mode=='observed':kk=f(row['effective_mass'])
  else:
   if mass_mode.startswith('unit'):mass=invM
   elif mass_mode.startswith('jacobian'):mass=f(f(f(N[0]*f(N[0]*invM))+f(N[2]*f(N[2]*invM)))+f(N[1]*f(N[1]*invM)))
   else:mass=invM*sum(float(a)*float(a) for a in N)
   products=[r[i][a]*response[i][a] for a in range(3)]
   if mass_mode.endswith('rounded'):products=[f(a) for a in products]
   kk=f(1/(mass+sum(products)))
  k.append(kk);e=x['error'][i]
  if bias_mode=='mul-div':b=f(f(e*f(.8))/dt)
  elif bias_mode=='pre-scale':b=f(e*f(f(.8)/dt))
  else:b=f(e*f(.8)/dt)
  bias.append(b)
 v=x['velocity'].copy();w=[0.,0.,0.];ls=[0.]*8;biased=None
 for it in range(x['iterations']+1):
  for i in range(8):
   nv=dot(N,[-a for a in v]);av=dot(r[i],w);b=bias[i] if it<x['iterations'] else max(0.,bias[i]);jv=f(nv-av)
   if update_mode=='delta-rounded':total=f(ls[i]+f(k[i]*f(jv-b)))
   elif update_mode=='accumulate-extended':total=f(ls[i]+k[i]*f(jv-b))
   elif update_mode=='bias-split':total=f(ls[i]+f(f(k[i]*jv)-f(k[i]*b)))
   elif update_mode=='all-scaled':total=f(ls[i]+f(f(f(k[i]*nv)-f(k[i]*av))-f(k[i]*b)))
   else:total=f(ls[i]+k[i]*(float(nv)-float(av)-float(b)))
   total=max(0.,total);change=f(total-ls[i]);ls[i]=total
   if change:
    for a in range(3):
     if linear_mode=='scalar':v[a]=f(v[a]+f(f(change*invM)*N[a]))
     elif linear_mode=='premultiplied':v[a]=f(v[a]+f(change*f(N[a]*invM)))
     else:v[a]=f(v[a]+change*invM*N[a])
     w[a]=f(w[a]+f(change*response[i][a])) if angular_mode=='rounded' else f(w[a]+change*response[i][a])
  if it==x['iterations']-1:biased=v.copy()
 position=[f(x['center'][a]+f(biased[a]*dt)) for a in range(3)]
 return v,position
options=[['observed','unit-rounded','unit-extended','jacobian-rounded','jacobian-extended','raw-extended'],['mul-div','pre-scale','double'],['xzy','xyz','double'],['delta-rounded','accumulate-extended','bias-split','all-scaled','full-extended'],['scalar','premultiplied','extended'],['rounded','extended']]
results=[]
for modes in itertools.product(*options):
 exact=0;ve=0.;pe=0.;hits=[]
 for x in fixtures:
  v,p=predict(x,*modes);ev=compare.norm(v,x['target_velocity']);ep=compare.norm(p,x['target_position']);ve=max(ve,ev);pe=max(pe,ep)
  if ev==0 and ep==0:exact+=1;hits.append([x['iterations'],x['name']])
 results.append({'modes':list(modes),'exact_cases':exact,'max_velocity_error':ve,'max_position_error':pe,'exact_case_names':hits})
results.sort(key=lambda r:(-r['exact_cases'],r['max_velocity_error'],r['max_position_error']))
report={'scope':'Diagnostic operator search using actual Native geometry and pre-modify errors. The predictor never receives expected outputs. No accepted production or standalone physics fix.',
        'model_count':len(results),'cases_per_model':len(fixtures),'complete_matches':[r for r in results if r['exact_cases']==len(fixtures)],'ranked_models':results,'goal_complete':False}
(out/'physics-v30-operator-search-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print('Models',len(results),'cases',len(fixtures),'complete',len(report['complete_matches']),'best',json.dumps(results[:5]),flush=True)
