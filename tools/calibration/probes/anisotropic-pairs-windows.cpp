#include <NxPhysics.h>
#include <iostream>
#include <iomanip>
#include <cstring>
struct Pair { const char* name; float u0,v0,u1,v1,roll0,roll1; bool pole; };
static const Pair cases[]={
 {"scalar-half",.08f,2,.5f,.5f,0,0,false},
 {"scalar-four",.08f,2,4,4,0,0,false},
 {"scalar-four-static",.08f,2,4,4,0,0,false},
 {"max-four",.08f,2,4,4,0,0,false},
 {"max-four-static",.08f,2,4,4,0,0,false},
 {"asymmetric-v",.08f,2,.3f,1,0,0,false},
 {"asymmetric-v-inverted",.3f,1,.08f,2,0,35,false},
 {"low-average",.1f,.4f,.2f,.3f,0,0,false},
 {"wide-average",.1f,3,.2f,.4f,0,0,false},
 {"wide-average-inverted",.2f,.4f,.1f,3,0,35,false},
 {"negative-difference",.8f,.1f,.7f,.2f,0,0,false},
 {"scalar-high-primary",2,4,3,3,0,0,false},

 {"same-side",.08f,2,.08f,2,0,0,false},
 {"side-v-half",.08f,.5f,.08f,.5f,0,0,false},
 {"side-v-one",.08f,1,.08f,1,0,0,false},
 {"side-v-four",.08f,4,.08f,4,0,0,false},
 {"side-all-high",2,4,2,4,0,0,false},
 {"side-decoration",.08f,2,.5f,.5f,0,0,false},
 {"side-high-decoration",.08f,2,4,4,0,0,false},
 {"same-pole",.08f,2,.08f,2,0,0,true},
 {"first-stronger",.08f,2,.3f,2,0,35,false},
 {"second-stronger",.3f,2,.08f,2,0,35,false},
 {"same-rotated-first",.08f,2,.08f,2,35,0,false},
 {"same-rotated-second",.08f,2,.08f,2,0,35,false},
 {"first-stronger-swapped",.3f,2,.08f,2,35,0,false},
 {"second-stronger-swapped",.08f,2,.3f,2,35,0,false}
};
class Contact: public NxUserContactModify {
public:
 const char* name; int tick,printed; Contact(const char* n):name(n),tick(0),printed(0){}
 virtual bool onContactConstraint(NxU32&,const NxShape* a,const NxShape*,NxU32,NxU32,NxContactCallbackData& d){
  if(printed++>=8)return true;
  NxMat33 local(d.localorientation0),frame=a->getActor().getCMassGlobalPose().M*local;
  std::cout<<"{\"case\":\""<<name<<"\",\"tick\":"<<tick<<",\"dynamic_friction\":["<<d.dynamicFriction0<<","<<d.dynamicFriction1<<"],\"static_friction\":["<<d.staticFriction0<<","<<d.staticFriction1<<"],\"axes\":[";
  for(int i=0;i<3;++i){NxVec3 v=frame.getColumn(i);if(i)std::cout<<",";std::cout<<"["<<v.x<<","<<v.y<<","<<v.z<<"]";}
  std::cout<<"]}"<<std::endl;return true;
 }
};
int main(){
 std::cout<<std::setprecision(9);NxSDKCreateError error;NxPhysicsSDK* sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk)return 2;
 sdk->setParameter(NX_ADAPTIVE_FORCE,0);sdk->setParameter(NX_SKIN_WIDTH,.025f);
 for(unsigned n=0;n<sizeof(cases)/sizeof(cases[0]);++n){const Pair& p=cases[n];Contact callback(p.name);NxSceneDesc sceneDesc;sceneDesc.gravity=NxVec3(0,0,0);sceneDesc.userContactModify=&callback;sceneDesc.flags=0;sceneDesc.upAxis=2;sceneDesc.timeStepMethod=NX_TIMESTEP_VARIABLE;sceneDesc.simType=NX_SIMULATION_SW;NxScene* scene=sdk->createScene(sceneDesc);if(!scene)return 3;
  NxMaterial* firstMaterial=0;NxActor* actors[2];for(int i=0;i<2;++i){NxMaterialDesc m;m.staticFriction=m.dynamicFriction=i?p.u1:p.u0;m.dynamicFrictionV=i?p.v1:p.v0;m.staticFrictionV=3.2f;m.restitution=0;m.dirOfAnisotropy=NxVec3(0,0,1);m.flags=((i==1&&(std::strstr(p.name,"scalar-")==p.name||std::strstr(p.name,"max-")==p.name))?0:NX_MF_ANISOTROPIC)|NX_MF_DISABLE_STRONG_FRICTION;m.frictionCombineMode=(i==1&&std::strstr(p.name,"max-")==p.name)?NX_CM_MAX:NX_CM_MIN;NxMaterial* material=scene->createMaterial(m);if(!material)return 4;if(i==0)firstMaterial=material;else if(p.u0==p.u1&&p.v0==p.v1)material=firstMaterial;
   std::cout<<"{\"case\":\""<<p.name<<"\",\"event\":\"material\",\"actor\":"<<i<<",\"u\":"<<material->getDynamicFriction()<<",\"v\":"<<material->getDynamicFrictionV()<<"}"<<std::endl;
   NxBoxShapeDesc box;box.dimensions=NxVec3(1.5f,.8f,.3f);box.skinWidth=.025f;box.materialIndex=material->getMaterialIndex();NxBodyDesc body;body.mass=1200;body.linearDamping=0;body.angularDamping=.05f;body.sleepEnergyThreshold=0;body.solverIterationCount=4;body.flags|=NX_BF_FROZEN_ROT;
   NxActorDesc a;a.shapes.pushBack(&box);a.body=(i==1&&std::strstr(p.name,"-static"))?0:&body;NxMaterialDesc wm;wm.flags=NX_MF_DISABLE_FRICTION;NxMaterial* wheelMaterial=scene->createMaterial(wm);NxWheelShapeDesc wheels[4];
   for(int w=0;w<4;++w){wheels[w].materialIndex=wheelMaterial->getMaterialIndex();wheels[w].group=4;wheels[w].localPose.t=NxVec3(w<2?1.1f:-1.1f,w%2==0?.75f:-.75f,100);wheels[w].localPose.M.setColumn(0,NxVec3(0,1,0));wheels[w].localPose.M.setColumn(1,NxVec3(0,0,1));wheels[w].localPose.M.setColumn(2,NxVec3(1,0,0));wheels[w].radius=.4f;wheels[w].suspensionTravel=.5f;wheels[w].suspension.spring=35000;wheels[w].suspension.damper=4500;wheels[w].inverseWheelMass=.1f;wheels[w].wheelFlags=64;wheels[w].skinWidth=.025f;if(a.body)a.shapes.pushBack(&wheels[w]);}
   a.flags=NX_AF_LOCK_COM|NX_AF_CONTACT_MODIFICATION;a.globalPose.t=p.pole?NxVec3(0,0,10+i*4):NxVec3(i*4,0,10);NxQuat q;q.fromAngleAxis(i?p.roll1:p.roll0,NxVec3(1,0,0));a.globalPose.M.fromQuat(q);actors[i]=scene->createActor(a);if(!actors[i])return 5;
  }
  actors[0]->setLinearVelocity(p.pole?NxVec3(0,0,4):NxVec3(4,0,0));
  for(int tick=0;tick<90;++tick){callback.tick=tick;scene->simulate(1.f/60);scene->flushStream();if(!scene->fetchResults(NX_RIGID_BODY_FINISHED,true))return 6;}
  if(callback.printed==0)return 7;sdk->releaseScene(*scene);
 }
 sdk->release();return 0;
}
