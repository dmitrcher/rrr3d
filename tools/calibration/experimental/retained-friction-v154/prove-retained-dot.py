from pathlib import Path
import json,gzip,struct
root=Path.cwd();p=root/'work/shape-center-v153'
f=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
w=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
u=lambda x:struct.unpack('<f',struct.pack('<I',x))[0]
add=lambda a,b:f(a+b);sub=lambda a,b:f(a-b);mul=lambda a,b:f(a*b);div=lambda a,b:f(a/b)
def dot(a,b,order):return add(add(mul(a[order[0]],b[order[0]]),mul(a[order[1]],b[order[1]])),mul(a[order[2]],b[order[2]]))
def cross(a,b):return [sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))]
source=list(map(json.loads,open(root/'outputs/mixed-contact-v152/source-track16-step249/track16-physical.jsonl')))
s=[x['sample'] for x in source if x['event']=='physics.native-contact-row']
native=list(map(json.loads,gzip.open(root/'outputs/mixed-contact-v152/native-track16-step249/track16-run0-private.jsonl.gz','rt')))
n=[x for x in native if x.get('function')=='solver-initialize-exit']
row=list(map(u,n[2]['native_solver_row']['words']));body=list(map(u,n[2]['native_body0']['words']))
assert list(map(w,s[2]['angular']))==list(map(w,body[4:7]))
q=row[16:20];x2=mul(q[0],2);y2=mul(q[1],2);z2=mul(q[2],2)
t0=[sub(mul(q[1],x2),mul(q[3],z2)),sub(1,add(mul(q[2],z2),mul(q[0],x2))),add(mul(q[2],y2),mul(q[3],x2))]
t1=[add(mul(q[2],x2),mul(q[3],y2)),sub(mul(q[2],y2),mul(q[3],x2)),sub(1,add(mul(q[1],y2),mul(q[0],x2)))]
r=list(map(f,s[2]['lever_arm']));normal=list(map(f,s[2]['normal']));old=row[27]
point=[add(v,a) for v,a in zip(body[:3],cross(body[4:7],r))]
delta=sub(mul(row[20],-dot(normal,point,[0,2,1])),row[24]);total=add(old,delta)
assert total>0
reports=[]
for name,order in [('old_XZY',[0,2,1]),('native_ZYX',[2,1,0])]:
 previous1=dot(t1,row[12:15],order);previous0=dot(t0,row[12:15],order)
 desired1=mul(row[22],-dot(t1,point,[0,2,1]));target1=add(previous1,desired1)
 if abs(target1)>mul(row[11],total):desired1=sub(mul(div(mul(row[23],total),abs(target1)),target1),previous1)
 desired0=mul(row[21],-dot(t0,point,[0,2,1]));target0=add(previous0,desired0)
 if abs(target0)>mul(row[7],total):desired0=sub(mul(div(mul(row[15],total),abs(target0)),target0),previous0)
 friction=[add(mul(t0[i],desired0),mul(t1[i],desired1)) for i in range(3)]
 force=[add(friction[i],mul(normal[i],delta)) for i in range(3)]
 torque=cross(r,force);tensor=body[15:24]
 response=[add(add(mul(tensor[i],torque[0]),mul(tensor[3+i],torque[1])),mul(tensor[6+i],torque[2])) for i in range(3)]
 angular=[add(body[4+i],response[i]) for i in range(3)];velocity=[add(body[i],mul(force[i],body[11])) for i in range(3)]
 expected=n[3]['native_body0']['words']
 reports.append(dict(mode=name,previous_zero_axis=w(previous0),force_words=list(map(w,force)),angular_words=list(map(w,angular)),velocity_words=list(map(w,velocity)),native_angular_exact=list(map(w,angular))==expected[4:7],source_angular_exact=list(map(w,angular))==list(map(w,s[3]['angular']))))
print(json.dumps(reports,indent=2))
assert reports[0]['source_angular_exact']
assert reports[1]['native_angular_exact']
(p/'retained-dot-proof.json').write_text(json.dumps(dict(scope='Offline identical captured Native/Source row0 iteration1 inputs, map16 physical249. Core1e8215..1e824a retained friction projections sumZ,Y,X; point velocity projections remainX,Z,Y. New association changes only the retained projection and reproduces the next original private body state.',reports=reports,goal_complete=False),indent=2)+'\n')
