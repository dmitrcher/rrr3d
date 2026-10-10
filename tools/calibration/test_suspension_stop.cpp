#include "physics/OriginalVehiclePhysics.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
namespace phys = r3d::physics;
int main() {
    unsetenv("RRR3D_CALIBRATION_PROFILE");
    phys::WorldDescription d; d.gravity=-20; d.startPosition={0,0,11};
    phys::TriangleMesh floor;
    floor.vertices={{-500,-500,10},{500,-500,10},{500,500,10},{-500,500,10}};
    floor.indices={0,1,2,0,2,3}; d.collisionMeshes.push_back(floor);
    auto& v=d.vehicle;v.mass=1200;v.halfExtents={1.5F,.8F,.3F};
    v.bodySleepEnergyThreshold=0;v.maximumTorque=600;v.maximumRpm=7000;
    v.maximumSpeed=100;v.brakeTorque=4000;v.differentialRatio=3.42F;
    v.steerAngle=.5235988F;v.restBrakeTorque=0;v.airbornePitchAcceleration=0;
    for(int i=0;i<4;++i){phys::WheelDescription w;
        w.position={i<2?1.1F:-1.1F,i%2==0?.75F:-.75F,0};
        w.radius=.4F;w.width=.2F;w.suspensionTravel=5;
        w.spring=w.damper=0;w.inverseMass=.1F;w.flags=64;
        w.longitudinalTire.stiffnessFactor=w.lateralTire.stiffnessFactor=0;
        w.driven=w.steering=i<2;v.wheels.push_back(w);
    }
    std::string error;auto world=phys::createOriginalVehicleWorld(d,error);
    if(!world){std::cerr<<error;return 1;}
    world->setVehicleFixedStepController([](std::size_t,float,const phys::VehicleInput&,const phys::VehicleFixedStepState&){phys::VehicleDriveCommand cmd;cmd.gear=1;return cmd;});
    world->addLinearVelocity(0,{5,0,-3});
    for(int tick=0;tick<180;++tick)world->step(1.0F/60,phys::VehicleInput{});
    const auto& s=world->vehicle();
    // Independent VC2008/Nx SDK probe: at step 180, actor Z=10.3500004,
    // Vx=5 exactly, all axle extensions=-0.05. No spring or tire force.
    if(std::abs(s.body.position.z-10.3500004F)>.003F ||
       std::abs(s.linearVelocity.x-5)>.0001F ||
       std::abs(s.linearVelocity.z)>.005F){
        std::cerr<<"Nx hard-stop mismatch: height="<<s.body.position.z
                 <<" Vx="<<s.linearVelocity.x<<" Vz="<<s.linearVelocity.z;return 2;
    }
    for(const auto& w:s.wheelContacts)
        if(!w.hasContact || std::abs(w.suspensionLength+.05F)>.004F)return 3;
    std::cout<<"Zero-stiffness compression stop matches Nx SDK height and preserves free roll\n";
}
