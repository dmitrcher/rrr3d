from pathlib import Path
import hashlib, json

root=Path.cwd()
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d')
work=root/'work/creation-order-v99/mac-candidate'
work.mkdir(exist_ok=False)
files={}
def replace(s,old,new):
    assert s.count(old)==1, old[:160]
    return s.replace(old,new)

path='src/Rock3dEngine/header/physics/OriginalVehiclePhysics.h'
s=(active/path).read_text(); files[path]=[s,replace(s,'    float gravity = -20.0F;\n};','    float gravity = -20.0F;\n    // Explicit scene construction contract for isolated reference probes.\n    // Normal races keep their existing track/decorations/vehicles order.\n    bool obstaclesAfterVehicles = false;\n};')]

path='tools/calibration/mac.cpp'
s=(active/path).read_text()
new=replace(s,' phys::WorldDescription d;d.gravity=', ' phys::WorldDescription d;\n const char* creationOrder=std::getenv("RRR3D_EXPERIMENT_SCENE_CREATION_ORDER_V99");\n d.obstaclesAfterVehicles=creationOrder && std::strcmp(creationOrder,"1")==0;\n d.gravity=')
files[path]=[s,new]

path='src/Rock3dEngine/header/physics/NativeContactCache.h'
s=(active/path).read_text()
new=replace(s,'bool persistentFrame=false) {','bool persistentFrame=false,unsigned dynamicSlot=0) {\n  // The ordered pair is the canonical Native pair; either body may be dynamic.\n  // Static world points and body-1 local arithmetic must keep their slots.')
new=replace(new,'levers->push_back(solverLever(a,points[reuse?matched[i]:i].on0))','levers->push_back(dynamicSlot==0?solverLever(a,points[reuse?matched[i]:i].on0):solverLever(b,points[reuse?matched[i]:i].on1))')
files[path]=[s,new]

path='src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp'
s=(active/path).read_text()
assert hashlib.sha256(s.encode()).hexdigest()=='66f1d17e43d8089b0b4c4db427a8b6b33f20a7e70cb553937d032517941abc21'
new=replace(s,'        if(!contacts.count)return true;\n        const char* materialFlag=', '''        if(!contacts.count)return true;
        const char* roleFlagV99=std::getenv("RRR3D_EXPERIMENT_NATIVE_CONTACT_ROLES_V99");
        const bool orderedRolesV99=roleFlagV99 && std::strcmp(roleFlagV99,"1")==0;
        // Measured Native pairs put the later-created actor in slot 0.
        // This default-off investigation uses scene creation order, not SAT's
        // reference polygon, as the source of canonical body roles.
        const bool staticFirstV99=orderedRolesV99 && surface.GetID().GetIndex()>car.GetID().GetIndex();
        const char* materialFlag=''')
new=replace(new,'const bool staticFirst=shapeOrderFrame?surface.GetID().GetIndex()>car.GetID().GetIndex():nativeHullReference;\n                nativeCandidate=motorrock::framev36::compute(callbackActor,actorQuaternion,plane,callbackPlane,staticFirst);', '''const bool staticFirst=orderedRolesV99?staticFirstV99:(shapeOrderFrame?surface.GetID().GetIndex()>car.GetID().GetIndex():nativeHullReference);
                auto canonicalPlane=plane;
                if(staticFirstV99)for(unsigned axis=0;axis<3;++axis)canonicalPlane[axis]=-canonicalPlane[axis];
                nativeCandidate=motorrock::framev36::compute(callbackActor,actorQuaternion,canonicalPlane,callbackPlane,staticFirst);''')
new=replace(new,'const auto& n=contacts.contacts[0].normal;const cf::Vector normal={{n.x,n.y,n.z}};', '''const auto& n=contacts.contacts[0].normal;cf::Vector normal={{n.x,n.y,n.z}};
            if(staticFirstV99)for(unsigned axis=0;axis<3;++axis)normal[axis]=-normal[axis];''')
new=replace(new,'const auto errors=nativeContactCaches_[key].prepare(actor,ground,points,normal,nc::de::mul(nc::de::add(full.skin,.025F),.2F),cacheFrame,&levers,persistentCacheFrame);', '''const auto errors=nativeContactCaches_[key].prepare(staticFirstV99?ground:actor,staticFirstV99?actor:ground,points,normal,nc::de::mul(nc::de::add(full.skin,.025F),.2F),cacheFrame,&levers,persistentCacheFrame,staticFirstV99?1U:0U);''')
new=replace(new,'sample<<",\\\"frame\\\":[";for(unsigned j=0;j<4;++j)sample<<(j?",":"")<<cacheFrame[j];', '''sample<<",\\\"canonical_dynamic_slot\\\":"<<(staticFirstV99?1:0)<<",\\\"car_body_index\\\":"<<car.GetID().GetIndex()<<",\\\"surface_body_index\\\":"<<surface.GetID().GetIndex();
                sample<<",\\\"frame\\\":[";for(unsigned j=0;j<4;++j)sample<<(j?",":"")<<cacheFrame[j];''')
new=replace(new,'''        createTrack();
        createDecorationBodies();
        vehicles_.reserve(description_.spawns.size());
        for (const auto& spawn : description_.spawns)
            createVehicle(spawn);''', '''        createTrack(description_.obstaclesAfterVehicles?1U:0U);
        if(!description_.obstaclesAfterVehicles)createDecorationBodies();
        vehicles_.reserve(description_.spawns.size());
        for (const auto& spawn : description_.spawns)
            createVehicle(spawn);
        if(description_.obstaclesAfterVehicles) {
            createTrack(2U);
            createDecorationBodies();
        }''')
new=replace(new,'    void createTrack()\n','    // Phase 0 preserves normal construction. Phases 1/2 keep the same merged\n    // surfaces and decoration owners, with vehicles between plane and obstacles.\n    void createTrack(unsigned creationPhase=0U)\n')
new=replace(new,'''            if (triangles[index].empty())
                continue;
            trackBodies_.push_back''', '''            if (triangles[index].empty() || (creationPhase==1U && index!=0U) || (creationPhase==2U && index==0U))
                continue;
            trackBodies_.push_back''')
new=replace(new,'            if (decorationTriangles[index].empty())\n','            if (decorationTriangles[index].empty() || creationPhase==1U)\n')
new=replace(new,'        // Map.cpp creates a +Z NxPlaneShape', '        if(creationPhase==2U)return; // Death plane is created exactly once.\n        // Map.cpp creates a +Z NxPlaneShape')
files[path]=[s,new]

manifest=[]
for path,(before,after) in files.items():
    for state,data in [('before',before),('after',after)]:
        dest=work/state/path; dest.parent.mkdir(parents=True,exist_ok=True); dest.write_text(data)
    manifest.append({'path':path,'before_sha256':hashlib.sha256(before.encode()).hexdigest(),'after_sha256':hashlib.sha256(after.encode()).hexdigest()})
(work/'source-manifest.json').write_text(json.dumps({'scope':'Default-off V99 explicit probe scene creation order and canonical contact roles. No SDK or coefficient changes; no reference state injection. Candidate pending actual Native same-input and regression acceptance.','files':manifest,'goal_complete':False},indent=2)+'\n')
for path,(_,after) in files.items(): (active/path).write_text(after)
print('Installed 4-file default-off V99 candidate; frozen V98 unchanged',flush=True)
