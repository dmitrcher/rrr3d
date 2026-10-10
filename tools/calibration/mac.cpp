#include "Fixture.h"
#include "physics/OriginalVehiclePhysics.h"
#include <iostream>
namespace cal=motorrock::calibration;
namespace phys=r3d::physics;
static void execute(const cal::Case& c) {
 phys::WorldDescription d;d.gravity=static_cast<float>(c.gravity);d.startPosition={0,0,static_cast<float>(c.height)};
 phys::TriangleMesh floor;float z=c.ground?10.0F:-10000.0F;
 floor.vertices={{-500,-500,z},{500,-500,z},{500,500,z},{-500,500,z}};floor.indices={0,1,2,0,2,3};d.collisionMeshes.push_back(floor);
 auto& v=d.vehicle;v.mass=static_cast<float>(c.mass);v.halfExtents={1.5F,.8F,.3F};v.centerOfMass={0,0,0};v.shapePosition={0,0,0};v.bodyFriction=.08F;v.bodySleepEnergyThreshold=0;v.brakeTorque=4000;v.maximumTorque=600;v.maximumRpm=7000;v.differentialRatio=3.42F;v.maximumSpeed=100;v.steerAngle=.5235988F;v.restBrakeTorque=0;v.clampRollAngle=v.clampPitchAngle=0;
 for(int i=0;i<4;++i){phys::WheelDescription w;w.position={i<2?1.1F:-1.1F,i%2==0?.75F:-.75F,0};w.flags=64;w.radius=.4F;w.width=.2F;w.suspensionTravel=.5F;w.spring=35000;w.damper=4500;w.inverseMass=.1F;w.driven=w.steering=i<2;w.longitudinalTire=w.lateralTire={1,.02F,2,.01F,1000000};v.wheels.push_back(w);}
 std::string error;auto world=phys::createOriginalVehicleWorld(d,error);if(!world)throw std::runtime_error(error);
 cal::Command x;
 world->setVehicleFixedStepController([&x](std::size_t,float,const phys::VehicleInput&,const phys::VehicleFixedStepState&){phys::VehicleDriveCommand cmd;cmd.motorTorque=static_cast<float>(x.torque);cmd.brakeTorque=static_cast<float>(x.brake);cmd.steeringAngle=static_cast<float>(x.steer);cmd.gear=1;return cmd;});
 world->addLinearVelocity(0,{static_cast<float>(c.velocity.x),static_cast<float>(c.velocity.y),static_cast<float>(c.velocity.z)});
 world->addAngularVelocity(0,{static_cast<float>(c.angular.x),static_cast<float>(c.angular.y),static_cast<float>(c.angular.z)});
 for(int tick=0;tick<c.steps;++tick){x=cal::command(c,tick);world->setDiagnosticAcceleration(0,{static_cast<float>(x.acceleration.x),static_cast<float>(x.acceleration.y),static_cast<float>(x.acceleration.z)});
 if(x.impulse.x||x.impulse.y||x.impulse.z)world->addLinearVelocity(0,{static_cast<float>(x.impulse.x),static_cast<float>(x.impulse.y),static_cast<float>(x.impulse.z)});
 world->step(1.0F/60,phys::VehicleInput{});const auto& a=world->vehicle();cal::Sample s;
 s.position=cal::V(a.body.position.x,a.body.position.y,a.body.position.z);s.velocity=cal::V(a.linearVelocity.x,a.linearVelocity.y,a.linearVelocity.z);s.momentum=cal::V(a.angularMomentum.x,a.angularMomentum.y,a.angularMomentum.z);
 s.q[0]=a.body.rotation.x;s.q[1]=a.body.rotation.y;s.q[2]=a.body.rotation.z;s.q[3]=a.body.rotation.w;
 for(std::size_t i=0;i<a.wheelAngularSpeeds.size();++i){s.wheelSpeed.push_back(a.wheelAngularSpeeds[i]);s.contacts.push_back(a.wheelContacts[i].hasContact?1:0);s.wheelHeight.push_back(a.wheelContacts[i].hasContact?a.wheelContacts[i].position.z:0);}
 cal::sample(c,tick,x,s);
 }
}
int runMacPhysicsCalibration(int argc,char**argv){try{const char* p=std::getenv("RRR3D_CALIBRATION_PROFILE");std::string profile="default";if(p&&*p){std::ifstream f(p);std::ostringstream text;text<<f.rdbuf();profile=std::string(p)+"\n"+text.str();}return cal::run(argc,argv,execute,profile);}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 2;}}
#ifndef MOTORROCK_CALIBRATION_EMBEDDED
int main(int argc,char**argv){return runMacPhysicsCalibration(argc,argv);}
#endif
