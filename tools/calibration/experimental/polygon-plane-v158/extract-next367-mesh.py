from pathlib import Path
import json,struct
p=Path(__file__).resolve().parent;text=Path('outputs/real-map-v124/repeats/track64-run0.words').read_text().split();assert text.pop(0)=='MOTORROCK_WORLD_V124';ws=list(map(int,text));i=0
u=lambda w:struct.unpack('<f',struct.pack('<I',w))[0];w=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def integer():
 global i
 r=ws[i];i+=1;return r
def vals(n):return [u(integer()) for _ in range(n)]
vals(22);integer();integer();vals(2);integer();vals(17);[integer() for _ in range(3)];nw=integer();assert nw==4
for _ in range(nw):vals(10);[integer() for _ in range(3)];vals(10)
rows=list(map(json.loads,open('outputs/polygon-plane-v158/source-track64-step367/track64-physical.jsonl')));source=[r['sample'] for r in rows if r['event']=='physics.source-mesh-input'];ground=next(r['sample']['ground'][9:12] for r in rows if r['event']=='physics.native-cache-input');count=integer();matches=[]
for m in range(count):
 surface,owner=integer(),integer();pos=vals(3);scale=vals(3);quat=vals(4);nv,ni=integer(),integer();vertices=[vals(3) for _ in range(nv)];indices=[integer() for _ in range(ni)];scaled=[[u(w(v[a]*scale[a])) for a in range(3)] for v in vertices]
 if list(map(w,pos))!=list(map(w,ground)):continue
 tris=[[scaled[indices[k+j]] for j in range(3)] for k in range(0,ni,3)]
 if not all([[w(v) for v in t] for t in tris[s['triangle']]]==[[w(v) for v in t] for t in s['vertices']] for s in source):continue
 matches.append(dict(mesh=m,surface=surface,owner=owner,position=pos,quaternion=quat,scale=scale,triangles=tris))
assert len(matches)==1,[(m['mesh'],m['surface']) for m in matches];entry=matches[0];(p/'next367-raw-mesh.json').write_text(json.dumps(entry,indent=2)+'\n');(p/'next367-raw-mesh.words').write_text(str(len(entry['triangles']))+'\n'+' '.join(str(w(v)) for t in entry['triangles'] for a in t for v in a)+'\n');print('Matched world64 mesh',entry['mesh'],'triangles',len(entry['triangles']),'surface',entry['surface'])
