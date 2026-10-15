from pathlib import Path
import json,gzip,struct,itertools,collections
p=Path('outputs/controller-stabilize-v185');rows=[n for n in map(json.loads,gzip.open(p/'accepted-native0.jsonl.gz','rt')) if n['event']=='stabilize.pose' and n['clutch_case']==0 and n['damping_case']<3]
f=lambda u:struct.unpack('<f',struct.pack('<I',u))[0];word=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];s=lambda x:f(word(x));cz=lambda a:[0 if x==0x80000000 else x for x in a];reports=[]
for co_full,det_full,add_full,mul_full in itertools.product([False,True],repeat=4):
 invexact=locexact=0
 for n in rows:
  m=list(map(f,n['checkpoint0'][:9]));r=[0.]*9
  for i in range(3):
   for j in range(3):
    a=(j+1)%3;b=(j+2)%3;c=(i+1)%3;d=(i+2)%3
    r[i*3+j]=s(m[a*3+c]*m[b*3+d]-m[a*3+d]*m[b*3+c]) if co_full else s(s(m[a*3+c]*m[b*3+d])-s(m[a*3+d]*m[b*3+c]))
  ts=[r[k]*m[k*3] for k in range(3)];det=s(sum(ts)) if det_full else s(s(s(ts[0])+s(ts[1]))+s(ts[2]));scale=s(1./det);r=[s(x*scale) for x in r];invexact+=cz(list(map(word,r)))==cz(n['checkpoint0'][9:18])
  actualinv=list(map(f,n['checkpoint0'][9:18]));v=list(map(f,n['momentum_before']));out=[]
  for i in range(3):
   ts=[actualinv[i*3+k]*v[k] for k in range(3)];ts=ts if mul_full else list(map(s,ts));out.append(s(sum(ts)) if add_full else s(s(ts[0]+ts[1])+ts[2]))
  locexact+=cz(list(map(word,out)))==cz(n['checkpoint1'])
 reports.append(dict(cofactor_double=co_full,det_double=det_full,add_double=add_full,multiply_double=mul_full,exact_inverse=invexact,exact_local_using_native_inverse=locexact,cases=len(rows)))
for r in sorted(reports,key=lambda r:(r['exact_inverse'],r['exact_local_using_native_inverse']),reverse=True)[:8]:print(r)
(p/'arithmetic-variants.json').write_text(json.dumps(reports,indent=2)+'\n')
