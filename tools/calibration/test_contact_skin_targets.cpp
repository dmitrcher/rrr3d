#include "physics/OriginalVehiclePhysics.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
namespace phys = r3d::physics;
int main() {
    unsetenv("RRR3D_CALIBRATION_PROFILE");
    for (bool thin : {false, true}) for (float skin : {.01F,.025F,.1F}) for (bool mesh : {false, true}) for (float angle : {0.0F, .2F, .5F, .8F, -.5F}) {
        const float halfZ=thin?.02F:.3F;
        const float support=1.5F*std::abs(std::cos(angle))+halfZ*std::abs(std::sin(angle));
        phys::WorldDescription d; d.gravity = 0; d.startPosition = {(mesh?8.0F:7.75F)-support+.25F,0,13};
        phys::TriangleMesh far; far.vertices={{-500,-500,-10000},{500,-500,-10000},{500,500,-10000},{-500,500,-10000}};
        far.indices={0,1,2,0,2,3}; d.collisionMeshes.push_back(far);
        auto& v=d.vehicle; v.freezeRotation=true; v.mass=1200;v.halfExtents={1.5F,.8F,halfZ};v.bodySkinWidth=skin;v.bodySleepEnergyThreshold=0;
        v.maximumTorque=600;v.maximumRpm=7000;v.maximumSpeed=100;v.differentialRatio=3.42F;v.brakeTorque=4000;
        for(int i=0;i<4;++i){phys::WheelDescription w;w.position={i<2?1.1F:-1.1F,i%2==0?.75F:-.75F,100};w.radius=.4F;w.width=.2F;w.suspensionTravel=.5F;w.spring=35000;w.damper=4500;w.inverseMass=.1F;w.flags=64;w.driven=w.steering=i<2;v.wheels.push_back(w);}
        if(mesh){phys::TriangleMesh wall;wall.surface=phys::CollisionSurface::TrackBorder;wall.vertices={{8,-10,10},{8,10,10},{8,10,16},{8,-10,16}};wall.indices={0,2,1,0,3,2};d.collisionMeshes.push_back(wall);}
        else{phys::DecorationDescription wall;wall.hasBodyShape=true;wall.halfExtents={.25F,10,3};wall.transform.position={8,0,13};d.decorations.push_back(wall);}
        std::string error;auto world=phys::createOriginalVehicleWorld(d,error);if(!world){std::cerr<<error;return 2;}
        phys::Quat q;q.y=std::sin(angle*.5F);q.w=std::cos(angle*.5F);world->synchronizeNetworkVehicle(0,d.startPosition,q,{}, {},true);
        for(int tick=0;tick<24;++tick)world->step(1.0F/60,phys::VehicleInput{});
        // Independent Nx probes settle to 8-1.5+.05 for the triangle and
        // 7.75-1.5+.05 for the box. The old adapter left an extra .025/.05.
        const float expected=(mesh?8.0F:7.75F)-support+skin+.025F;
        const float actual=world->vehicle().body.position.x;
        if(!std::isfinite(actual)||std::abs(actual-expected)>2.0e-5F){std::cerr<<"Source skin target mismatch "<<mesh<<" angle "<<angle<<" skin "<<skin<<" thin "<<thin<<": "<<actual<<" expected "<<expected;return 3;}
    }
    std::cout<<"Independent Nx aligned and tilted box/triangle skin targets match\n";
}
