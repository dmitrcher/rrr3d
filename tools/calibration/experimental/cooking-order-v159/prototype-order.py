from pathlib import Path
import struct,json,gzip
p=Path('work/cooking-order-v159')
def f(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def load(path):
 a=list(map(int,Path(path).read_text().split())); assert len(a)==1+a[0]*9
 return [[[struct.unpack('<f',struct.pack('<I',w))[0] for w in a[1+i*9+j*3:1+i*9+j*3+3]] for j in range(3)] for i in range(a[0])]
def reorder(tris):
 # Geometry-only splatter axis and geometric center, positive-first in-place partition.
 # Opcode HybridModel; original Cooking RVA29164 sets leaf limit8.
 centers=[[f(f(f(t[0][a]+t[1][a])+t[2][a])*f(1/3)) for a in range(3)] for t in tris]
 indices=list(range(len(tris))); nodes=[]
 def rec(lo,hi):
  n=hi-lo
  if n<=8:return
  means=[0.,0.,0.]
  for i in indices[lo:hi]:
   for a in range(3):means[a]=f(means[a]+centers[i][a])
  means=[f(v/f(n)) for v in means];var=[0.,0.,0.]
  for i in indices[lo:hi]:
   for a in range(3):
    d=f(centers[i][a]-means[a]);var[a]=f(var[a]+f(d*d))
  var=[f(v/f(n-1)) for v in var]
  axis=max(range(3),key=lambda a:var[a]); split=0.
  for i in indices[lo:hi]:
   for v in tris[i]:split=f(split+v[axis])
  split=f(split/f(n*3));mid=lo
  for i in range(lo,hi):
   if centers[indices[i]][axis]>split:indices[i],indices[mid]=indices[mid],indices[i];mid+=1
  nodes.append(dict(lo=lo,hi=hi,axis=axis,split=split,mid=mid))
  if mid==lo or mid==hi:return
  rec(lo,mid);rec(mid,hi)
 rec(0,len(indices));return indices,nodes
report=[]
for label,path,meta in [('baseline32','work/cooking-order-v159/raw32.words','work/cooking-order-v159/evidence/native-metadata/track0-run0-metadata.jsonl.gz'),('next52','work/cooking-order-v159/raw52.words','work/cooking-order-v159/evidence/native-metadata/track64-run0-metadata.jsonl.gz')]:
 tris=load(path); raw=[tuple(struct.unpack('<9I',struct.pack('<9f',*sum(t,[])))) for t in tris];md=[json.loads(s) for s in gzip.open(meta,'rt')];native=[raw.index(tuple(x['vertices'])) for x in md]
 order,nodes=reorder(tris);diff=[i for i,(a,b) in enumerate(zip(order,native)) if a!=b]
 result=dict(mesh=label,raw_to_cooked_algorithm_order=order,native_order=native,differing_positions=diff,nodes=nodes);report.append(result);print(label,'different',len(diff),'candidate',order)
(p/'prototype-order-proof.json').write_text(json.dumps(report,indent=2)+'\n')
