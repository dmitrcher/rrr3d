#include "physics/OriginalVehiclePhysics.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
namespace phys = r3d::physics;
int main() {
    unsetenv("RRR3D_CALIBRATION_PROFILE");
    for (float speed : {2.0F, 10.0F, 30.0F}) {
        phys::WorldDescription d; d.gravity = 0; d.startPosition = {0,0,3};
        phys::TriangleMesh floor;
        floor.vertices = {{-500,-500,0},{500,-500,0},{500,500,0},{-500,500,0}};
        floor.indices = {0,1,2,0,2,3}; d.collisionMeshes.push_back(floor);
        auto& v = d.vehicle;
        v.mass = 1000; v.halfExtents = {1,1,.5F}; v.bodySleepEnergyThreshold = 0;
        v.brakeTorque = 4000; v.maximumTorque = 600; v.maximumRpm = 7000;
        v.maximumSpeed = 100; v.differentialRatio = 3.42F; v.steerAngle = .5235988F;
        v.restBrakeTorque = 0; v.airbornePitchAcceleration = 0;
        // Keep suspension rays away from the ground: only chassis impulses act.
        for (int i = 0; i < 4; ++i) {
            phys::WheelDescription w; w.position = {(i&1)?1.0F:-1.0F,(i&2)?.7F:-.7F,100};
            w.driven = true; w.radius = .5F; w.width = .2F; w.suspensionTravel = .3F;
            w.spring = 20000; w.damper = 1000; w.inverseMass = .1F; v.wheels.push_back(w);
        }
        std::string error; auto world = phys::createOriginalVehicleWorld(d,error);
        if (!world) {std::cerr << error; return 1;}
        world->addLinearVelocity(0,{0,0,-speed});
        world->step(1.0F/60, phys::VehicleInput{});
        bool impact = false;
        for (int tick = 0; tick < 180; ++tick) {
            const auto before = world->vehicle().linearVelocity;
            world->step(1.0F/60, phys::VehicleInput{});
            const auto& state = world->vehicle();
            double reported = 0;
            for (const auto& c : state.bodyContacts) {
                if (!std::isfinite(c.frictionForce) || !std::isfinite(c.frictionForceVector.x) ||
                    !std::isfinite(c.frictionForceVector.y) || !std::isfinite(c.frictionForceVector.z)) return 4;
                reported += c.normal.z*c.force/60;
            }
            double actual = v.mass*(state.linearVelocity.z-before.z);
            if (std::abs(reported-actual) > std::max(0.02, std::abs(actual)*1e-4)) {
                std::cerr << "Impulse mismatch speed=" << speed << " tick=" << tick
                          << " reported=" << reported << " momentum change=" << actual << '\n';
                return 2;
            }
            impact = impact || reported > 1;
        }
        if (!impact) return 3;
    }
    std::cout << "Reported chassis impulses match actual momentum changes at three impact speeds\n";
}
