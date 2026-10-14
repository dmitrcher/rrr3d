from pathlib import Path
import hashlib,json
root=Path.cwd();work=root/'work/tire-lifetime-v106';source=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=source/'build/macos-arm64-m10/_deps/rrr3d_jolt-src';adapter=source/'src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp';header=source/'src/Rock3dEngine/header/physics/NativeWheelPairLifetimeV106.h'
backup=work/'before/src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp';backup.parent.mkdir(parents=True,exist_ok=True);assert not backup.exists();backup.write_bytes(adapter.read_bytes());assert not header.exists()
header.write_text('''#pragma once
#include <array>
#include <set>
#include <vector>
// Original wheel contact managers are processed even when another collider
// supplies the selected wheel hit. Their deferred destruction clears all four
// wheel objects (Native 206ef0 -> 148540), independently of public HasContact.
namespace motorrock::wheelpairv106 {
using Bounds=std::array<std::array<float,3>,2>;
inline bool overlap(const Bounds& a,const Bounds& b) {
    for(unsigned axis=0;axis<3;++axis)
        if(a[0][axis]>b[1][axis] || b[0][axis]>a[1][axis])return false;
    return true;
}
class Lifetime {
    std::vector<std::set<unsigned>> previous;
    std::vector<bool> pending;
public:
    void clear() {previous.clear();pending.clear();}
    std::vector<bool> update(const std::vector<std::set<unsigned>>& current) {
        if(previous.size()!=current.size()) {
            previous=current;pending.assign(current.size(),false);return pending;
        }
        auto reset=pending;
        for(unsigned wheel=0;wheel<current.size();++wheel) {
            pending[wheel]=false;
            for(unsigned collider:previous[wheel])if(!current[wheel].count(collider))pending[wheel]=true;
        }
        previous=current;return reset;
    }
};
}
''')
s=adapter.read_text()
def replace(old,new):
 global s
 assert s.count(old)==1,old;s=s.replace(old,new)
replace('#include "physics/NativeContactOrderV79.h"','#include "physics/NativeContactOrderV79.h"\n#include "physics/NativeWheelPairLifetimeV106.h"')
replace('        sourceContactOrderV79_.clear();','        sourceContactOrderV79_.clear();\n        sourceWheelPairLifetimesV106_.clear();')
replace('            prepareNativeContactOrderV79();','            prepareNativeContactOrderV79();\n            prepareNativeWheelPairLifetimeV106();')
replace('    void prepareNativeContactOrderV79()','''    void prepareNativeWheelPairLifetimeV106()
    {
        if(!tuning_.physxWheelCompat
           || !motorrock::tirev55::enabled("RRR3D_EXPERIMENT_NATIVE_TIRE_OBJECT_LIFETIME_V106")
           || !motorrock::tirev55::enabled("RRR3D_EXPERIMENT_NATIVE_WHEEL_PAIR_RESET_V106"))return;
        namespace life=motorrock::wheelpairv106;
        namespace wr=motorrock::wheelrayv52;
        std::vector<life::Bounds> staticBounds;
        for(const auto& mesh:description_.collisionMeshes) {
            life::Bounds bounds{};bool first=true;
            for(unsigned index:mesh.indices) {
                const auto point=transformPoint(mesh.transform,mesh.vertices.at(index));
                const float v[3]={point.x,point.y,point.z};
                for(unsigned axis=0;axis<3;++axis) {
                    if(first)bounds[0][axis]=bounds[1][axis]=v[axis];
                    else {bounds[0][axis]=std::min(bounds[0][axis],v[axis]);bounds[1][axis]=std::max(bounds[1][axis],v[axis]);}
                }
                first=false;
            }
            if(!first)staticBounds.push_back(bounds);
        }
        const auto addStaticBox=[&](JPH::BodyID id) {
            if(id.IsInvalid())return;
            JPH::BodyLockRead lock(system_.GetBodyLockInterface(),id);
            if(!lock.Succeeded() || !lock.GetBody().IsStatic())return;
            motorrock::wheelboxrayv59::Box box;
            if(!contactListener_.wheelSourceBoxV59(lock.GetBody(),box))return;
            life::Bounds bounds{};
            for(unsigned axis=0;axis<3;++axis) {
                const float extent=wr::add(wr::add(wr::mul(std::abs(box.columns[0][axis]),box.half[0]),wr::mul(std::abs(box.columns[1][axis]),box.half[1])),wr::mul(std::abs(box.columns[2][axis]),box.half[2]));
                bounds[0][axis]=wr::sub(box.position[axis],extent);bounds[1][axis]=wr::add(box.position[axis],extent);
            }
            staticBounds.push_back(bounds);
        };
        for(const auto& decoration:decorations_) {
            addStaticBox(decoration.shapeBody);
            for(auto id:decoration.childShapeBodies)addStaticBox(id);
        }
        sourceWheelPairLifetimesV106_.resize(vehicles_.size());
        for(unsigned vehicleIndex=0;vehicleIndex<vehicles_.size();++vehicleIndex) {
            auto& vehicle=vehicles_[vehicleIndex];const auto& source=vehicle.spawn.vehicle;
            JPH::BodyLockRead lock(system_.GetBodyLockInterface(),vehicle.body);
            if(!lock.Succeeded())throw std::runtime_error("Native wheel pair lifetime: missing vehicle");
            const auto& body=lock.GetBody();
            const auto q=fromJolt(body.GetRotation());const auto cm=fromJolt(JPH::Vec3(body.GetCenterOfMassPosition())),local=fromJolt(body.GetShape()->GetCenterOfMass());
            const auto actor=motorrock::quaternionposev43::origin({{q.x,q.y,q.z,q.w}},{{cm.x,cm.y,cm.z}},{{local.x,local.y,local.z}});
            std::vector<std::set<unsigned>> pairs(source.wheels.size());
            for(unsigned index=0;index<source.wheels.size();++index) {
                const auto& wheel=source.wheels[index];namespace wp=motorrock::wheelposev54;
                const auto ray=wp::ray(wp::pose(wp::compose({{actor[0],actor[1],actor[2],q.x,q.y,q.z,q.w}},{{wheel.position.x,wheel.position.y,wheel.position.z,.5F,.5F,.5F,.5F}})),wheel.radius,wheel.suspensionTravel);
                life::Bounds bounds{};
                for(unsigned axis=0;axis<3;++axis){bounds[0][axis]=std::min(ray.start[axis],ray.end[axis]);bounds[1][axis]=std::max(ray.start[axis],ray.end[axis]);}
                if(vehicle.enabled && source.wheelContactsEnabled)for(unsigned collider=0;collider<staticBounds.size();++collider)
                    if(life::overlap(bounds,staticBounds[collider]))pairs[index].insert(collider);
            }
            const auto resets=sourceWheelPairLifetimesV106_[vehicleIndex].update(pairs);
            for(unsigned index=0;index<resets.size();++index)if(resets[index])vehicle.constraint->ClearSourceWheelObjectsV106(index);
            const char* path=std::getenv("RRR3D_WHEEL_PAIR_TRACE_FILE_V106");
            if(path && *path)if(FILE* file=std::fopen(path,"a")) {
                std::fprintf(file,"{\\"sdk_counter_before_step\\":%u,\\"vehicle\\":%u,\\"resets\\":[",vehicle.constraint->GetSourceSimulationCounterV59(),vehicleIndex);
                for(unsigned i=0;i<resets.size();++i)std::fprintf(file,"%s%s",i?",":"",resets[i]?"true":"false");
                std::fprintf(file,"],\\"static_pairs\\":[");
                for(unsigned i=0;i<pairs.size();++i) {std::fprintf(file,"%s[",i?",":"");bool first=true;for(unsigned collider:pairs[i]){std::fprintf(file,"%s%u",first?"":",",collider);first=false;}std::fprintf(file,"]");}
                std::fprintf(file,"]}\\n");std::fclose(file);
            }
        }
    }

    void prepareNativeContactOrderV79()''')
replace('    motorrock::contactorderv79::Scene sourceContactOrderV79_;','    std::vector<motorrock::wheelpairv106::Lifetime> sourceWheelPairLifetimesV106_;\n    motorrock::contactorderv79::Scene sourceContactOrderV79_;')
p=sdk/'Jolt/Physics/Vehicle/VehicleConstraint.h';h=p.read_text();old='    const Array<uint64>& GetSourceTireObjectOrderV106() const { return mSourceTireObjectOrderV106; }';assert h.count(old)==1;h=h.replace(old,old+'''
    void ClearSourceWheelObjectsV106(uint index) {
        if(index<mSourceTireObjectOrderV106.size())mSourceTireObjectOrderV106[index]=0;
        if(index<mSourceSpringCreationOrderV64.size())mSourceSpringCreationOrderV64[index]=0;
        if(index<mSourceStopCreationOrderV64.size())mSourceStopCreationOrderV64[index]=0;
        if(index<mSourceSpringObjectPresentV104.size())mSourceSpringObjectPresentV104[index]=false;
    }''');p.write_text(h);adapter.write_text(s)
m=json.loads((work/'source-manifest.json').read_text());m['scope']+=' Static mesh/box broadphase pair removal defers full wheel-object clearing to next step, independent of selected hit. Dynamic contact managers and decoration meshes are not yet implemented.'
for record in m['files']:record['after_sha256']=hashlib.sha256((sdk/record['path']).read_bytes()).hexdigest()
for p in [adapter,header]:m['files'].append({'location':'source','path':str(p.relative_to(source)),'before_sha256':hashlib.sha256(backup.read_bytes()).hexdigest() if p==adapter else None,'after_sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
(work/'source-manifest.json').write_text(json.dumps(m,indent=2)+'\n')
