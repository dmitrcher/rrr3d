#include "Fixture.h"
#include "physics/OriginalVehiclePhysics.h"
#include <iostream>
namespace cal=motorrock::calibration;
namespace phys=r3d::physics;
static phys::Vec3 v3(cal::V v){return {static_cast<float>(v.x),static_cast<float>(v.y),static_cast<float>(v.z)};}
static phys::WheelDescription::TireFunction tire(const cal::Tire& t){return {static_cast<float>(t.extremumSlip),static_cast<float>(t.extremumValue),static_cast<float>(t.asymptoteSlip),static_cast<float>(t.asymptoteValue),static_cast<float>(t.stiffness)};}
static void execute(const cal::Case& c) {
 phys::WorldDescription d;d.gravity=static_cast<float>(c.gravity);d.startPosition={0,0,static_cast<float>(c.height)};
 phys::TriangleMesh floor;float z=c.ground?10.0F:-10000.0F;
 auto height=[&](float x,float y){return z+static_cast<float>(c.slopeX*x+c.slopeY*y);};
 floor.vertices={{-500,-500,height(-500,-500)},{500,-500,height(500,-500)},{500,500,height(500,500)},{-500,500,height(-500,500)}};floor.indices={0,1,2,0,2,3};d.collisionMeshes.push_back(floor);
 auto& v=d.vehicle;v.freezeRotation=c.fixedOrientation;v.mass=static_cast<float>(c.mass);v.halfExtents=v3(c.halfExtents);v.centerOfMass=v3(c.centerOfMass);v.shapePosition=v3(c.shapePosition);v.bodyFriction=static_cast<float>(c.bodyFriction);v.bodySleepEnergyThreshold=static_cast<float>(c.sleepThreshold);v.tireSpring=static_cast<float>(c.tireSpring);v.brakeTorque=static_cast<float>(c.brakeLimit);v.maximumTorque=600;v.maximumRpm=7000;v.differentialRatio=3.42F;v.maximumSpeed=100;v.steerAngle=.5235988F;v.restBrakeTorque=static_cast<float>(c.restBrake);v.clampRollAngle=v.clampPitchAngle=0;
 for(const auto& source:c.wheels){phys::WheelDescription w;w.position=v3(source.position);w.flags=source.flags;w.radius=static_cast<float>(source.radius);w.width=.2F;w.suspensionTravel=static_cast<float>(source.travel);w.spring=static_cast<float>(source.spring);w.damper=static_cast<float>(source.damper);w.suspensionTarget=static_cast<float>(source.target);w.inverseMass=static_cast<float>(source.inverseMass);w.driven=source.driven;w.steering=source.steering;w.longitudinalTire=tire(source.longitudinal);w.lateralTire=tire(source.lateral);v.wheels.push_back(w);}
 if(c.pair){phys::VehicleSpawn first;first.vehicle=v;first.position=d.startPosition;d.spawns.push_back(first);phys::VehicleSpawn second=first;second.vehicle.mass=static_cast<float>(c.peerMass);second.vehicle.bodyFriction=static_cast<float>(c.peerFriction);second.position=v3(c.peerPosition);d.spawns.push_back(second);}
 if(c.obstacle==7||c.obstacle==8||c.obstacle==9){phys::TriangleMesh border;border.surface=phys::CollisionSurface::TrackBorder;
  border.vertices=c.obstacle!=8?std::vector<phys::Vec3>{{8,-10,10},{8,10,10},{8,10,16},{8,-10,16}}:std::vector<phys::Vec3>{{-500,-500,10},{500,-500,10},{500,500,10},{-500,500,10}};
  border.indices=c.obstacle==9?std::vector<std::uint32_t>{0,2,1,0,3,2}:std::vector<std::uint32_t>{0,1,2,0,2,3};d.collisionMeshes.push_back(border);
 }
 else if(c.obstacle==5){phys::DecorationDescription o;o.hasBodyShape=true;o.halfExtents={20,20,.25F};o.transform.position={0,0,9.75F};d.decorations.push_back(o);}
 else if(c.obstacle==6){phys::DecorationDescription o;o.hasBodyShape=true;o.halfExtents={10,.25F,3};o.transform.position={0,8,13};d.decorations.push_back(o);}
 else if(c.obstacle){phys::DecorationDescription o;o.hasBodyShape=true;o.halfExtents=c.obstacle<=2?phys::Vec3{.25F,10,3}:c.obstacle==3?phys::Vec3{.5F,10,.15F}:phys::Vec3{20,.25F,.3F};o.transform.position=c.obstacle<=2?phys::Vec3{8,0,13}:c.obstacle==3?phys::Vec3{8,0,10.15F}:phys::Vec3{8,.9F,10.3F};d.decorations.push_back(o);}
 std::string error;auto world=phys::createOriginalVehicleWorld(d,error);if(!world)throw std::runtime_error(error);
 if(c.pair){for(std::size_t i=0;i<2;++i){const float angle=static_cast<float>(i?c.peerRoll:c.primaryRoll);phys::Quat q;q.x=std::sin(angle*.5F);q.w=std::cos(angle*.5F);world->synchronizeNetworkVehicle(i,i?v3(c.peerPosition):d.startPosition,q,{}, {},true);world->setWheelTractionEnabled(i,c.traction);}world->addLinearVelocity(1,v3(c.peerVelocity));}
 world->setWheelTractionEnabled(0,c.traction);
 cal::Command x;
 world->setVehicleFixedStepController([&x,&c](std::size_t,float,const phys::VehicleInput&,const phys::VehicleFixedStepState&){phys::VehicleDriveCommand cmd;cmd.motorTorque=static_cast<float>(x.torque);cmd.brakeTorque=static_cast<float>(x.brake);cmd.steeringAngle=static_cast<float>(x.steer);cmd.gear=1;cmd.applyExtraGravity=c.extraGravity;cmd.airbornePitchAcceleration=static_cast<float>(c.airPitch);cmd.angularDamping={static_cast<float>(c.angularDamping.x),static_cast<float>(c.angularDamping.y),static_cast<float>(c.angularDamping.z)};cmd.clampRollAngle=static_cast<float>(c.clampRoll);cmd.clampPitchAngle=static_cast<float>(c.clampPitch);return cmd;});
 world->addLinearVelocity(0,{static_cast<float>(c.velocity.x),static_cast<float>(c.velocity.y),static_cast<float>(c.velocity.z)});
 world->addAngularVelocity(0,{static_cast<float>(c.angular.x),static_cast<float>(c.angular.y),static_cast<float>(c.angular.z)});
 for(int tick=0;tick<c.steps;++tick){x=cal::command(c,tick);world->setDiagnosticAcceleration(0,{static_cast<float>(x.acceleration.x),static_cast<float>(x.acceleration.y),static_cast<float>(x.acceleration.z)});
 if(x.impulse.x||x.impulse.y||x.impulse.z)world->addLinearVelocity(0,{static_cast<float>(x.impulse.x),static_cast<float>(x.impulse.y),static_cast<float>(x.impulse.z)});
 world->step(1.0F/60,phys::VehicleInput{});const auto& a=world->vehicle(c.pair?static_cast<std::size_t>(c.watched):0);cal::Sample s;
 s.position=cal::V(a.body.position.x,a.body.position.y,a.body.position.z);s.velocity=cal::V(a.linearVelocity.x,a.linearVelocity.y,a.linearVelocity.z);s.momentum=cal::V(a.angularMomentum.x,a.angularMomentum.y,a.angularMomentum.z);
 s.q[0]=a.body.rotation.x;s.q[1]=a.body.rotation.y;s.q[2]=a.body.rotation.z;s.q[3]=a.body.rotation.w;
 for(std::size_t i=0;i<a.wheelAngularSpeeds.size();++i){s.wheelSpeed.push_back(a.wheelAngularSpeeds[i]);s.contacts.push_back(a.wheelContacts[i].hasContact?1:0);s.wheelHeight.push_back(a.wheelContacts[i].hasContact?a.wheelContacts[i].position.z:0);const auto& w=a.wheelContacts[i];s.longSlip.push_back(w.longitudinalSlip);s.latSlip.push_back(w.lateralSlip);s.suspension.push_back(w.hasContact?w.suspensionLength:0);s.normalForce.push_back(w.suspensionForce);s.longImpulse.push_back(w.longitudinalImpulse);s.latImpulse.push_back(w.lateralImpulse);}
 for(const auto& contact:a.bodyContacts){std::ostringstream body;
 body<<"{\"tick\":"<<tick<<",\"normal_force_magnitude\":"<<contact.force
     <<",\"friction_force\":"<<cal::vector(cal::V(contact.frictionForceVector.x,contact.frictionForceVector.y,contact.frictionForceVector.z))<<'}';
 motorrock::parity::event("calibration.body-impulse",c.name,body.str());}
 cal::sample(c,tick,x,s);
 }
}
int runMacPhysicsCalibration(int argc,char**argv){try{const char* p=std::getenv("RRR3D_CALIBRATION_PROFILE");std::string profile="default";if(p&&*p){std::ifstream f(p);std::ostringstream text;text<<f.rdbuf();profile=std::string(p)+"\n"+text.str();}return cal::run(argc,argv,execute,profile);}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 2;}}
#ifndef MOTORROCK_CALIBRATION_EMBEDDED
int main(int argc,char**argv){return runMacPhysicsCalibration(argc,argv);}
#endif
