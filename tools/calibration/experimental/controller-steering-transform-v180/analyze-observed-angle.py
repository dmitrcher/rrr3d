from pathlib import Path
import json,gzip,struct,math
out=Path('outputs/controller-steering-transform-v180')
def f(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def value(u):return struct.unpack('<f',struct.pack('<I',u))[0]
def word(x):return struct.unpack('<I',struct.pack('<f',x))[0] if x else 0
def add(a,b):return f(a+b)
def sub(a,b):return f(a-b)
def mul(a,b):return f(a*b)
def div(a,b):return f(a/b)
def matrix(q):
 x,y,z,w=q;xx=mul(mul(x,x),2);yy=mul(mul(y,y),2);zz=mul(mul(z,z),2);xy=mul(mul(x,y),2);xz=mul(mul(x,z),2);yz=mul(mul(y,z),2);wx=mul(mul(w,x),2);wy=mul(mul(w,y),2);wz=mul(mul(w,z),2)
 return [[sub(sub(1,yy),zz),sub(xy,wz),add(xz,wy)],[add(xy,wz),sub(sub(1,xx),zz),sub(yz,wx)],[sub(xz,wy),add(yz,wx),sub(sub(1,xx),yy)]]
def dot(a,b):return add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]))
def mm(a,b):return [[dot(a[i],[b[k][j] for k in range(3)]) for j in range(3)] for i in range(3)]
def translation(r,t,p):return [add(dot(row,p),t[i]) for i,row in enumerate(r)]
def quaternion(r):
 tr=add(add(r[0][0],r[1][1]),r[2][2]);assert tr>=0;s=f(math.sqrt(add(tr,1)));w=mul(.5,s);s=div(.5,s);return [mul(sub(r[2][1],r[1][2]),s),mul(sub(r[0][2],r[2][0]),s),mul(sub(r[1][0],r[0][1]),s),w]
rows=[x for x in map(json.loads,gzip.open(out/'accepted-native0.jsonl.gz','rt')) if x['event']=='steering.pose'];bad={k:[] for k in ['position','quaternion']};results=[]
for n in rows:
 r=[[value(n['matrix_before'][i*3+j]) for j in range(3)] for i in range(3)];p=list(map(value,n['position_before']));speed=value(n['speed_word']);speed=0 if abs(speed)<1 else speed;inp=value(n['input_word']);angle=mul(inp,value(n['maximum_word']));alpha=max(-1,min(1,div(speed,10)));yaw=mul(mul(div(mul(alpha,angle),value(n['maximum_word'])),value(1078530011)),div(1,60))
 if n['angle_hook_hit']:yaw=value(n['angle_word'])
 if angle:
  half=mul(yaw,.5);rot=matrix([0,0,f(math.sin(half)),f(math.cos(half))]);pivot=translation(r,p,[-1,0,0]);r=mm(r,rot);p=translation(r,pivot,[1,0,0]);q=quaternion(r)
 else:q=list(map(value,n['rotation_before']))
 for key,pred,target in [('position',p,n['position_call']),('quaternion',q,n['rotation_call'])]:
  if list(map(word,pred))!=[0 if x==0x80000000 else x for x in target]:bad[key].append(dict(case=n['case'],expected=target,actual=list(map(word,pred))))
 results.append(dict(case=n['case'],yaw_word=word(yaw),position=list(map(word,p)),rotation=list(map(word,q))))
print({k:len(v) for k,v in bad.items()});print({k:v[:4] for k,v in bad.items()});(out/'transform-known-angle-analysis.json').write_text(json.dumps(dict(cases=len(rows),differences=bad,predictions=results),indent=2)+'\n')
