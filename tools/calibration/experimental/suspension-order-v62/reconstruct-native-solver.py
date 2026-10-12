from pathlib import Path
import json,struct
f=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
fs=lambda w:[struct.unpack('<f',struct.pack('<I',x))[0] for x in w]
add=lambda a,b:f(a+b);sub=lambda a,b:f(a-b);mul=lambda a,b:f(a*b);div=lambda a,b:f(a/b)
def cross(a,b):return [sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))]
def dot(a,b):return add(add(mul(a[0],b[0]),mul(a[2],b[2])),mul(a[1],b[1]))
def axes(q):
 x,y,z,w=q;xx=mul(mul(2,x),x);yy=mul(mul(2,y),y);zz=mul(mul(2,z),z);xy=mul(mul(2,x),y);xz=mul(mul(2,x),z);yz=mul(mul(2,y),z);xw=mul(mul(2,x),w);yw=mul(mul(2,y),w);zw=mul(mul(2,z),w)
 return [[sub(1,add(yy,zz)),add(xy,zw),sub(xz,yw)],[sub(xy,zw),sub(1,add(xx,zz)),add(yz,xw)],[add(xz,yw),sub(yz,xw),sub(1,add(xx,yy))]]
import gzip
ns=[json.loads(l) for l in gzip.open('outputs/triangle-native-followup-v60/native-border-grounded-62-0-private.jsonl.gz','rt')]
prepared=[r for r in ns if r.get('simulation_step')==61 and r.get('function')=='solver-initialize-exit'][:4]
entries=[r for r in ns if r.get('simulation_step')==61 and r.get('function')=='solver-iteration-entry']
rows=[fs(r['native_solver_row']['words']) for r in prepared]
body=fs(entries[0]['native_body0']['words']);v=body[:3];w=body[4:7];I=body[15:24];M=body[11];lambdas=[0.]*4;friction=[[0.]*3 for _ in range(4)];issues=[];history=[]
for j in range(20):
 i=j%4;rw=rows[i];flip=i>=2;r=rw[8:11] if flip else rw[4:7];N,T1,T2=axes(rw[16:20]);N=[-x for x in N] if flip else N
 point=[add(a,k) for a,k in zip(v,cross(w,r))];bias=rw[24] if j<16 else max(0.,rw[24]);delta=sub(mul(rw[20],-dot(N,point)),bias);total=add(lambdas[i],delta)
 if total<0:delta=-lambdas[i];total=0.
 lambdas[i]=total;d=[]
 for axis,K,biasfric,staticmu,dynamicmu in [(T1,rw[21],rw[25],rw[7],rw[15]),(T2,rw[22],rw[26],rw[11],rw[23])]:
  previous=dot(axis,friction[i]);desired=sub(mul(K,-dot(axis,point)),biasfric);target=add(previous,desired)
  if total>0 and abs(target)>mul(staticmu,total):desired=sub(mul(div(mul(dynamicmu,total),abs(target)),target),previous)
  d.append(desired)
 df=[add(mul(a,d[0]),mul(t,d[1])) for a,t in zip(T1,T2)] if total>0 else [0.]*3
 friction[i]=[add(a,k) for a,k in zip(friction[i],df)];force=[add(a,mul(t,delta)) for a,t in zip(df,N)];torque=cross(r,force);v=[add(a,mul(k,M)) for a,k in zip(v,force)];w=[add(w[k],add(add(mul(I[k],torque[0]),mul(I[3+k],torque[1])),mul(I[6+k],torque[2]))) for k in range(3)]
 if j<19:
  nb=fs(entries[j+1]['native_body1' if (j+1)%4>=2 else 'native_body0']['words'])
  item={'entry':j,'velocity':v.copy(),'expected_velocity':nb[:3],'angular':w.copy(),'expected_angular':nb[4:7],'force':force,'friction_delta':df,'lambda':total};history.append(item)
  if v!=nb[:3] or w!=nb[4:7]:issues.append(item)
print(json.dumps({'exact':19-len(issues),'issues':issues[:6]},indent=2))
Path('outputs/physics-v62-native-forward-solver-audit.json').write_text(json.dumps({'scope':'Contact-only reconstruction; group boundaries cross omitted wheel rows and are not valid isolated contact comparisons.','history':history,'issues':issues},indent=2)+'\n')
