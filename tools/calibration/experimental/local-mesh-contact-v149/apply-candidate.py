from pathlib import Path
import shutil,hashlib
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');cpp=active/'src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp';inc=active/'src/Rock3dEngine/header/physics';out=Path(__file__).resolve().parent
assert hashlib.sha256(cpp.read_bytes()).hexdigest()=='776ed3ef3ec9632d5ffcc9822db31b1b30c0a100a8ac021d7d4fa1cdd37ac787'
(out/'before').mkdir(exist_ok=True);shutil.copy2(cpp,out/'before/JoltVehiclePhysics.cpp');shutil.copy2(inc/'SourceMeshManifold.h',out/'before/SourceMeshManifold.h')
shutil.copy2(out/'include/NativeMeshPartsV149.h',inc/'NativeMeshPartsV149.h');shutil.copy2(out.parent/'relative-contact-v147/include/NativeRelativeContactV147.h',inc/'NativeRelativeContactV147.h')
s=(inc/'SourceMeshManifold.h').read_text();s=s.replace('PxReal sepShift, bool nativeEdge=false)','PxReal sepShift, bool nativeEdge=false,bool vertexPlane=false)');assert 'bool vertexPlane=false' in s
s=s.replace('const float ld0 = -localPlane0.d;','const float ld0 = vertexPlane?nativeedge::dot(localPlane0.n,vertices0[indices0[0]]):-localPlane0.d;')
a=s.index('inline void faceContacts(');prefix=s[:a];tail=s[a:];tail=tail.replace('const Cm::Matrix34* nativeInverse=nullptr)','const Cm::Matrix34* nativeInverse=nullptr,const PxPlane* patchPlane=nullptr)',1).replace('const PxPlane plane=trianglePlane(triangle);','const PxPlane plane=patchPlane?*patchPlane:trianglePlane(triangle);',1)
tail=tail.replace('const auto toBox=box.getInverseRT();','const auto toBox=nativeInverse?*nativeInverse:box.getInverseRT();')
tail=tail.replace('PxVec3(0),0,nativeEdge);','PxVec3(0),0,nativeEdge,patchPlane!=nullptr);');s=prefix+tail;(inc/'SourceMeshManifold.h').write_text(s)
s=cpp.read_text().replace('#include "physics/NativeHybridRayV142.h"','#include "physics/NativeHybridRayV142.h"\n#include "physics/NativeRelativeContactV147.h"\n#include "physics/NativeMeshPartsV149.h"')
s=s.replace('unsigned convexEdges=7; JPH::SubShapeID subShapeV122;','unsigned convexEdges=7,flatPartV149=0; JPH::SubShapeID subShapeV122;')
needle='        std::scoped_lock lock(mutex_);\n        shape.SetUserData(sourceMeshMarker | sourceMeshes_.size());'
insert='''        if(placedPose && std::getenv("RRR3D_EXPERIMENT_NATIVE_LOCAL_CONTACT_V149")) {
            std::vector<motorrock::meshpartsv149::Triangle> input;
            for(const auto& t:record->triangles)input.push_back(t.points);
            const auto parts=motorrock::meshpartsv149::build(input);
            for(unsigned i=0;i<record->triangles.size();++i)record->triangles[i].flatPartV149=parts.flat[i];
        }
'''
assert needle in s;s=s.replace(needle,insert+needle)
needle='        sourcemesh::PxVec3 lo(FLT_MAX),hi(-FLT_MAX);'
a=s.index(needle,s.index('    bool meshManifold('));insert='''        const char* localFlagV149=std::getenv("RRR3D_EXPERIMENT_NATIVE_LOCAL_CONTACT_V149");
        const bool localV149=mesh->placedV126 && full.hasPhysicalPose && localFlagV149 && std::strcmp(localFlagV149,"1")==0;
        auto geometryBoxV149=box;
        if(localV149) {
            namespace rc=motorrock::relativecontactv147;
            const rc::Pose body{{{box.p.x,box.p.y,box.p.z}},full.physicalPose};
            const rc::Pose frame{{{mesh->poseV126.position[0],mesh->poseV126.position[1],mesh->poseV126.position[2]}},mesh->poseV126.quaternion};
            const auto forward=rc::relative(frame,body),inverse=rc::relative(body,frame);
            const auto convert=[](const rc::Transform& t){return sourcemesh::Cm::Matrix34{
                {t.rotation.a[0][0],t.rotation.a[1][0],t.rotation.a[2][0]},
                {t.rotation.a[0][1],t.rotation.a[1][1],t.rotation.a[2][1]},
                {t.rotation.a[0][2],t.rotation.a[1][2],t.rotation.a[2][2]},
                {t.position[0],t.position[1],t.position[2]}};};
            geometryBoxV149=convert(forward);nativeMeshInverse=convert(inverse);
        }
        std::map<unsigned,sourcemesh::PxPlane> patchPlanesV149;
''';s=s[:a]+insert+s[a:]
a=s.index('    bool meshManifold(');b=s.index('        if(contacts.count>16)',a);part=s[a:b]
part=part.replace('const auto point=box.transform({','const auto point=geometryBoxV149.transform({')
part=part.replace('if(mesh->placedV126){const auto v=', 'if(localV149)verts[j]=p;\n                else if(mesh->placedV126){const auto v=')
needle='            sourcemesh::faceContacts(contacts,half,box,verts,triangleIndex,nativeEdge,t.convexEdges,useNativeMeshInverse?&nativeMeshInverse:nullptr);'
replacement='''            const sourcemesh::PxPlane* patchV149=nullptr;
            if(localV149) {
                const auto plane=sourcemesh::trianglePlane(verts);
                auto found=patchPlanesV149.find(t.flatPartV149);
                if(found==patchPlanesV149.end())found=patchPlanesV149.emplace(t.flatPartV149,plane).first;
                patchV149=&found->second;
            }
            sourcemesh::faceContacts(contacts,half,geometryBoxV149,verts,triangleIndex,nativeEdge,t.convexEdges,(useNativeMeshInverse||localV149)?&nativeMeshInverse:nullptr,patchV149);
            if(localV149)for(unsigned i=before;i<contacts.count;++i) {
                auto& c=contacts.contacts[i];
                const auto p=motorrock::placedmeshv126::point(mesh->poseV126,{{c.point.x,c.point.y,c.point.z}});
                const auto n=motorrock::placedmeshv126::direction(mesh->poseV126,{{c.normal.x,c.normal.y,c.normal.z}});
                c.point={p[0],p[1],p[2]};c.normal={n[0],n[1],n[2]};
            }
'''
assert needle in part;part=part.replace(needle,replacement)
needle='                namespace cf=motorrock::contactframe;\n                cf::Vector e1,e2;'
replacement='''                namespace cf=motorrock::contactframe;
                auto publicVertsV149=verts;
                if(localV149)for(auto& v:publicVertsV149){const auto p=motorrock::placedmeshv126::point(mesh->poseV126,{{v.x,v.y,v.z}});v={p[0],p[1],p[2]};}
                cf::Vector e1,e2;'''
assert needle in part;part=part.replace(needle,replacement).replace('double(verts[1][axis])-verts[0][axis]','double(publicVertsV149[1][axis])-publicVertsV149[0][axis]').replace('double(verts[2][axis])-verts[0][axis]','double(publicVertsV149[2][axis])-publicVertsV149[0][axis]')
s=s[:a]+part+s[b:];cpp.write_text(s)
print('Applied experimental local mesh contact calculation; existing world material frame retained')
