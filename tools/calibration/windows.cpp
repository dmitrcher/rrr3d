#include <NxPhysics.h>
#include "Fixture.h"
#include <iostream>
namespace cal=motorrock::calibration;
static NxPhysicsSDK* sdk=0;
// Same neutral GameCar wheel-contact budget (no clutch/tireSpring effects).
struct WheelContact : NxUserWheelContactModify {
 float gravity; explicit WheelContact(float g):gravity(g){}
 virtual bool onWheelContact(NxWheelShape* w,NxVec3&,NxVec3& normal,NxReal&,NxReal& force,NxShape*,NxMaterialIndex&,NxU32) {
  const float load=std::abs(w->getActor().getMass()*normal.z*gravity/4);
  force=std::min(force,load*1.5f);return true;
 }
};
static void execute(const cal::Case& c) {
 NxSceneDesc desc;desc.gravity=NxVec3(0,0,(float)c.gravity);desc.upAxis=2;desc.timeStepMethod=NX_TIMESTEP_VARIABLE;desc.simType=NX_SIMULATION_SW;
 NxScene* scene=sdk->createScene(desc);if(!scene)throw std::runtime_error("PhysX scene creation failed");
 NxMaterial* material=scene->getMaterialFromIndex(0);material->setStaticFriction(.5f);material->setDynamicFriction(.5f);material->setRestitution(.5f);
 NxMaterialDesc carMat;carMat.staticFriction=carMat.dynamicFriction=.08f;carMat.restitution=0;
 NxMaterial* carMaterial=scene->createMaterial(carMat);
 if(c.ground){NxPlaneShapeDesc floor;floor.normal=NxVec3(0,0,1);floor.d=10;NxActorDesc actor;actor.shapes.pushBack(&floor);if(!scene->createActor(actor))throw std::runtime_error("Floor creation failed");}
 NxBoxShapeDesc box;box.dimensions=NxVec3(1.5f,.8f,.3f);box.skinWidth=.025f;box.materialIndex=carMaterial->getMaterialIndex();
 NxBodyDesc body;body.mass=(float)c.mass;body.linearDamping=0;body.angularDamping=.05f;body.sleepEnergyThreshold=0;body.solverIterationCount=4;
 NxActorDesc actor;actor.body=&body;actor.shapes.pushBack(&box);actor.globalPose.t=NxVec3(0,0,(float)c.height);
 NxActor* car=scene->createActor(actor);if(!car)throw std::runtime_error("Car creation failed");
 WheelContact callback(static_cast<float>(c.gravity));
 NxWheelShape* wheels[4];
 for(int i=0;i<4;++i){NxWheelShapeDesc w;w.localPose.t=NxVec3(i<2?1.1f:-1.1f,i%2==0?.75f:-.75f,0);w.localPose.M.setColumn(0,NxVec3(0,1,0));w.localPose.M.setColumn(1,NxVec3(0,0,1));w.localPose.M.setColumn(2,NxVec3(1,0,0));w.radius=.4f;w.suspensionTravel=.5f;w.suspension.spring=35000;w.suspension.damper=4500;w.suspension.targetValue=0;w.inverseWheelMass=.1f;w.wheelFlags=64;w.skinWidth=.025f;w.wheelContactModify=&callback;
 wheels[i]=car->createShape(w)->isWheel();if(!wheels[i])throw std::runtime_error("Wheel creation failed");}
 car->setLinearVelocity(NxVec3((float)c.velocity.x,(float)c.velocity.y,(float)c.velocity.z));car->setAngularVelocity(NxVec3((float)c.angular.x,(float)c.angular.y,(float)c.angular.z));
 for(int tick=0;tick<c.steps;++tick){const cal::Command x=cal::command(c,tick);
 car->addForce(NxVec3((float)x.acceleration.x,(float)x.acceleration.y,(float)x.acceleration.z),NX_ACCELERATION);
 if(x.impulse.x||x.impulse.y||x.impulse.z)car->addForce(NxVec3((float)x.impulse.x,(float)x.impulse.y,(float)x.impulse.z),NX_VELOCITY_CHANGE);
 for(int i=0;i<4;++i){wheels[i]->setMotorTorque(i<2?(float)x.torque:0);wheels[i]->setBrakeTorque((float)x.brake);wheels[i]->setSteerAngle(i<2?(float)x.steer:0);}
 scene->simulate(1.0f/60);scene->flushStream();if(!scene->fetchResults(NX_RIGID_BODY_FINISHED,true))throw std::runtime_error("PhysX fetchResults failed");
 cal::Sample s;NxVec3 p=car->getGlobalPosition(),v=car->getLinearVelocity(),m=car->getAngularMomentum();NxQuat q=car->getGlobalOrientationQuat();
 s.position=cal::V(p.x,p.y,p.z);s.velocity=cal::V(v.x,v.y,v.z);s.momentum=cal::V(m.x,m.y,m.z);s.q[0]=q.x;s.q[1]=q.y;s.q[2]=q.z;s.q[3]=q.w;
 for(int i=0;i<4;++i){NxWheelContactData contact;bool hit=wheels[i]->getContact(contact)!=0;s.wheelSpeed.push_back(wheels[i]->getAxleSpeed());s.contacts.push_back(hit?1:0);s.wheelHeight.push_back(hit?contact.contactPoint.z:0);}
 cal::sample(c,tick,x,s);
 }
 scene->releaseActor(*car);sdk->releaseScene(*scene);
}
int main(int argc,char**argv){try{NxSDKCreateError error;sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk){std::ostringstream why;why<<"PhysX 2.8.4 SDK creation failed: error "<<error;throw std::runtime_error(why.str());}sdk->setParameter(NX_SKIN_WIDTH,.025f);int r=cal::run(argc,argv,execute,"windows-reference");sdk->release();return r;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';if(sdk)sdk->release();return 2;}}
