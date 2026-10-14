from pathlib import Path
import json,hashlib,shutil
root=Path.cwd();w=root/'work/wheel-objects-v104';active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');sdk=active/'build/macos-arm64-m10/_deps/rrr3d_jolt-src';files=[]
header='''#pragma once
// Object lifetime is distinct from active solver mass or spring coefficients.
namespace NativeWheelObjectLifetimeV104 {
inline bool springConfigured(float originalSuspensionTravel) {
    return originalSuspensionTravel > 0.0f;
}
inline bool resetActorRows(unsigned queryMode, bool springObjectPresent) {
    return queryMode == 0 && !springObjectPresent;
}
}
'''
cp=sdk/'Jolt/Physics/Vehicle/VehicleConstraint.cpp';hp=sdk/'Jolt/Physics/Vehicle/VehicleConstraint.h';ap=active/'src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp';c=cp.read_text();h=hp.read_text();a=ap.read_text()
assert hashlib.sha256(c.encode()).hexdigest()=='622f5881a2cc1abec7479217ff2e7b38caf773b1bb7415daf93ffd0cb9ae95c4'
assert hashlib.sha256(a.encode()).hexdigest()=='6bd88767ffd32d33653f62a2b13b8f1be3e9e39cc12f381ff9b5ed9a6c3c6fdb';assert 'V104' not in h and 'V104' not in a
nc=c.replace('#include <Jolt/Physics/Vehicle/VehicleConstraint.h>','#include <Jolt/Physics/Vehicle/VehicleConstraint.h>\n#include <Jolt/Physics/Vehicle/NativeWheelObjectLifetimeV104.h>')
start=nc.index('void VehicleConstraint::SetupVelocityConstraint(float inDeltaTime)');pos=nc.index('\tRMat44 body_transform = mBody->GetWorldTransform();',start)
block='''    const char* objectFlagV104=std::getenv("RRR3D_EXPERIMENT_NATIVE_WHEEL_OBJECT_LIFETIME_V104");
    const bool objectLifetimeV104=creationOrder && objectFlagV104 && std::strcmp(objectFlagV104,"1")==0
        && mSourceWheelTravelV104.size()==mWheels.size();
    if(objectLifetimeV104) {
        if(mSourceSpringObjectPresentV104.size()!=mWheels.size())
            mSourceSpringObjectPresentV104.resize(mWheels.size(),false);
        for(uint index=0;index<mWheels.size();++index) {
            const Wheel* wheel=mWheels[index];
            if(wheel->HasContact() && NativeWheelObjectLifetimeV104::resetActorRows(0,mSourceSpringObjectPresentV104[index])) {
                for(auto &order:mSourceSpringCreationOrderV64)order=0;
                for(auto &order:mSourceStopCreationOrderV64)order=0;
                for(uint other=0;other<mWheels.size();++other)mSourceSpringObjectPresentV104[other]=false;
                break;
            }
        }
    }
''';nc=nc[:pos]+block+nc[pos:]
old='''        if(wheel->mSuspensionPart.IsActive() && !mSourceSpringCreationOrderV64[index])
            mSourceSpringCreationOrderV64[index]=++mSourceRowCreationCounterV64;'''
new='''        const bool configuredSpringObjectV104=objectLifetimeV104 && wheel->HasContact()
            && NativeWheelObjectLifetimeV104::springConfigured(mSourceWheelTravelV104[index]);
        if((objectLifetimeV104? configuredSpringObjectV104 : wheel->mSuspensionPart.IsActive()) && !mSourceSpringCreationOrderV64[index])
            mSourceSpringCreationOrderV64[index]=++mSourceRowCreationCounterV64;'''
assert nc.count(old)==1;nc=nc.replace(old,new)
needle='\tCalculatePitchRollConstraintProperties(body_transform);'
pos=nc.index(needle,start);assert pos<nc.index('void VehicleConstraint::ResetWarmStart()',start);nc=nc[:pos]+'''    if(objectLifetimeV104)for(uint index=0;index<mWheels.size();++index)
        mSourceSpringObjectPresentV104[index]=mWheels[index]->HasContact()
            && NativeWheelObjectLifetimeV104::springConfigured(mSourceWheelTravelV104[index]);
'''+nc[pos:]
nh=h.replace('''                mSourceSpringCreationOrderV64[index]=0;mSourceStopCreationOrderV64[index]=0;''','''                mSourceSpringCreationOrderV64[index]=0;mSourceStopCreationOrderV64[index]=0;
                if(index<mSourceSpringObjectPresentV104.size())mSourceSpringObjectPresentV104[index]=false;''')
needle='    const Array<uint64>& GetSourceContactCreationOrderV87() const { return mSourceContactCreationOrderV79; }';assert nh.count(needle)==1;nh=nh.replace(needle,'''    // Original travel, before Jolt radius/skin coordinate adjustments.
    void SetSourceWheelObjectTravelV104(const Array<float>& travel) { mSourceWheelTravelV104=travel; }
'''+needle)
needle='    uint64 mSourceRowCreationCounterV64 = 0;';assert nh.count(needle)==1;nh=nh.replace(needle,needle+'''
    Array<float> mSourceWheelTravelV104;
    Array<bool> mSourceSpringObjectPresentV104;
''')
needle='''        runtime.constraint =
            new JPH::VehicleConstraint(lock.GetBody(), settings);''';assert a.count(needle)==1;na=a.replace(needle,needle+'''
        JPH::Array<float> sourceWheelTravelV104;
        sourceWheelTravelV104.reserve(source.wheels.size());
        for(const auto& wheel:source.wheels)sourceWheelTravelV104.push_back(wheel.suspensionTravel);
        runtime.constraint->SetSourceWheelObjectTravelV104(sourceWheelTravelV104);
''')
changes=[('sdk','Jolt/Physics/Vehicle/VehicleConstraint.cpp',cp,c,nc),('sdk','Jolt/Physics/Vehicle/VehicleConstraint.h',hp,h,nh),('sdk','Jolt/Physics/Vehicle/NativeWheelObjectLifetimeV104.h',sdk/'Jolt/Physics/Vehicle/NativeWheelObjectLifetimeV104.h',None,header),('adapter','src/Rock3dEngine/source/physics/JoltVehiclePhysics.cpp',ap,a,na)]
for loc,rel,p,b,n in changes:
 if b is None:assert not p.exists()
 for state,text in [('before',b),('after',n)]:
  if text is not None:
   q=w/state/loc/rel;q.parent.mkdir(parents=True,exist_ok=True);q.write_text(text)
 files.append({'location':loc,'path':rel,'before_sha256':hashlib.sha256(b.encode()).hexdigest() if b is not None else None,'after_sha256':hashlib.sha256(n.encode()).hexdigest()})
(w/'source-manifest.json').write_text(json.dumps({'scope':'Default-off Native spring object existence and inactive creation-index reservation, using unchanged original wheel travel/contact; actor-wide vertical reset only. No tire order, coefficients, measured state or frame/case dispatch changes. VehicleConstraint private arrays change layout; rebuild all consumers. Wheel/Settings layout unchanged.','files':files,'goal_complete':False},indent=2)+'\n')
for loc,rel,p,b,n in changes:p.write_text(n)
print('Installed V104 explicit object lifetime candidate; four guarded sources; full consumer rebuild required',flush=True)
