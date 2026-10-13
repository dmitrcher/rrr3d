from pathlib import Path
import json,struct,gzip,hashlib
root=Path.cwd();work=root/'work/box-face-v97';inc=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics');p=inc/'SourceMeshManifold.h';raw=p.read_bytes();(work/'before').mkdir(exist_ok=True);assert (work/'before/SourceMeshManifold.h').read_bytes()==raw
helper='''// Native PhysXCore box polygon table at RVA 0x312774. These are geometric
// corner indices for all six faces, independent of fixture, frame or pose.
namespace nativeboxfacesv97 {
inline bool enabled() { const char* flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_BOX_FACE_VERTICES_V97");return flag && std::strcmp(flag,"1")==0; }
inline std::array<PxVec3,4> vertices(PxVec3 half,int axis,float sign) {
 const PxU8 faces[6][4]={{0,1,2,3},{1,5,6,2},{5,4,7,6},{4,0,3,7},{3,2,6,7},{4,5,1,0}};
 const int corners[8][3]={{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}};
 const int face=axis==0?(sign>0?1:3):axis==1?(sign>0?4:5):(sign>0?2:0);
 std::array<PxVec3,4> result;
 for(unsigned i=0;i<4;++i)for(unsigned j=0;j<3;++j)result[i][j]=corners[faces[face][i]][j]<0?-half[j]:half[j];
 return result;
}
}

'''
s=raw.decode();needle='namespace boxtrianglesatv60 {';assert s.count(needle)==1;s=s.replace(needle,helper+needle)
for end in [';verts[i][b]=signs[j][1]*half[b];}']:
 assert s.count(end)==2;s=s.replace(end,end+'\n if(nativeboxfacesv97::enabled())verts=nativeboxfacesv97::vertices(half,axis,sign);')
(work/'SourceMeshManifoldCandidate.h').write_text(s);(work/'after').mkdir(exist_ok=True);(work/'after/SourceMeshManifold.h').write_text(s)
f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0];target=bytes([0,1,2,3,1,5,6,2,5,4,7,6,4,0,3,7,3,2,6,7,4,5,1,0]);fixtures=[];full=None
for directory in ['native-polygons','native-polygons-upper','native-polygons-upper179']:
 for path in sorted((root/'outputs/box-face-v97'/directory).glob('*-private.jsonl.gz')):
  for r in map(json.loads,gzip.open(path,'rt')):
   if r.get('function')!='polygon-input-entry':continue
   neigh=r['polygon1_index_global_neighborhood'];b=struct.pack('<'+'I'*len(neigh['words']),*neigh['words']);off=b.find(target);assert off>=0;core=r['eip']-1-r['offset'];assert neigh['address']+off-core==0x312774
   values=list(map(f,r['polygon1_vertices']['words']));selected=[values[i*3:i*3+3] for i in r['polygon1']['indices']];plane=list(map(f,r['polygon_plane1']['words']));axis=max(range(3),key=lambda i:abs(plane[i]));sign=plane[axis];half=[max(abs(v[i]) for v in selected) for i in range(3)]
   fixtures.append({'capture':str(path.relative_to(root)),'physical_tick':r['sample_tick_v71'],'half':half,'axis':axis,'sign':sign,'selected':selected,'indices':r['polygon1']['indices'],'table_rva':0x312774})
   if len(values)==24:full=[values[i:i+3] for i in range(0,24,3)]
assert full is not None
for row in range(6):
 selected=[full[i] for i in target[row*4:row*4+4]];axis,sign=[(2,-1),(0,1),(2,1),(0,-1),(1,1),(1,-1)][row];fixtures.append({'scope':'Actual Native full corner array bound to actual global face index row; two other face paths not dynamically selected in these captures','axis':axis,'sign':sign,'half':[max(abs(v[i]) for v in full) for i in range(3)],'selected':selected,'indices':list(target[row*4:row*4+4])})
(root/'outputs/box-face-v97/native-face-vertex-fixtures.json').write_text(json.dumps({'scope':'10 actual Native polygon calls plus six actual static face table rows bound to actual eight-corner array. No simulation outputs used by runtime.','fixtures':fixtures,'full_native_corners':full,'native_face_table':list(target),'goal_complete':False},indent=2)+'\n')
(work/'source-manifest.json').write_text(json.dumps({'files':[{'path':'src/Rock3dEngine/header/physics/SourceMeshManifold.h','before_sha256':hashlib.sha256(raw).hexdigest(),'after_sha256':hashlib.sha256(s.encode()).hexdigest()}],'flag':'RRR3D_EXPERIMENT_NATIVE_BOX_FACE_VERTICES_V97','default_enabled':False,'goal_complete':False},indent=2)+'\n')
print('Prepared guarded candidate; fixtures',len(fixtures))
