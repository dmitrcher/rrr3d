#!/usr/bin/env python3
"""Validate the solver model against independent PhysX 2.8.4 probes."""
import json,math,sys
from pathlib import Path

def predict(case,n,final=True,reverse=True):
 inp,actual=case['input'],case['actual'];ref=case['iterations'][str(n)]
 mass,inertia,com=actual['mass'],actual['inertia'],actual['com'];dt=inp['dt'];v=inp['initial_velocity'][2]+inp['gravity']*dt;wx=wy=0
 wheels=inp['fixture_parameters']['wheels'];lam=[0.0]*len(wheels);pose_v=0
 order=range(len(wheels)-1,-1,-1) if reverse else range(len(wheels))
 for step in range(n+1):
  for index in order:
   if not ref['wheels'][index]['contact']:continue
   w=wheels[index];x=w['position'][0]-com[0];y=w['position'][1]-com[1]
   gamma=1/(dt*(w['damper']+dt*w['spring']));response=1/mass+y*y/inertia[0]+x*x/inertia[1];effective=1/(response+gamma);retention=response*effective
   c=ref['wheels'][index]['suspension_length']-w['travel']+.05-w['target'];bias=.7*dt*w['spring']*gamma*c
   if step==n and final:bias*=1-retention**n
   old=lam[index];lam[index]=max(0,retention*old-effective*(v+wx*y-wy*x+bias));delta=lam[index]-old
   v+=delta/mass;wx+=y*delta/inertia[0];wy-=x*delta/inertia[1]
  if step==n-1:pose_v=v
 return v,wx*inertia[0],wy*inertia[1],inp['height']+pose_v*dt

def main():
 cases=json.loads(Path(sys.argv[1]).read_text())['cases'];assert len(cases)==63
 maxima=[0.0]*4;wrong_final=wrong_order=0
 for c in cases:
  for n in [1,2,4,8]:
   ref=c['iterations'][str(n)];observed=[ref['velocity'][2],*ref['angular_momentum'][:2],ref['position'][2]]
   errors=[abs(a-b) for a,b in zip(predict(c,n),observed)];maxima=[max(a,b) for a,b in zip(maxima,errors)]
   assert errors[0]<5e-6 and max(errors[1:3])<.003,(c['name'],n,errors)
   # Height prediction is the COM position; an offset actor origin also rotates.
   if not any(c['actual']['com']):assert errors[3]<3e-6,(c['name'],n,errors)
   wrong_final+=abs(predict(c,n,False)[0]-observed[0])>1e-4
   wrong_order+=math.dist(predict(c,n,reverse=False)[1:3],observed[1:3])>.01
 assert wrong_final>=30 and wrong_order>=30,(wrong_final,wrong_order)
 print('252 independent first-step comparisons passed; maxima:',maxima,'rejected wrong final bias:',wrong_final,'wrong wheel order:',wrong_order)
if __name__=='__main__':main()
