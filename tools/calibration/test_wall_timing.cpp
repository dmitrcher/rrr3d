#include "physics/OriginalVehiclePhysics.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
namespace phys = r3d::physics;
int main() {
    unsetenv("RRR3D_CALIBRATION_PROFILE");
    // Independent VC2008 / Nx SDK captures, no ground or gravity.
    // This checks contact onset only: impulse and angular parity remain open.
    const float speeds[] = {4, 8, 12, 24};
    const int referenceTicks[] = {94, 47, 32, 16};
    for (int probe=0; probe<4; ++probe) {
        phys::WorldDescription d; d.gravity=0; d.startPosition={0,0,13};
        phys::TriangleMesh floor;
        floor.vertices={{-500,-500,-10000},{500,-500,-10000},
                        {500,500,-10000},{-500,500,-10000}};
        floor.indices={0,1,2,0,2,3}; d.collisionMeshes.push_back(floor);
        auto& v=d.vehicle; v.mass=1200; v.halfExtents={1.5F,.8F,.3F};
        v.bodySleepEnergyThreshold=0; v.restBrakeTorque=0;
        v.brakeTorque=4000; v.maximumTorque=600; v.maximumRpm=7000;
        v.maximumSpeed=100; v.differentialRatio=3.42F; v.steerAngle=.5235988F;
        for(int i=0;i<4;++i) {
            phys::WheelDescription w;
            w.position={i<2?1.1F:-1.1F,i%2==0?.75F:-.75F,0};
            w.radius=.4F; w.width=.2F; w.suspensionTravel=.5F;
            w.spring=35000; w.damper=4500; w.inverseMass=.1F; w.flags=64;
            w.driven=w.steering=i<2; v.wheels.push_back(w);
        }
        phys::DecorationDescription wall; wall.hasBodyShape=true;
        wall.halfExtents={.25F,10,3}; wall.transform.position={8,0,13};
        d.decorations.push_back(wall);
        std::string error; auto world=phys::createOriginalVehicleWorld(d,error);
        if(!world){std::cerr<<error;return 1;}
        world->setVehicleFixedStepController([](std::size_t,float,
            const phys::VehicleInput&,const phys::VehicleFixedStepState&){
            phys::VehicleDriveCommand command;command.gear=1;return command;});
        world->addLinearVelocity(0,{speeds[probe],0,0});
        int impact=-1;
        for(int tick=0;tick<120;++tick) {
            world->step(1.0F/60,phys::VehicleInput{});
            if(std::abs(world->vehicle().linearVelocity.x-speeds[probe])>.01F){
                impact=tick;break;
            }
        }
        if(impact!=referenceTicks[probe]) {
            std::cerr<<"Nx contact onset mismatch: speed="<<speeds[probe]
                     <<" tick="<<impact<<" expected="<<referenceTicks[probe];
            return 2;
        }
    }
    std::cout<<"First wall impact matches independent Nx captures at four speeds\n";
}
