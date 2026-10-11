#!/usr/bin/env python3
"""Check measured Nx lateral tire impulse envelopes in both motion directions.

This scalar oracle uses SDK-reported force/slip inputs. It does not predict the
coupled rotating-body solve, whose residual remains under investigation.
"""
import json,sys
from pathlib import Path

def curve(tire,slip):
 x,peak,y,tail,_=tire
 a=abs(slip)
 if a<x:
  t=a/x
  return peak*(-t*t*t+t*t+t)
 if a<y:
  t=(a-x)/(y-x)
  return -2*(tail-peak)*t*t*t+3*(tail-peak)*t*t+peak
 return tail

if __name__=='__main__':
 ref=json.loads(Path(sys.argv[1]).read_text());checked=0;maximum=0;wrong_sign=0
 assert len(ref['groups'])==8
 for name,group in ref['groups'].items():
  assert len(group['cases'])==144
  for case in group['cases']:
   assert len(case['samples'])==24
   direction=-1 if case['initial_velocity'][1]>0 else 1 if case['initial_velocity'][1]<0 else 0
   for sample in case['samples']:
    for i,(slip,force,actual) in enumerate(sample['wheels']):
     expected=direction*curve(case['tires'][i],slip)*force*case['dt']
     error=abs(expected-actual);maximum=max(maximum,error)
     assert error<1.0e-6,(name,case['name'],sample['tick'],i,expected,actual)
     wrong_sign+=abs(-expected-actual)>1.0e-5;checked+=1
 assert checked==110592 and wrong_sign>10000
 print(checked,'Nx lateral impulse envelope channels passed; max error',maximum)
