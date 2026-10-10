#include "physics/OriginalVehiclePhysics.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
namespace phys = r3d::physics;
int main() {
    unsetenv("RRR3D_CALIBRATION_PROFILE");
    phys::WorldDescription d;d.gravity=-20;d.startPosition={0,0,10.25F};
    phys::TriangleMesh distant;
    distant.vertices={{-500,-500,-10000},{500,-500,-10000},{500,500,-10000},{-500,500,-10000}};
    distant.indices={0,1,2,0,2,3};d.collisionMeshes.push_back(distant);
    auto& v=d.vehicle;v.freezeRotation=true;v.mass=1200;v.halfExtents={1.5F,.8F,.3F};
    v.bodySleepEnergyThreshold=0;v.restBrakeTorque=0;v.brakeTorque=4000;
    v.maximumTorque=600;v.maximumRpm=7000;v.maximumSpeed=100;
    v.differentialRatio=3.42F;v.steerAngle=.5235988F;
    for(int i=0;i<4;++i){phys::WheelDescription w;
        w.position={i<2?1.1F:-1.1F,i%2==0?.75F:-.75F,100};
        w.radius=.4F;w.width=.2F;w.suspensionTravel=.5F;w.spring=35000;w.damper=4500;
        w.inverseMass=.1F;w.flags=64;w.driven=w.steering=i<2;v.wheels.push_back(w);}
    phys::DecorationDescription floor;floor.hasBodyShape=true;
    floor.halfExtents={20,20,.25F};floor.transform.position={0,0,9.75F};d.decorations.push_back(floor);
    std::string error;auto world=phys::createOriginalVehicleWorld(d,error);
    if(!world){std::cerr<<error;return 1;}
    world->setVehicleFixedStepController([](std::size_t,float,const phys::VehicleInput&,
        const phys::VehicleFixedStepState&){phys::VehicleDriveCommand command;command.gear=1;return command;});
    world->addLinearVelocity(0,{5,5,0});phys::Vec3 start{};
    for(int tick=0;tick<=40;++tick){world->step(1.0F/60,phys::VehicleInput{});
        const auto& state=world->vehicle();
        if(!std::isfinite(state.kineticEnergy)||state.angularMomentum.x!=0||
           state.angularMomentum.y!=0||state.angularMomentum.z!=0||
           state.body.rotation.x!=0||state.body.rotation.y!=0||state.body.rotation.z!=0)return 2;
        if(tick==20)start=state.linearVelocity;
    }
    const auto finish=world->vehicle().linearVelocity;
    // Independent Nx capture: ticks 20..40 decelerate both axes by 5.80001235.
    // The ellipse incorrectly predicts mu*g/sqrt(2) = 4.1012193 per axis.
    for(float acceleration:{(start.x-finish.x)*3,(start.y-finish.y)*3})
        if(std::abs(acceleration-5.80001235F)>.0002F){
            std::cerr<<"Nx independent friction mismatch: "<<acceleration;return 3;}
    std::cout<<"Nx diagonal translation deceleration matches on both axes; frozen state is finite\n";
}
