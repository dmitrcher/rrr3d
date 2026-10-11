#include "physics/OriginalVehiclePhysics.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace phys = r3d::physics;

static phys::VehicleState firstImpact(bool frozen, bool mesh, float depth, float speed)
{
    phys::WorldDescription d;
    d.gravity = 0;
    d.startPosition = {(mesh ? 8.0F : 7.75F) - 1.5F + depth, 0, 13};
    phys::TriangleMesh distant;
    distant.vertices = {{-500,-500,-10000},{500,-500,-10000},
                        {500,500,-10000},{-500,500,-10000}};
    distant.indices = {0,1,2,0,2,3};
    d.collisionMeshes.push_back(distant);
    auto& v = d.vehicle;
    v.freezeRotation = frozen;
    v.mass = 1200;
    v.halfExtents = {1.5F,.8F,.3F};
    v.bodySleepEnergyThreshold = 0;
    v.maximumTorque = 600;
    v.maximumRpm = 7000;
    v.maximumSpeed = 100;
    v.differentialRatio = 3.42F;
    v.brakeTorque = 4000;
    for (int i = 0; i < 4; ++i)
    {
        phys::WheelDescription w;
        w.position = {i < 2 ? 1.1F : -1.1F, i % 2 == 0 ? .75F : -.75F, 100};
        w.radius = .4F; w.width = .2F; w.suspensionTravel = .5F;
        w.spring = 35000; w.damper = 4500; w.inverseMass = .1F;
        w.flags = 64; w.driven = w.steering = i < 2;
        v.wheels.push_back(w);
    }
    if (mesh)
    {
        phys::TriangleMesh wall;
        wall.surface = phys::CollisionSurface::TrackBorder;
        wall.vertices = {{8,-10,10},{8,10,10},{8,10,16},{8,-10,16}};
        wall.indices = {0,2,1,0,3,2};
        d.collisionMeshes.push_back(wall);
    }
    else
    {
        phys::DecorationDescription wall;
        wall.hasBodyShape = true;
        wall.halfExtents = {.25F,10,3};
        wall.transform.position = {8,0,13};
        d.decorations.push_back(wall);
    }
    std::string error;
    auto world = phys::createOriginalVehicleWorld(d, error);
    if (!world) throw std::runtime_error(error);
    world->addLinearVelocity(0, {speed,0,0});
    world->step(1.0F / 60, phys::VehicleInput{});
    return world->vehicle();
}

static float difference(phys::Vec3 a, phys::Vec3 b)
{
    return std::max({std::abs(a.x-b.x), std::abs(a.y-b.y), std::abs(a.z-b.z)});
}

int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    // The independent VC2008 / Nx 2.8.4 reference has exactly equal first
    // position and linear velocity for all 36 frozen/free impact pairs,
    // including normal material friction. See reference-frozen-pose-v13.json.
    // It also reports unchanged frozen orientation and zero frozen momentum.
    try
    {
        setenv("RRR3D_CALIBRATION_PROFILE", argv[1], 1);
        float legacyDifference = 0;
        bool freeRotated = false;
        for (int enabled : {1, 0})
        {
            { std::ofstream profile(argv[1]); profile << "physx_frozen_pose " << enabled << '\n'; }
            for (bool mesh : {false, true})
                for (float depth : {-.02F,0.0F,.025F,.05F,.25F,1.0F})
                    for (float speed : {-8.0F,0.0F,8.0F})
                    {
                        const auto fixed = firstImpact(true, mesh, depth, speed);
                        const auto free = firstImpact(false, mesh, depth, speed);
                        const float delta = std::max(difference(fixed.body.position, free.body.position),
                            difference(fixed.linearVelocity, free.linearVelocity));
                        if (!enabled) { legacyDifference = std::max(legacyDifference, delta); continue; }
                        if (!std::isfinite(delta) || delta > 2.0e-6F)
                            throw std::runtime_error("Frozen/free first-step translation differs from Nx invariant");
                        if (difference(fixed.angularMomentum, {}) != 0 ||
                            fixed.body.rotation.x != 0 || fixed.body.rotation.y != 0 ||
                            fixed.body.rotation.z != 0 || fixed.body.rotation.w != 1)
                            throw std::runtime_error("Frozen orientation/momentum differs from Nx invariant");
                        freeRotated |= std::abs(free.body.rotation.x) + std::abs(free.body.rotation.y)
                            + std::abs(free.body.rotation.z) > 1.0e-6F;
                    }
        }
        if (!freeRotated || legacyDifference < .001F)
            throw std::runtime_error("Impact fixtures do not distinguish the old zero-inertia policy");
        std::remove(argv[1]);
        std::cout << "36 independent Nx frozen/free impact invariants match; legacy negative control differs by "
                  << legacyDifference << '\n';
        return 0;
    }
    catch (const std::exception& e) { std::remove(argv[1]); std::cerr << e.what() << '\n'; return 1; }
}
