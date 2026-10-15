from pathlib import Path
import json,hashlib,shutil
p=Path(__file__).resolve().parent;a=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');rel='src/Rock3dEngine/header/physics/SourceMeshManifold.h';target=a/rel
expected=next(e['after_sha256'] for e in json.load(open('work/contact-reference-v156/overlay.json')) if e['path']==rel);assert hashlib.sha256(target.read_bytes()).hexdigest()==expected
s=target.read_text()
def replace(a,b):
 global s
 assert s.count(a)==1,a;s=s.replace(a,b)
needle='inline float z(PxVec3 src,const Cm::Matrix34& mat){'
replace(needle,'''// Original polygon planes use the rounded projection basis axis.
// Core225d09..225d37 and22644d..226473 accumulate Z,Y,X.
inline float planeHeight(PxVec3 axis,PxVec3 vertex){return add(add(mul(axis.z,vertex.z),mul(axis.y,vertex.y)),mul(axis.x,vertex.x));}
'''+needle)
needle='\tconst PxVec3 n = flipNormal ? -worldSepAxis : worldSepAxis;'
replace(needle,needle+'''
    const char* polygonPlaneFlagV158=std::getenv("RRR3D_EXPERIMENT_NATIVE_POLYGON_PLANE_V158");
    const bool polygonPlaneV158=nativeEdge && polygonPlaneFlagV158 && std::strcmp(polygonPlaneFlagV158,"1")==0;
''')
needle='\t\t\tconst float ld1 = -localPlane1.d;';assert needle in s
s=s.replace(needle,'\t\t\tfloat ld1 = -localPlane1.d;',1);line='\t\t\tfloat ld1 = -localPlane1.d;';i=s.index('\n',s.index(line));s=s[:i]+'''\n            if(polygonPlaneV158)ld1=nativeprojection::planeHeight(rotT1.column2,vertices1[indices1[0]]);'''+s[i:]
needle='\t\tconst float ld0 = vertexPlane?nativeedge::dot(localPlane0.n,vertices0[indices0[0]]):-localPlane0.d;';assert needle in s
s=s.replace(needle,'\t\tfloat ld0 = vertexPlane?nativeedge::dot(localPlane0.n,vertices0[indices0[0]]):-localPlane0.d;',1);line='\t\tfloat ld0 = vertexPlane?nativeedge::dot(localPlane0.n,vertices0[indices0[0]]):-localPlane0.d;';i=s.index('\n',s.index(line));s=s[:i]+'''\n        if(polygonPlaneV158)ld0=nativeprojection::planeHeight(rotT0.column2,vertices0[indices0[0]]);'''+s[i:]
b=p/'before/source'/rel;f=p/'after/source'/rel;b.parent.mkdir(parents=True,exist_ok=True);f.parent.mkdir(parents=True,exist_ok=True);assert not b.exists();shutil.copy2(target,b);f.write_text(s);target.write_text(s);(p/'overlay.json').write_text(json.dumps([dict(group='source',path=rel,before_sha256=hashlib.sha256(b.read_bytes()).hexdigest(),after_sha256=hashlib.sha256(f.read_bytes()).hexdigest())],indent=2)+'\n');print('Applied default-off projection basis plane height correction')
