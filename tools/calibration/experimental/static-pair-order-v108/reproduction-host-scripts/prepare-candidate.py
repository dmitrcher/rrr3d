from pathlib import Path
import hashlib,json
r=Path.cwd();w=r/'work/static-pair-order-v108';source=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=source/'build/macos-arm64-m10/_deps/rrr3d_jolt-src';files={};locations={}
for location,base,name in [('adapter',source,'src/Rock3dEngine/header/physics/NativeContactNodesV90.h'),('adapter',source,'src/Rock3dEngine/header/physics/NativeContactOrderV79.h'),('adapter',source,'src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp'),('sdk',sdk,'Jolt/Physics/Vehicle/VehicleConstraint.h'),('sdk',sdk,'Jolt/Physics/Vehicle/VehicleConstraint.cpp')]:
 p=base/name;files[name]=p.read_text();locations[name]=(location,p);b=w/'before'/location/name;b.parent.mkdir(parents=True,exist_ok=True);assert not b.exists();b.write_bytes(p.read_bytes())
def change(name,a,b):assert files[name].count(a)==1,(name,a);files[name]=files[name].replace(a,b)
node='src/Rock3dEngine/header/physics/NativeContactNodesV90.h';scene='src/Rock3dEngine/header/physics/NativeContactOrderV79.h';adapter='src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp';h='Jolt/Physics/Vehicle/VehicleConstraint.h';cpp='Jolt/Physics/Vehicle/VehicleConstraint.cpp'
change(node,'    unsigned slot(unsigned a,unsigned b) const','''    std::vector<Pair> removedPairsV108() const {
        std::vector<Pair> result;
        for(unsigned bit:removed_) {
            const auto& node=nodes_[bit/4];result.emplace_back(node.owner,node.neighbor[bit%4]);
        }
        return result;
    }
    unsigned slot(unsigned a,unsigned b) const''')
change(scene,'    bool recycleNodesV90_=false;','''    bool recycleNodesV90_=false;
    bool trackPairEventsV108_=false;
    std::vector<std::pair<unsigned,unsigned>> createdPairsV108_,removedPairsV108_;''')
change(scene,'        if(recycleNodesV90_) {for(const auto &entry:poolV90_.publish()){auto it=pairs.find(key(entry.first,entry.second));if(it!=pairs.end() && !it->second.serial)it->second.serial=++counter;}return;}','''        if(recycleNodesV90_) {
            if(trackPairEventsV108_)removedPairsV108_=poolV90_.removedPairsV108();
            const auto entries=poolV90_.publish();
            if(trackPairEventsV108_)createdPairsV108_=entries;
            for(const auto& entry:entries){auto it=pairs.find(key(entry.first,entry.second));if(it!=pairs.end() && !it->second.serial)it->second.serial=++counter;}
            return;
        }''')
change(scene,'    bool initialized() const { return !shapes.empty(); }','''    bool initialized() const { return !shapes.empty(); }
    void trackPairEventsV108(bool value) {trackPairEventsV108_=value;if(!value){createdPairsV108_.clear();removedPairsV108_.clear();}}
    const auto& createdPairsV108() const {return createdPairsV108_;}
    const auto& removedPairsV108() const {return removedPairsV108_;}''')
change(scene,'counter=0; }','counter=0;createdPairsV108_.clear();removedPairsV108_.clear(); }')
change(h,'    uint64 mSourceTireObjectCounterV106 = 0;','    uint64 mSourceTireObjectCounterV106 = 0;\n    Array<uint64> mSourceCallbackDispatchOrderV108;')
change(h,'    const Array<uint64>& GetSourceTireObjectOrderV106()','    void SetSourceCallbackDispatchOrderV108(const Array<uint64>& order) {mSourceCallbackDispatchOrderV108=order;}\n    const Array<uint64>& GetSourceTireObjectOrderV106()')
needle='''    if(creationOrder)for(uint index:creationIndicesV79) {'''
change(cpp,needle,'''    const char* dispatchFlagV108=std::getenv("RRR3D_EXPERIMENT_NATIVE_STATIC_PAIR_DISPATCH_V108");
    bool dispatchV108=callbackQueueV105 && dispatchFlagV108 && std::strcmp(dispatchFlagV108,"1")==0
        && mSourceCallbackDispatchOrderV108.size()==mWheels.size();
    if(dispatchV108)for(uint index=0;index<mWheels.size();++index)
        if(mWheels[index]->HasContact() && !mSourceCallbackDispatchOrderV108[index])dispatchV108=false;
    if(dispatchV108)std::stable_sort(creationIndicesV79.begin(),creationIndicesV79.end(),[&](uint a,uint b) {
        return mSourceCallbackDispatchOrderV108[a]<mSourceCallbackDispatchOrderV108[b];
    });
    const char* dispatchTraceV108=std::getenv("RRR3D_CALLBACK_DISPATCH_TRACE_FILE_V108");
    if(dispatchTraceV108 && *dispatchTraceV108 && dispatchFlagV108 && std::strcmp(dispatchFlagV108,"1")==0)
        if(FILE* file=std::fopen(dispatchTraceV108,"a")) {
            std::fprintf(file,"{\\"sdk_counter\\":%u,\\"supported\\":%s,\\"dispatch_ranks\\":[",GetSourceSimulationCounterV59(),dispatchV108?"true":"false");
            for(uint i=0;i<mSourceCallbackDispatchOrderV108.size();++i)std::fprintf(file,"%s%llu",i?",":"",static_cast<unsigned long long>(mSourceCallbackDispatchOrderV108[i]));
            std::fprintf(file,"],\\"creation_visits\\":[");
            for(uint i=0;i<creationIndicesV79.size();++i)std::fprintf(file,"%s%u",i?",":"",creationIndicesV79[i]);
            std::fprintf(file,"]}\\n");std::fclose(file);
        }
    if(creationOrder)for(uint index:creationIndicesV79) {''')
change(adapter,'#include "physics/NativeContactOrderV79.h"','#include "physics/NativeContactOrderV79.h"\n#include "physics/NativeContactManagerOrderV108.h"')
change(adapter,'        sourceContactOrderV79_.clear();','        sourceContactOrderV79_.clear();\n        sourceWheelManagerOrderV108_.clear();')
change(adapter,'        sourceContactOrderV79_.useRecycledNodesV90(sourceStaticGroundOrderV90_);','''        sourceStaticPairOrderV108_=motorrock::tirev55::enabled("RRR3D_EXPERIMENT_NATIVE_STATIC_PAIR_DISPATCH_V108")
            && motorrock::tirev55::enabled("RRR3D_EXPERIMENT_NATIVE_STATIC_CONTACT_NODES_V90")
            && description_.collisionMeshes.size()==1 && vehicles_.size()==1 && !decorations_.empty()
            && description_.collisionMeshes.front().surface==CollisionSurface::TrackPlane;
        if(sourceStaticPairOrderV108_)for(const auto& decoration:description_.decorations)
            if(!decoration.hasBodyShape || decoration.dynamic || !decoration.childShapes.empty() || !decoration.meshes.empty())sourceStaticPairOrderV108_=false;
        sourceContactOrderV79_.useRecycledNodesV90(sourceStaticGroundOrderV90_ || sourceStaticPairOrderV108_);
        sourceContactOrderV79_.trackPairEventsV108(sourceStaticPairOrderV108_);
        sourceStaticBodyShapesV108_.clear();''')
change(adapter,'        if(sourceStaticGroundOrderV90_) {','        if(sourceStaticGroundOrderV90_ || sourceStaticPairOrderV108_) {')
change(adapter,'        sourceContactBodyShapesV79_.resize(vehicles_.size());','''        const auto addStaticBoxesV108=[&]() {
            if(!sourceStaticPairOrderV108_)return;
            for(const auto& decoration:decorations_) {
                JPH::BodyLockRead lock(system_.GetBodyLockInterface(),decoration.shapeBody);
                if(!lock.Succeeded())throw std::runtime_error("Native static manager order: missing box");
                motorrock::wheelboxrayv59::Box box;
                if(!contactListener_.wheelSourceBoxV59(lock.GetBody(),box))throw std::runtime_error("Native static manager order: unsupported static box");
                order::Shape shape;shape.actor=0;shape.enabled=lock.GetBody().IsInBroadPhase();
                for(unsigned axis=0;axis<3;++axis) {
                    const float extent=wr::add(wr::add(wr::mul(std::abs(box.columns[0][axis]),box.half[0]),wr::mul(std::abs(box.columns[1][axis]),box.half[1])),wr::mul(std::abs(box.columns[2][axis]),box.half[2]));
                    shape.bounds[0][axis]=wr::sub(box.position[axis],extent);shape.bounds[1][axis]=wr::add(box.position[axis],extent);
                }
                sourceStaticBodyShapesV108_[decoration.shapeBody.GetIndexAndSequenceNumber()]=static_cast<unsigned>(shapes.size()+1);
                shapes.push_back(shape);
            }
        };
        if(!description_.obstaclesAfterVehicles)addStaticBoxesV108();
        sourceContactBodyShapesV79_.resize(vehicles_.size());''')
change(adapter,'        const bool initializing=!sourceContactOrderV79_.initialized();','''        if(description_.obstaclesAfterVehicles)addStaticBoxesV108();
        const bool initializing=!sourceContactOrderV79_.initialized();''')
change(adapter,'        sourceContactOrderV79_.update(shapes);','''        sourceContactOrderV79_.update(shapes);
        if(sourceStaticPairOrderV108_) {
            std::vector<std::pair<unsigned,unsigned>> added,removed;
            const auto wheelPair=[&](const auto& pair) {return shapes.at(pair.first-1).wheel || shapes.at(pair.second-1).wheel;};
            for(const auto& pair:sourceContactOrderV79_.createdPairsV108())if(wheelPair(pair))added.push_back(pair);
            for(const auto& pair:sourceContactOrderV79_.removedPairsV108())if(wheelPair(pair))removed.push_back(pair);
            sourceWheelManagerOrderV108_.update(added,removed);
        } else sourceWheelManagerOrderV108_.clear();''')
change(adapter,'                    constraint.SetSourceContactCreationOrderV79(ranks,sourceStaticGroundOrderV90_);','''                    constraint.SetSourceContactCreationOrderV79(ranks,sourceStaticGroundOrderV90_);
                    JPH::Array<JPH::uint64> dispatch;
                    if(sourceStaticPairOrderV108_) {
                        dispatch.resize(constraint.GetWheels().size(),0);
                        for(JPH::uint wheel=0;wheel<dispatch.size();++wheel) {
                            const auto* current=constraint.GetWheel(wheel);if(!current->HasContact())continue;
                            unsigned support=0;
                            if(!trackBodies_.empty() && current->GetContactBodyID()==trackBodies_.front())support=1;
                            else {const auto it=sourceStaticBodyShapesV108_.find(current->GetContactBodyID().GetIndexAndSequenceNumber());if(it!=sourceStaticBodyShapesV108_.end())support=it->second;}
                            if(support)dispatch[wheel]=sourceWheelManagerOrderV108_.order(sourceContactBodyShapesV79_[vehicleIndexValue]+1+wheel,support);
                        }
                    }
                    constraint.SetSourceCallbackDispatchOrderV108(dispatch);''')
change(adapter,'    motorrock::contactorderv79::Scene sourceContactOrderV79_;','''    bool sourceStaticPairOrderV108_=false;
    std::map<JPH::uint32,unsigned> sourceStaticBodyShapesV108_;
    motorrock::contactmanagerorderv108::List sourceWheelManagerOrderV108_;
    motorrock::contactorderv79::Scene sourceContactOrderV79_;''')
new='src/Rock3dEngine/header/physics/NativeContactManagerOrderV108.h';np=source/new;assert not np.exists();files[new]='''#pragma once
#include <algorithm>
#include <utility>
#include <vector>
// Original wheel contact-manager processor uses dense slots: removing a
// manager moves the last registered wheel manager into its vacant position.
// Broadphase deletion is retired at the next processing step. Body contact
// managers have a separate processor; their pairs never occupy these slots.
namespace motorrock::contactmanagerorderv108 {
class List {
public:
    using Pair=std::pair<unsigned,unsigned>;
private:
    std::vector<Pair> active_,pending_;
    static Pair key(Pair p){return {std::min(p.first,p.second),std::max(p.first,p.second)};}
public:
    void clear(){active_.clear();pending_.clear();}
    void update(const std::vector<Pair>& added,const std::vector<Pair>& removed) {
        for(Pair pair:pending_) {auto it=std::find(active_.begin(),active_.end(),key(pair));if(it!=active_.end()){*it=active_.back();active_.pop_back();}}
        pending_.clear();
        for(Pair pair:added)if(std::find(active_.begin(),active_.end(),key(pair))==active_.end())active_.push_back(key(pair));
        pending_=removed;
    }
    unsigned order(unsigned a,unsigned b) const {auto it=std::find(active_.begin(),active_.end(),key({a,b}));return it==active_.end()?0:static_cast<unsigned>(it-active_.begin()+1);}
    const auto& entries() const {return active_;}
};
}
''';locations[new]=('adapter',np)
records=[]
for name,s in files.items():
 loc,p=locations[name];p.write_text(s);b=w/'before'/loc/name;records.append({'location':loc,'path':name,'before_sha256':hashlib.sha256(b.read_bytes()).hexdigest() if b.exists() else None,'after_sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
(w/'source-manifest.json').write_text(json.dumps({'scope':'Default-off current static wheel contact-manager dispatch slots, separate from persistent object/callback ages and pair birth. Original deferred dense removal/last-slot replacement observed at curb203/220/224. One terrain mesh, one vehicle, static box decorations with exact creation ordering; other scene classes retain prior behavior. No coefficients/frame-case rules/oracle state. Private VehicleConstraint layout changes; rebuild all consumers.','files':records,'goal_complete':False},indent=2)+'\n')
