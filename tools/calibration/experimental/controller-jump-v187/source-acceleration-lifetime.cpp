#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemSingleThreaded.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/StateRecorderImpl.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

class Layers final : public JPH::BroadPhaseLayerInterface {
public:
    JPH::uint GetNumBroadPhaseLayers() const override { return 1; }
    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer) const override { return JPH::BroadPhaseLayer(0); }
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer) const override { return "isolated"; }
#endif
};
class Pair final : public JPH::ObjectLayerPairFilter {
public: bool ShouldCollide(JPH::ObjectLayer, JPH::ObjectLayer) const override { return false; }
};
class Broad final : public JPH::ObjectVsBroadPhaseLayerFilter {
public: bool ShouldCollide(JPH::ObjectLayer, JPH::BroadPhaseLayer) const override { return false; }
};
static unsigned word(float v) { unsigned u; std::memcpy(&u,&v,4); return u; }
static void check(bool value,const char* reason) { if (!value) throw std::runtime_error(reason); }

int main() {
    try {
        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory;
        JPH::RegisterTypes();
        {
            Layers layers; Pair pair; Broad broad;
            JPH::PhysicsSystem system;
            system.Init(32,0,32,32,layers,broad,pair);
            system.SetGravity(JPH::Vec3::sZero());
            JPH::TempAllocatorImpl temporary(10*1024*1024);
            JPH::JobSystemSingleThreaded jobs(2048);
            auto& bodies=system.GetBodyInterface();
            constexpr float dt=1.0F/60.0F;
            const float expected=-20.0F*dt;
            for (float mass : {.1F,.333F,1.0F,300.0F,1200.0F,4800.0F,1000000.0F}) {
                JPH::BodyCreationSettings settings(new JPH::BoxShape(JPH::Vec3(1,1,1)),
                    JPH::RVec3(0,0,100),JPH::Quat::sIdentity(),JPH::EMotionType::Dynamic,0);
                settings.mOverrideMassProperties=JPH::EOverrideMassProperties::CalculateInertia;
                settings.mMassPropertiesOverride.mMass=mass;
                settings.mLinearDamping=settings.mAngularDamping=0;
                settings.mAllowSleeping=false;
                auto* body=bodies.CreateBody(settings);
                check(body!=nullptr,"body creation");
                const auto id=body->GetID();
                bodies.AddBody(id,JPH::EActivation::Activate);
                auto* motion=body->GetMotionProperties();
                motion->SetSourceLinearAccelerationV187(JPH::Vec3(0,0,-20));
                JPH::StateRecorderImpl saved;
                motion->SaveState(saved);
                motion->ResetForce();
                check(motion->GetSourceLinearAccelerationV187()==JPH::Vec3::sZero(),"explicit force reset retained acceleration");
                saved.Rewind();
                motion->RestoreState(saved);
                check(motion->GetSourceLinearAccelerationV187()==JPH::Vec3(0,0,-20),"restore lost pending acceleration");
                check(system.Update(dt,2,&temporary,&jobs)==JPH::EPhysicsUpdateError::None,"first update");
                const float first=bodies.GetLinearVelocity(id).GetZ();
                check(word(first)==word(expected),"acceleration depends on body mass or was lost within substeps");
                check(motion->GetSourceLinearAccelerationV187()==JPH::Vec3::sZero(),"end-of-update force reset retained acceleration");
                check(system.Update(dt,2,&temporary,&jobs)==JPH::EPhysicsUpdateError::None,"second update");
                check(word(bodies.GetLinearVelocity(id).GetZ())==word(first),"one-shot acceleration repeated next update");
                motion->SetSourceLinearAccelerationV187(JPH::Vec3(0,0,-20));
                motion->ResetMotion();
                check(system.Update(dt,2,&temporary,&jobs)==JPH::EPhysicsUpdateError::None,"motion-reset update");
                check(word(bodies.GetLinearVelocity(id).GetZ())==word(0.0F),"ResetMotion retained queued acceleration");
                std::cout<<"{\"mass\":"<<mass<<",\"first_vz_word\":"<<word(first)
                    <<",\"substeps\":2,\"mass_independent\":true,\"force_reset\":true,\"restore\":true,\"reset_motion\":true}\n";
                bodies.RemoveBody(id); bodies.DestroyBody(id);
            }
        }
        JPH::UnregisterTypes(); delete JPH::Factory::sInstance; JPH::Factory::sInstance=nullptr;
        return 0;
    } catch (const std::exception& e) { std::cerr<<e.what()<<'\n'; return 1; }
}
