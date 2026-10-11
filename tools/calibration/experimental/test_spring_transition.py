#!/usr/bin/env python3
"""Independent Nx scalar oracle checks, including rejected solver alternatives."""
import json,sys,math
from pathlib import Path

def solve(case,previous,current,n,retain=True,cold=True,hard_first=True,activation=0,old_springs=()):
 inp=case['input'];mass=case['actual']['mass'];dt=inp['dt'];mask=current[2];length=current[3]
 if not mask and retain:mask,length=previous[2],previous[3]
 wheels=inp['fixture_parameters']['wheels'];v=previous[1]+inp['gravity']*dt;rows=[]
 for index,w in enumerate(wheels):
  if not mask & (1<<index):continue
  gamma=1/(dt*(w['damper']+dt*w['spring'])) if w['damper']+dt*w['spring'] else 0
  eff=1/(1/mass+gamma);r=eff/mass
  old=old_springs[index] if not cold and len(old_springs)>index else 0
  rows.append({'i':index,'hard':length<activation,'soft':w['spring']>0 or w['damper']>0,'bh':.7*(length+.05)/dt,'bs':.7*dt*w['spring']*gamma*(length-w['travel']+.05-w['target']),'eff':eff,'r':r,'lh':0,'ls':old})
  v+=old/mass
 for step in range(n+1):
  for row in reversed(rows):
   for name in (['hard','soft'] if hard_first else ['soft','hard']):
    if not row[name]:continue
    if name=='hard':
     old=row['lh'];row['lh']=max(0,old-mass*(v+(0 if step==n else row['bh'])));v+=(row['lh']-old)/mass
    else:
     old=row['ls'];row['ls']=max(0,row['r']*old-row['eff']*(v+row['bs']*(1-row['r']**n if step==n else 1)));v+=(row['ls']-old)/mass
  if step==n-1:pose_v=v
 old=[0.0]*4
 for row in rows:old[row['i']]=row['ls']
 return v,previous[0]+pose_v*dt,old

def main():
 cases=json.loads(Path(sys.argv[1]).read_text())['cases'];assert len(cases)==324
 assert sum(c['name'].startswith('lost-spring-') for c in cases)==144
 assert sum(c['name'].startswith('hard-spring-') for c in cases)==180
 maxima=[0,0];samples=transitions=0;rejected={'no_retention':0,'warm_spring':0,'spring_first':0,'late_activation':0}
 for case in cases:
  assert set(case['iterations'])=={'1','2','4','8'}
  for label,series in case['iterations'].items():
   n=int(label);assert len(series)==case['input']['steps'];previous=[case['input']['height'],case['input']['initial_velocity'][2],0,0];warm=[]
   for current in series:
    assert current[2] in [0,1,15]
    pred=solve(case,previous,current,n);errors=[abs(pred[0]-current[1]),abs(pred[1]-current[0])];maxima=[max(a,b) for a,b in zip(maxima,errors)]
    assert errors[0]<3e-6 and errors[1]<3e-6,(case['name'],n,samples,errors)
    if previous[2] and not current[2]:transitions+=1
    rejected['no_retention']+=abs(solve(case,previous,current,n,retain=False)[0]-current[1])>1e-4
    wrong=solve(case,previous,current,n,cold=False,old_springs=warm);warm=wrong[2];rejected['warm_spring']+=abs(wrong[0]-current[1])>1e-4
    rejected['spring_first']+=abs(solve(case,previous,current,n,hard_first=False)[0]-current[1])>1e-4
    rejected['late_activation']+=abs(solve(case,previous,current,n,activation=-.05)[0]-current[1])>1e-4
    previous=current;samples+=1
 assert samples==19152 and transitions==590,(samples,transitions)
 assert all(v>30 for v in rejected.values()),rejected
 print('19152 independent Nx samples passed; 590 contact losses; maxima V/P',maxima,'rejected alternatives',rejected)
if __name__=='__main__':main()
