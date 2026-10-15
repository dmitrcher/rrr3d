from pathlib import Path
import struct,gzip,json
root=Path.cwd();p=root/'work/cooking-order-v159';checks=[]
for track,file in [(0,'work/cooking-order-v159/raw32.words'),(64,'work/cooking-order-v159/raw52.words')]:
 words=list(map(int,Path(file).read_text().split()));count=words[0];assert len(words)==1+9*count
 raw=[tuple(words[1+9*i:1+9*(i+1)]) for i in range(count)];assert len(set(raw))==count
 permutation=[]
 for rep in [0,1]:
  metadata=list(map(json.loads,gzip.open(root/f'work/cooking-order-v159/evidence/native-metadata/track{track}-run{rep}-metadata.jsonl.gz','rt')))
  assert len(metadata)==count;order=[raw.index(tuple(t['vertices'])) for t in metadata];assert sorted(order)==list(range(count))
  if permutation:assert order==permutation
  else:permutation=order
  checks.append(dict(track=track,repeat=rep,triangles=count,exact9_vertex_words=True,cooked_to_raw=order))
(p/'face-order-proof.json').write_text(json.dumps(dict(scope='Original cooking output. Separate metadata helper validated against unchanged900state histories and prior32face metadata. Raw triangles uniquely matched by all9vertex words.',checks=checks,goal_complete=False),indent=2)+'\n')
print('Four full original cooked orders uniquely bound by exact vertices')
