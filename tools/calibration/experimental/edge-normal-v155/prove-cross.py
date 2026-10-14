import json,gzip,struct,math
from pathlib import Path
f=lambda x:struct.unpack('<f',struct.pack('<f',x))[0];w=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];u=lambda x:struct.unpack('<f',struct.pack('<I',x))[0]
s=[r['sample'] for r in map(json.loads,open('outputs/retained-friction-v154/source-rows478/track0-physical.jsonl')) if r['event']=='physics.native-edge-stages' and r['sample']['accepted']]
n=[r for r in map(json.loads,gzip.open('outputs/retained-friction-v154/native-step478/track0-run0-private.jsonl.gz','rt')) if r.get('function')=='edge-crossing-accepted']
reports=[]
for a,b in zip(s,n):
 v=list(map(f,a['delta']));normal=list(map(f,a['direction']));result=[]
 for name,fuse in [('unfused',False),('fused-first',True)]:
  c=[f(math.fma(v[j],normal[k],-f(v[k]*normal[j]))) if fuse else f(f(v[j]*normal[k])-f(v[k]*normal[j])) for j,k in [(1,2),(2,0),(0,1)]]
  length=f(f(f(c[0]*c[0])+f(c[2]*c[2]))+f(c[1]*c[1]));inv=1/math.sqrt(length);unit=[f(x*inv) for x in c]
  result.append(dict(mode=name,raw_cross=list(map(w,c)),unit=list(map(w,unit)),native_exact=list(map(w,unit))==b['edge_plane_normal']['words'],source_exact=list(map(w,unit))==list(map(w,a['plane_normal']))))
 reports.append(dict(triangle=a['triangle'],modes=result))
print(json.dumps(reports,indent=2));Path('work/edge-normal-v155/cross-association-proof.json').write_text(json.dumps(dict(scope='Offline identical actual map0/478 delta/normal inputs. Candidate only, no runtime implementation yet.',reports=reports,goal_complete=False),indent=2)+'\n')
