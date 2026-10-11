#include <NxPhysics.h>
#include "Fixture.h"
#include <iostream>
#include "EulerAngles.h"
#include "WindowsReferenceGeometry.h"
namespace cal=motorrock::calibration;
static NxPhysicsSDK* sdk=0;
static NxCookingInterface* cooking=0;
static NxVec3 v3(cal::V v){return NxVec3((float)v.x,(float)v.y,(float)v.z);}
static NxTireFunctionDesc tire(const cal::Tire& t){NxTireFunctionDesc d;d.extremumSlip=(float)t.extremumSlip;d.extremumValue=(float)t.extremumValue;d.asymptoteSlip=(float)t.asymptoteSlip;d.asymptoteValue=(float)t.asymptoteValue;d.stiffnessFactor=(float)t.stiffness;return d;}
// Same neutral GameCar wheel-contact budget (no clutch/tireSpring effects).
struct WheelContact : NxUserWheelContactModify {
 float gravity,tireSpring;bool traction; explicit WheelContact(const cal::Case& c):gravity((float)c.gravity),tireSpring((float)c.tireSpring),traction(c.traction){}
 virtual bool onWheelContact(NxWheelShape* w,NxVec3&,NxVec3& normal,NxReal&,NxReal& force,NxShape*,NxMaterialIndex&,NxU32) {
  const float load=std::abs(w->getActor().getMass()*normal.z*gravity/4);
  force=!traction || (tireSpring>0 && force>load*tireSpring)?0:std::min(force,load*1.5f);return true;
 }
};
static void execute(const cal::Case& c) {
 CalibrationTrackContact bodyContacts(c.name);CalibrationBodyReport bodyReport(c.name);
 NxSceneDesc desc;desc.userContactModify=&bodyContacts;desc.userContactReport=&bodyReport;desc.gravity=NxVec3(0,0,(float)c.gravity);desc.upAxis=2;desc.timeStepMethod=NX_TIMESTEP_VARIABLE;desc.simType=NX_SIMULATION_SW;
 NxScene* scene=sdk->createScene(desc);if(!scene)throw std::runtime_error("PhysX scene creation failed");
 NxMaterial* material=scene->getMaterialFromIndex(0);material->setStaticFriction(.5f);material->setDynamicFriction(.5f);material->setRestitution(.5f);
 NxMaterialDesc carMat;carMat.staticFriction=carMat.dynamicFriction=(float)c.bodyFriction;carMat.restitution=0;carMat.staticFrictionV=3.2f;carMat.dynamicFrictionV=2;carMat.dirOfAnisotropy=NxVec3(0,0,1);carMat.flags=NX_MF_ANISOTROPIC|NX_MF_DISABLE_STRONG_FRICTION;carMat.frictionCombineMode=NX_CM_MIN;
 NxMaterial* carMaterial=scene->createMaterial(carMat);
 NxTriangleMesh* groundMesh=0;NxTriangleMesh* borderMesh=0;
 if(c.ground){NxVec3 points[4];const float xs[]={-500,500,500,-500},ys[]={-500,-500,500,500};for(int i=0;i<4;++i)points[i]=NxVec3(xs[i],ys[i],(float)(10+c.slopeX*xs[i]+c.slopeY*ys[i]));NxU32 indices[]={0,1,2,0,2,3};NxTriangleMeshDesc mesh;mesh.numVertices=4;mesh.pointStrideBytes=sizeof(NxVec3);mesh.points=points;mesh.numTriangles=2;mesh.triangleStrideBytes=3*sizeof(NxU32);mesh.triangles=indices;CalibrationStream stream;if(!cooking->NxCookTriangleMesh(mesh,stream))throw std::runtime_error("Ground mesh cooking failed");groundMesh=sdk->createTriangleMesh(stream);if(!groundMesh)throw std::runtime_error("Ground mesh creation failed");NxMaterialDesc mat;mat.staticFriction=mat.dynamicFriction=.1f;mat.restitution=0;NxMaterial* trackMat=scene->createMaterial(mat);NxTriangleMeshShapeDesc floor;floor.meshData=groundMesh;floor.materialIndex=trackMat->getMaterialIndex();NxActorDesc actor;actor.shapes.pushBack(&floor);if(!scene->createActor(actor))throw std::runtime_error("Floor creation failed");}
 NxMaterialDesc wheelMat;wheelMat.flags=NX_MF_DISABLE_FRICTION;wheelMat.restitution=0;NxMaterial* wheelMaterial=scene->createMaterial(wheelMat);

 NxBoxShapeDesc box;box.shapeFlags |= NX_SF_POINT_CONTACT_FORCE;box.dimensions=v3(c.halfExtents);box.localPose.t=v3(c.shapePosition);box.skinWidth=(float)c.bodySkinWidth;box.materialIndex=carMaterial->getMaterialIndex();
 NxBodyDesc body;body.mass=(float)c.mass;body.linearDamping=0;body.angularDamping=.05f;body.sleepEnergyThreshold=(float)c.sleepThreshold;body.massLocalPose.t=v3(c.centerOfMass);body.solverIterationCount=4;
 if(c.fixedOrientation)body.flags|=NX_BF_FROZEN_ROT;
 WheelContact callback(c);NxWheelShapeDesc wheelDescriptions[4];NxWheelShape* wheels[4];
 NxActorDesc actor;actor.body=&body;actor.flags=NX_AF_LOCK_COM|NX_AF_CONTACT_MODIFICATION;actor.shapes.pushBack(&box);actor.globalPose.t=NxVec3((float)c.startOffset.x,(float)c.startOffset.y,(float)(c.height+c.startOffset.z));
 for(int i=0;i<4;++i){NxWheelShapeDesc& w=wheelDescriptions[i];w.materialIndex=wheelMaterial->getMaterialIndex();w.group=4;const cal::Wheel& source=c.wheels[i];w.localPose.t=v3(source.position);w.localPose.M.setColumn(0,NxVec3(0,1,0));w.localPose.M.setColumn(1,NxVec3(0,0,1));w.localPose.M.setColumn(2,NxVec3(1,0,0));w.radius=(float)source.radius;w.suspensionTravel=(float)source.travel;w.suspension.spring=(float)source.spring;w.suspension.damper=(float)source.damper;w.suspension.targetValue=(float)source.target;w.inverseWheelMass=(float)source.inverseMass;w.wheelFlags=source.flags;w.longitudalTireForceFunction=tire(source.longitudinal);w.lateralTireForceFunction=tire(source.lateral);w.skinWidth=.025f;w.wheelContactModify=&callback;
 actor.shapes.pushBack(&w);}
 if(c.pair){NxQuat q;q.fromAngleAxisFast((float)c.primaryRoll,NxVec3(1,0,0));actor.globalPose.M.fromQuat(q);}
 if(c.initialPitch!=0){NxQuat q;q.fromAngleAxisFast((float)c.initialPitch,NxVec3(0,1,0));actor.globalPose.M.fromQuat(q);}
 NxActor* car=scene->createActor(actor);if(!car)throw std::runtime_error("Car creation failed");car->userData=reinterpret_cast<void*>(1);
 car->setContactReportFlags(NX_NOTIFY_ON_START_TOUCH|NX_NOTIFY_ON_TOUCH|NX_NOTIFY_FORCES);
 for(int i=0;i<4;++i){wheels[i]=car->getShapes()[i+1]->isWheel();if(!wheels[i])throw std::runtime_error("Wheel shape order changed");}
 NxActor* peer=0;std::vector<NxWheelShapeDesc> peerWheelDescriptions;peerWheelDescriptions.reserve(4);NxWheelShape* peerWheels[4];
 if(c.pair){NxBodyDesc peerBody=body;peerBody.mass=(float)c.peerMass;NxBoxShapeDesc peerBox=box;
  if(c.peerFriction!=c.bodyFriction){NxMaterialDesc peerMat=carMat;peerMat.staticFriction=peerMat.dynamicFriction=(float)c.peerFriction;peerBox.materialIndex=scene->createMaterial(peerMat)->getMaterialIndex();}
  NxActorDesc peerActor=actor;peerActor.body=&peerBody;peerActor.shapes.clear();peerActor.shapes.pushBack(&peerBox);peerActor.globalPose.t=v3(c.peerPosition);NxQuat q;q.fromAngleAxisFast((float)c.peerRoll,NxVec3(1,0,0));peerActor.globalPose.M.fromQuat(q);
  // NxShapeDesc::operator= is deliberately a no-op. Copy construction also
  // preserves local pose, material, collision group and shape flags.
  for(int i=0;i<4;++i){peerWheelDescriptions.push_back(wheelDescriptions[i]);peerActor.shapes.pushBack(&peerWheelDescriptions[i]);}
  peer=scene->createActor(peerActor);if(!peer)throw std::runtime_error("Peer car creation failed");peer->userData=reinterpret_cast<void*>(2);peer->setCMassOffsetLocalPose(peerBody.massLocalPose);peer->setLinearVelocity(v3(c.peerVelocity));
  for(int i=0;i<4;++i){peerWheels[i]=peer->getShapes()[i+1]->isWheel();if(!peerWheels[i])throw std::runtime_error("Peer wheel shape order changed");}
 }
 if(c.obstacle==7||c.obstacle==8||c.obstacle==9){NxVec3 points[4];
  if(c.obstacle!=8){points[0]=NxVec3(8,-10,10);points[1]=NxVec3(8,10,10);points[2]=NxVec3(8,10,16);points[3]=NxVec3(8,-10,16);}else{points[0]=NxVec3(-500,-500,10);points[1]=NxVec3(500,-500,10);points[2]=NxVec3(500,500,10);points[3]=NxVec3(-500,500,10);}
  NxU32 indices[]={0,1,2,0,2,3};if(c.obstacle==9){std::swap(indices[1],indices[2]);std::swap(indices[4],indices[5]);}NxTriangleMeshDesc mesh;mesh.numVertices=4;mesh.pointStrideBytes=sizeof(NxVec3);mesh.points=points;mesh.numTriangles=2;mesh.triangleStrideBytes=3*sizeof(NxU32);mesh.triangles=indices;CalibrationStream stream;if(!cooking->NxCookTriangleMesh(mesh,stream))throw std::runtime_error("Border mesh cooking failed");borderMesh=sdk->createTriangleMesh(stream);if(!borderMesh)throw std::runtime_error("Border mesh creation failed");
  NxMaterialDesc mat;mat.staticFriction=.1f;mat.dynamicFriction=4;mat.restitution=0;mat.frictionCombineMode=NX_CM_MAX;NxMaterial* borderMat=scene->createMaterial(mat);NxTriangleMeshShapeDesc border;border.meshData=borderMesh;border.materialIndex=borderMat->getMaterialIndex();NxActorDesc a;a.shapes.pushBack(&border);if(!scene->createActor(a))throw std::runtime_error("Border actor creation failed");
 }
 else if(c.obstacle==5){NxBoxShapeDesc o;o.dimensions=NxVec3(20,20,.25f);NxActorDesc a;a.shapes.pushBack(&o);a.globalPose.t=NxVec3(0,0,9.75f);if(!scene->createActor(a))throw std::runtime_error("Friction floor creation failed");}
 else if(c.obstacle==6){NxBoxShapeDesc o;o.dimensions=NxVec3(10,.25f,3);NxActorDesc a;a.shapes.pushBack(&o);a.globalPose.t=NxVec3(0,8,13);if(!scene->createActor(a))throw std::runtime_error("Lateral wall creation failed");}
 else if(c.obstacle){NxBoxShapeDesc o;o.dimensions=c.obstacle<=2?NxVec3(.25f,10,3):c.obstacle==3?NxVec3(.5f,10,.15f):NxVec3(20,.25f,.3f);NxActorDesc a;a.shapes.pushBack(&o);a.globalPose.t=c.obstacle<=2?NxVec3(8,0,13):c.obstacle==3?NxVec3(8,0,10.15f):NxVec3(8,.9f,10.3f);if(!scene->createActor(a))throw std::runtime_error("Obstacle creation failed");}
 car->setCMassOffsetLocalPose(body.massLocalPose);
 std::ostringstream actual;actual<<"{\"mass\":"<<car->getMass()<<",\"inertia\":"<<cal::vector(cal::V(car->getMassSpaceInertiaTensor().x,car->getMassSpaceInertiaTensor().y,car->getMassSpaceInertiaTensor().z))<<",\"com\":"<<cal::vector(cal::V(car->getCMassLocalPosition().x,car->getCMassLocalPosition().y,car->getCMassLocalPosition().z))<<'}';motorrock::parity::event("calibration.engine-description",c.name,actual.str());

 if(peer){std::ostringstream p;p<<"{\"mass\":"<<peer->getMass()<<",\"inertia\":"<<cal::vector(cal::V(peer->getMassSpaceInertiaTensor().x,peer->getMassSpaceInertiaTensor().y,peer->getMassSpaceInertiaTensor().z))<<",\"com\":"<<cal::vector(cal::V(peer->getCMassLocalPosition().x,peer->getCMassLocalPosition().y,peer->getCMassLocalPosition().z))<<'}';motorrock::parity::event("calibration.peer-description",c.name,p.str());}
 car->setLinearVelocity(NxVec3((float)c.velocity.x,(float)c.velocity.y,(float)c.velocity.z));car->setAngularVelocity(NxVec3((float)c.angular.x,(float)c.angular.y,(float)c.angular.z));
 for(int tick=0;tick<c.steps;++tick){bodyContacts.tick=bodyReport.tick=tick;const cal::Command x=cal::command(c,tick);
 if(c.extraGravity)car->addForce(NxVec3(0,0,(float)c.gravity),NX_ACCELERATION);
 if(c.airPitch)car->addLocalTorque(NxVec3(0,(float)c.airPitch,0),NX_ACCELERATION);
 if(c.angularDamping.x!=1 || c.angularDamping.y!=1 || c.angularDamping.z!=1 || c.clampRoll>0 || c.clampPitch>0){
 NxVec3 momentum=car->getAngularMomentum();NxMat33 mat=car->getGlobalOrientation(),inverse;
 if(mat.getInverse(inverse)){momentum=inverse*momentum;bool contact=false;for(int w=0;w<4;++w){NxWheelContactData data;contact=contact||wheels[w]->getContact(data)!=0;}NxVec3 inertia=car->getMassSpaceInertiaTensor();if(!contact){if(c.clampRoll>0)momentum.x=std::max(-(float)(inertia.x*c.clampRoll*2),std::min((float)(inertia.x*c.clampRoll*2),momentum.x));if(c.clampPitch>0)momentum.y=std::max(-(float)(inertia.y*c.clampPitch*2),std::min((float)(inertia.y*c.clampPitch*2),momentum.y));}momentum=mat*NxVec3(momentum.x*(float)c.angularDamping.x,momentum.y*(float)c.angularDamping.y,momentum.z*(float)c.angularDamping.z);
 if(c.clampRoll>0 || c.clampPitch>0){NxQuat q=car->getGlobalOrientationQuat();Quat source;source.x=q.x;source.y=q.y;source.z=q.z;source.w=q.w;EulerAngles angles=Eul_FromQuat(source,EulOrdXYZs);if(c.clampRoll>0)angles.x=std::max(-(float)c.clampRoll,std::min((float)c.clampRoll,angles.x));if(c.clampPitch>0)angles.y=std::max(-(float)c.clampPitch,std::min((float)c.clampPitch,angles.y));Quat result=Eul_ToQuat(angles);q.setXYZW(result.x,result.y,result.z,result.w);car->setGlobalOrientationQuat(q);}}
 car->setAngularMomentum(momentum);}
 car->addForce(NxVec3((float)x.acceleration.x,(float)x.acceleration.y,(float)x.acceleration.z),NX_ACCELERATION);
 if(x.impulse.x||x.impulse.y||x.impulse.z)car->addForce(NxVec3((float)x.impulse.x,(float)x.impulse.y,(float)x.impulse.z),NX_VELOCITY_CHANGE);
 for(int i=0;i<4;++i){wheels[i]->setMotorTorque(c.wheels[i].driven?(float)x.torque:0);wheels[i]->setBrakeTorque((float)x.brake);wheels[i]->setSteerAngle(c.wheels[i].steering?(float)x.steer:0);}
 scene->simulate(1.0f/60);scene->flushStream();if(!scene->fetchResults(NX_RIGID_BODY_FINISHED,true))throw std::runtime_error("PhysX fetchResults failed");
 cal::Sample s;NxActor* observed=c.pair&&c.watched==1?peer:car;NxWheelShape** observedWheels=c.pair&&c.watched==1?peerWheels:wheels;NxVec3 p=observed->getGlobalPosition(),v=observed->getLinearVelocity(),m=observed->getAngularMomentum();NxQuat q=observed->getGlobalOrientationQuat();
 s.position=cal::V(p.x,p.y,p.z);s.velocity=cal::V(v.x,v.y,v.z);s.momentum=cal::V(m.x,m.y,m.z);s.q[0]=q.x;s.q[1]=q.y;s.q[2]=q.z;s.q[3]=q.w;
 for(int i=0;i<4;++i){NxWheelContactData contact;bool hit=observedWheels[i]->getContact(contact)!=0;s.wheelSpeed.push_back(observedWheels[i]->getAxleSpeed());s.contacts.push_back(hit?1:0);s.wheelHeight.push_back(hit?contact.contactPoint.z:0);s.longSlip.push_back(hit?contact.longitudalSlip:0);s.latSlip.push_back(hit?contact.lateralSlip:0);s.suspension.push_back(hit?contact.contactPosition-observedWheels[i]->getRadius():0);s.normalForce.push_back(hit?contact.contactForce:0);s.longImpulse.push_back(hit?contact.longitudalImpulse:0);s.latImpulse.push_back(hit?contact.lateralImpulse:0);}
 cal::sample(c,tick,x,s);
 }
 if(peer)scene->releaseActor(*peer);scene->releaseActor(*car);sdk->releaseScene(*scene);if(groundMesh)sdk->releaseTriangleMesh(*groundMesh);if(borderMesh)sdk->releaseTriangleMesh(*borderMesh);
}
int main(int argc,char**argv){try{NxSDKCreateError error;sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk){std::ostringstream why;why<<"PhysX 2.8.4 SDK creation failed: error "<<error;throw std::runtime_error(why.str());}sdk->setParameter(NX_ADAPTIVE_FORCE,0);sdk->setParameter(NX_SKIN_WIDTH,.025f);cooking=NxGetCookingLib(NX_PHYSICS_SDK_VERSION);if(!cooking || !cooking->NxInitCooking())throw std::runtime_error("PhysX cooking initialization failed");int r=cal::run(argc,argv,execute,"windows-reference");cooking->NxCloseCooking();sdk->release();return r;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';if(sdk)sdk->release();return 2;}}
