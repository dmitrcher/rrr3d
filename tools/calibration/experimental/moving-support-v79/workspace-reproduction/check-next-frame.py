from pathlib import Path
import json,gzip,struct
F=lambda v:struct.unpack('<f',struct.pack('<f',v))[0]
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
f=lambda v:struct.unpack('<f',struct.pack('<I',v))[0]
add=lambda a,b:F(a+b);sub=lambda a,b:F(a-b);mul=lambda a,b:F(a*b)
def cross(a,b):return [sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))]
def inverse(q,v,direction):
 dot=add(add(mul(v[0],q[0]),mul(v[2 if direction else 1],q[2 if direction else 1])),mul(v[1 if direction else 2],q[1 if direction else 2]));co=sub(mul(q[3],q[3]),.5);cr=cross(q[:3],v)
 return [mul(add(sub(mul(co,v[i]),mul(q[3],cr[i])),mul(q[i],dot)),2)for i in range(3)]
def forward(q,v,direction):
 dot=add(add(mul(v[2 if direction else 0],q[2 if direction else 0]),mul(v[0 if direction else 1],q[0 if direction else 1])),mul(v[1 if direction else 2],q[1 if direction else 2]));co=sub(mul(mul(q[3],q[3]),2),1);cr=cross(q[:3],v)
 return [add(add(mul(co,v[i]),mul(mul(cr[i],q[3]),2)),mul(mul(q[i],dot),2))for i in range(3)]
mac=list(map(json.loads,open('outputs/moving-support-v79/onset29/prep-0.jsonl')))
rows=[x for x in map(json.loads,gzip.open('outputs/moving-support-v77/native-kernel/creation-pair-stock-pole-1-29-0-private.jsonl.gz','rt')) if x.get('function')=='spring-solve-input'][:4]
order=[2,0,3,1];checks=[]
objects=[x for x in map(json.loads,gzip.open('outputs/moving-support-v77/native-kernel/creation-pair-stock-pole-1-29-0-private.jsonl.gz','rt')) if x.get('function')=='spring-row-entry'][:4]
for index,r,obj in zip(order,rows,objects):
 m=mac[index];q=list(map(F,m['quaternion']));n=list(map(F,m['world_axis']));p=list(map(F,m['world_point']));c=list(map(F,m['center']));delta=[sub(p[i],c[i]) for i in range(3)]
 axis=forward(q,inverse(q,n,True),True);lever=forward(q,inverse(q,delta,False),False);jac=cross(lever,axis)
 w=r['solve_row']['words'];got=[bits(x)for x in jac];native=w[12:15]
 ow=obj['spring_object']['words'];localp=list(map(f,ow[9:12]));locala=list(map(f,ow[15:18]));control=cross(forward(q,localp,False),forward(q,locala,True))
 checks.append({'native_local_point':localp,'native_local_axis':locala,'native_local_frame_control_matches':list(map(bits,control))==native,'wheel':index,'mac_direct_jacobian_words':list(map(bits,m['spring']['jacobian'])),'native_jacobian_words':native,'local_roundtrip_words':got,'roundtrip_matches_native':got==native,'native_axis0':list(map(f,w[4:7])),'native_axis1':list(map(f,w[8:11])),'local_roundtrip_axis':axis})
 print(checks[-1])
Path('outputs/moving-support-v79/next-frame-row-audit.json').write_text(json.dumps({'scope':'Physical tick29. Same Mac world contact point, quaternion and center. V57 local-frame reconstruction is a diagnostic control, not enabled for dynamic contacts.','checks':checks,'goal_complete':False},indent=2)+'\n')
