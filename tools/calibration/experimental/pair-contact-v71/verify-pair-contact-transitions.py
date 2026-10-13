"""Reconstruct original pair contact updates from each exact saved Native input.
Only compare immediate adjacent contact rows in the same contact solve group.
No Mac state, trajectory fitting, or across-group assumptions enter this proof.
"""
from pathlib import Path
import gzip,json,struct,hashlib
f=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
fs=lambda w:[struct.unpack('<f',struct.pack('<I',x))[0] for x in w]
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
add=lambda a,b:f(a+b)
sub=lambda a,b:f(a-b)
mul=lambda a,b:f(a*b)
div=lambda a,b:f(a/b)
def cross(a,b):return [sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))]
def dot(a,b):return add(add(mul(a[0],b[0]),mul(a[2],b[2])),mul(a[1],b[1]))
def axes(q):
 x,y,z,w=q
 xx=mul(mul(2,x),x);yy=mul(mul(2,y),y);zz=mul(mul(2,z),z)
 xy=mul(mul(2,x),y);xz=mul(mul(2,x),z);yz=mul(mul(2,y),z)
 xw=mul(mul(2,x),w);yw=mul(mul(2,y),w);zw=mul(mul(2,z),w)
 return [[sub(1,add(yy,zz)),add(xy,zw),sub(xz,yw)],[sub(xy,zw),sub(1,add(xx,zz)),add(yz,xw)],[add(xz,yw),sub(yz,xw),sub(1,add(xx,yy))]]
def solve(row,bodies,dynamic_threshold=False):
 N,T1,T2=axes(row[16:20]); points=[]
 for body,r in zip(bodies,[row[4:7],row[8:11]]):points.append([add(a,b) for a,b in zip(body[:3],cross(body[4:7],r))])
 point=[sub(a,b) for a,b in zip(*points)]
 old=row[27];delta=sub(mul(row[20],-dot(N,point)),row[24]);total=add(old,delta)
 if total<0:total=0.;delta=-old
 deltas=[]
 for axis,K,bias,static_mu,dynamic_mu in [(T1,row[21],row[25],row[7],row[15]),(T2,row[22],row[26],row[11],row[23])]:
  previous=dot(axis,row[12:15]);desired=sub(mul(K,-dot(axis,point)),bias);target=add(previous,desired)
  # PhysX uses x87 multiplication for axis1 static comparison (no f32 store).
  limit=(dynamic_mu if dynamic_threshold else static_mu)*total
  if axis is T2:limit=f(limit)
  if total>0 and abs(target)>limit:desired=sub(mul(div(mul(dynamic_mu,total),abs(target)),target),previous)
  deltas.append(desired)
 df=[add(mul(a,deltas[0]),mul(b,deltas[1])) for a,b in zip(T1,T2)] if total>0 else [0.]*3
 force=[add(a,mul(b,delta)) for a,b in zip(df,N)]
 outputs=[]
 for body,r,sign in zip(bodies,[row[4:7],row[8:11]],[1.,-1.]):
  applied=[mul(sign,x) for x in force];torque=cross(r,applied);I=body[15:24]
  v=[add(a,mul(b,body[11])) for a,b in zip(body[:3],applied)]
  w=[add(body[4+k],add(add(mul(I[k],torque[0]),mul(I[3+k],torque[1])),mul(I[6+k],torque[2]))) for k in range(3)]
  outputs.append(v+w)
 return outputs,force
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--captures-root',type=Path,default=Path(__file__).resolve().parent/'fixtures');parser.add_argument('--out',type=Path,default=Path('pair-contact-proof-v71.json'));args=parser.parse_args()
history=[]
for name in ['pair-same-head-0','pair-stock-head-0','pair-pole-oblique-0','pair-vertical-weak-peer-0','pair-weak-second-0']:
 for rep in range(2):
  folder='native-frame' if name in ['pair-same-head-0','pair-stock-head-0'] else 'native-polar-frame'
  path=args.captures_root/f'{folder}/contact-{name}-{rep}-private.jsonl.gz'
  data=list(map(json.loads,gzip.open(path,'rt')))
  entries=[r for r in data if r.get('function')=='solver-iteration-entry'];prepared=[r for r in data if r.get('function')=='solver-initialize-exit']
  assert len(entries)==len(prepared) and len(entries)%5==0
  count=len(entries)//5
  for j,entry in enumerate(entries):
   if j%count==count-1:continue
   row=fs(prepared[j]['native_solver_row']['words']); bodies=[fs(entry[f'native_body{k}']['words']) for k in range(2)]
   outputs,force=solve(row,bodies);alternate,_=solve(row,bodies,True)
   expected=[]
   for k in range(2):
    assert entry[f'native_body{k}']['address']==entries[j+1][f'native_body{k}']['address']
    b=fs(entries[j+1][f'native_body{k}']['words']);expected.append(b[:3]+b[4:7])
   actual=[bits(v) for b in outputs for v in b];wanted=[bits(v) for b in expected for v in b]
   history.append({'case':name,'repeat':rep,'tick':entry['sample_tick_v71'],'iteration':j//count+1,'contact':j%count,'exact':actual==wanted,'dynamic_threshold_exact':[bits(v) for b in alternate for v in b]==wanted,'computed_words':actual,'native_words':wanted,'force':force,'max_error':max(abs(a-b) for c,d in zip(outputs,expected) for a,b in zip(c,d))})
report={'scope':__doc__,'exact_transitions':sum(r['exact'] for r in history),'transitions':len(history),'dynamic_threshold_exact':sum(r['dynamic_threshold_exact'] for r in history),'history':history,'goal_complete':False}
args.out.write_text(json.dumps(report,indent=2)+'\n')
print(report['exact_transitions'],'/',len(history),'exact; dynamic-only threshold',report['dynamic_threshold_exact'])
for r in history:
 if not r['exact']:print(r)
