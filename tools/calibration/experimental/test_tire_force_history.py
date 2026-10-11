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
 return v,previous[0]+pose_v*dt,old,[next((r["ls"]+r["lh"] for r in rows if r["i"]==i),0) for i in range(4)]


if __name__=='__main__':
 cases=json.loads(Path(sys.argv[1]).read_text())['cases'];assert len(cases)==68
 max_error=0;checked=wrong_current=wrong_soft=wrong_zero=0
 for case in cases:
  inp=case['input'];dt=inp['dt'];cap=abs(inp['gravity'])*case['actual']['mass']/4*1.5
  previous=[inp['height'],inp['initial_velocity'][2],0,0];last_soft=[0]*4;last_total=[0]*4
  for a in case['samples']:
   mask=sum(1<<i for i,w in enumerate(a['wheels']) if w['contact']);length=next((w['suspension_length'] for w in a['wheels'] if w['contact']),0);current=[a['position'][2],a['velocity'][2],mask,length];pred=solve(case,previous,current,4)
   for i,w in enumerate(a['wheels']):
    observed=w['engine_normal_force'];expected=min(cap,max(0,last_total[i]/dt)) if w['contact'] else 0
    max_error=max(max_error,abs(expected-observed));assert abs(expected-observed)<.01,(case['name'],a['tick'],i,expected,observed)
    if w['contact']:
     wrong_current+=abs(min(cap,max(0,pred[3][i]/dt))-observed)>.1
     wrong_soft+=abs(min(cap,max(0,last_soft[i]/dt))-observed)>.1
     if cap==0:wrong_zero+=abs(max(0,last_total[i]/dt)-observed)>.1
   last_soft=pred[2] if mask else [0]*4;last_total=pred[3] if mask else [0]*4;previous=current;checked+=1
 assert checked==888 and wrong_current>100 and wrong_soft>10 and wrong_zero>10,(checked,wrong_current,wrong_soft,wrong_zero)
 print('888 independent Nx force-history samples passed; maxN',max_error,'rejected current/soft-only/zero-load',wrong_current,wrong_soft,wrong_zero)
