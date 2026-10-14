from pathlib import Path
import hashlib,json,struct,math
root=Path.cwd();base=root/'outputs/real-map-v124/repeats';reports={}
def inspect(p):
 lines=p.read_text().splitlines();assert lines[0]=='MOTORROCK_WORLD_V124';words=list(map(int,' '.join(lines[1:]).split()));offset=0
 def take(n):
  nonlocal offset
  out=words[offset:offset+n];offset+=n;assert len(out)==n;return out
 def floats(values):
  for u in values:assert 0<=u<=0xffffffff and math.isfinite(struct.unpack('<f',struct.pack('<I',u))[0])
 floats(take(7));vehicle=take(40);floats([v for i,v in enumerate(vehicle) if i not in [15,16,19,37,38,39]]);assert all(vehicle[i] in [0,1] for i in [15,16,19,37,38,39]);wheelcount=take(1)[0];assert wheelcount==4
 for i in range(wheelcount):
  w=take(23);floats(w[:10]+w[13:]);assert w[11] in [0,1] and w[12] in [0,1]
 meshes=take(1)[0];vertices=triangles=0
 for i in range(meshes):
  meta=take(14);assert meta[0]<=4;floats(meta[2:12]);nv,ni=meta[12:];assert ni%3==0;floats(take(nv*3));indices=take(ni);assert all(k<nv for k in indices);vertices+=nv;triangles+=ni//3
 decorations=take(1)[0];childshapes=0
 for i in range(decorations):
  d=take(27);floats(d[:23]);assert all(x in [0,1] for x in d[23:26]);childshapes+=d[26]
  for j in range(d[26]):floats(take(11))
 spawncount=take(1)[0];assert spawncount==1;floats(take(6*spawncount));assert offset==len(words)
 return {'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'bytes':p.stat().st_size,'words':len(words),'vertices':vertices,'triangles':triangles,'meshes':meshes,'decorations':decorations,'child_shapes':childshapes,'vehicles':spawncount,'wheels':wheelcount}
for track in [0,16,48,64]:
 captures=[inspect(base/f'track{track}-run{i}.words') for i in [0,1,2]];assert captures[0]==captures[1]==captures[2];reports[str(track)]=captures[0]
(base/'fixture-verification.json').write_text(json.dumps({'scope':'Actual source world descriptions exported at gameplay load boundary; finite rawF32 words, every index/count valid and entire wire format consumed, allgeometry/body/wheelparameters same at seeds42/42/999. Macrepeatability only, Native replay reader/compare pending.','tracks':reports,'goal_complete':False},indent=2)+'\n');print(reports)
