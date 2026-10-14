#pragma once
#include "physics/OriginalVehiclePhysics.h"
#include <cstring>
#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace rrr3d::diagnostics {
// The words preserve actual F32 descriptions; no reference output or runtime
// state enters this fixture. Optional diagnostics leave the world unchanged.
inline void exportSourceWorldV124(const r3d::physics::WorldDescription& d,const char* path) {
 using namespace r3d::physics;
 std::ofstream out(path);if(!out)throw std::runtime_error("Cannot write world fixtureV124");
 out << "MOTORROCK_WORLD_V124\n";
 auto integer=[&](std::uint64_t x){out << x << ' ';};
 auto scalar=[&](float x){std::uint32_t u;std::memcpy(&u,&x,4);integer(u);};
 auto vector=[&](Vec3 v){scalar(v.x);scalar(v.y);scalar(v.z);};
 auto quaternion=[&](Quat q){scalar(q.x);scalar(q.y);scalar(q.z);scalar(q.w);};
 auto transform=[&](const Transform& t){vector(t.position);vector(t.scale);quaternion(t.rotation);};
 auto tire=[&](const WheelDescription::TireFunction& f){scalar(f.extremumSlip);scalar(f.extremumValue);scalar(f.asymptoteSlip);scalar(f.asymptoteValue);scalar(f.stiffnessFactor);};
 scalar(d.gravity);vector(d.startPosition);vector(d.startDirection);out << '\n';
 const auto& v=d.vehicle;
 scalar(v.mass);vector(v.halfExtents);vector(v.shapePosition);vector(v.centerOfMass);vector(v.angularDamping);
 scalar(v.bodyLinearDamping);scalar(v.bodyAngularDamping);integer(v.wheelContactsEnabled);integer(v.bodyContactFrictionEnabled);scalar(v.bodySkinWidth);scalar(v.bodySleepEnergyThreshold);integer(v.freezeRotation);
 for(float x:{v.bodyFriction,v.brakeTorque,v.differentialRatio,v.maximumRpm,v.idlingRpm,v.maximumTorque,v.torqueEfficiency,v.restBrakeTorque,v.maximumSpeed,v.tireSpring,v.airbornePitchAcceleration,v.clampRollAngle,v.clampPitchAngle,v.steerAngle,v.steeringControl,v.steerSpeed,v.steerRotation})scalar(x);
 integer(v.automaticGears);integer(v.gravitySteering);integer(v.clutchImmunity);out << '\n';
 integer(v.wheels.size());out << '\n';
 for(const auto& wheel:v.wheels){vector(wheel.position);for(float x:{wheel.radius,wheel.width,wheel.suspensionTravel,wheel.spring,wheel.damper,wheel.suspensionTarget,wheel.inverseMass})scalar(x);integer(wheel.flags);integer(wheel.driven);integer(wheel.steering);tire(wheel.longitudinalTire);tire(wheel.lateralTire);out << '\n';}
 integer(d.collisionMeshes.size());out << '\n';
 for(const auto& mesh:d.collisionMeshes){integer(static_cast<unsigned>(mesh.surface));integer(mesh.decorationInstance);transform(mesh.transform);integer(mesh.vertices.size());integer(mesh.indices.size());out << '\n';for(Vec3 point:mesh.vertices){vector(point);out << '\n';}for(auto index:mesh.indices)integer(index);out << '\n';}
 integer(d.decorations.size());out << '\n';
 for(const auto& a:d.decorations){transform(a.transform);vector(a.shapePosition);quaternion(a.shapeRotation);vector(a.halfExtents);scalar(a.skinWidth);scalar(a.mass);scalar(a.sleepEnergyThreshold);integer(a.hasBodyShape);integer(a.dynamic);integer(a.collisionResponse);integer(a.childShapes.size());out << '\n';for(const auto& child:a.childShapes){vector(child.position);quaternion(child.rotation);vector(child.halfExtents);scalar(child.skinWidth);out << '\n';}}
 integer(d.spawns.size());out << '\n';
 for(const auto& spawn:d.spawns){vector(spawn.position);vector(spawn.direction);out << '\n';}
 out.flush();if(!out)throw std::runtime_error("Incomplete world fixtureV124");
}
}
