#include <NxPhysics.h>
#include <windows.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstring>
#include "NativeGameStartupV130.h"
#include "build-id.h"
static DWORD jump=0;
static BOOL CALLBACK method(PSYMBOL_INFO s,ULONG,void*){if(strcmp(s->Name,"r3d::game::GameCar::JumpProgress")==0)jump=(DWORD)s->Address-nativegamev130::base;return TRUE;}
static unsigned word(float v){unsigned u;memcpy(&u,&v,4);return u;}
static float value(unsigned u){float v;memcpy(&v,&u,4);return v;}
static void vec(std::ostream&o,NxVec3 v){o<<'['<<word(v.x)<<','<<word(v.y)<<','<<word(v.z)<<']';}
static void quat(std::ostream&o,NxQuat q){o<<'['<<word(q.x)<<','<<word(q.y)<<','<<word(q.z)<<','<<word(q.w)<<']';}
template<class T>static void field(void*object,const wchar_t*name,const T&v){ULONG offset=nativegamev130::field(nativegamev130::type("r3d::game::GameCar"),name);memcpy((char*)object+offset,&v,sizeof(v));}
static float scalar(void*object,const wchar_t*name){float v;ULONG offset=nativegamev130::field(nativegamev130::type("r3d::game::GameCar"),name);memcpy(&v,(char*)object+offset,4);return v;}
int main(int argc,char**argv){try{
 if(argc!=3)return 2;std::ifstream in(argv[1]);std::ofstream out(argv[2]);if(!in||!out)throw std::runtime_error("Cannot open fixture/output");
 out<<"{\"event\":\"observer.build\",\"source_sha256\":\""<<kProbeSourceSha<<"\",\"build_id\":\""<<kProbeBuildId<<"\"}\n";
 nativegamev130::init();SymEnumSymbols(nativegamev130::process,nativegamev130::base,"*",method,0);if(jump!=0x498110)throw std::runtime_error("Unexpected original JumpProgress RVA");
 NxSDKCreateError error;NxPhysicsSDK*sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk)throw std::runtime_error("SDK failed");
 const char*names[]={"Rock3dGame.dll","PhysXCore.dll","PhysXLoader.dll"};for(unsigned i=0;i<3;++i){char p[MAX_PATH]={0};if(!GetModuleFileNameA(GetModuleHandleA(names[i]),p,MAX_PATH))throw std::runtime_error("Module missing");out<<"{\"event\":\"runtime.module\",\"name\":\""<<names[i]<<"\",\"path\":\"";for(char*c=p;*c;++c){if(*c=='\\'||*c=='\"')out<<'\\';out<<*c;}out<<"\"}\n";}
 unsigned id,ct,u;
 while(in>>id>>ct){float x[8];for(unsigned i=0;i<8;++i){if(!(in>>u))throw std::runtime_error("Truncated fixture");x[i]=value(u);}const float mass=x[0],gravity=x[1],pitch=x[5],spring=x[6],methodDelta=x[7];NxVec3 initialVelocity(x[2],x[3],x[4]);
  NxSceneDesc sd;sd.gravity=NxVec3(0,0,gravity);sd.timeStepMethod=NX_TIMESTEP_VARIABLE;NxScene*scene=sdk->createScene(sd);if(!scene)throw std::runtime_error("Scene failed");
  NxBodyDesc bd;bd.mass=mass;bd.linearDamping=bd.angularDamping=0;bd.sleepEnergyThreshold=1;NxBoxShapeDesc box;box.dimensions=NxVec3(1,1,.25f);NxActorDesc ad;ad.flags=NX_AF_LOCK_COM;ad.body=&bd;ad.globalPose.t=NxVec3(0,0,5);ad.shapes.pushBack(&box);NxWheelShapeDesc wheels[4];
  for(unsigned i=0;i<4;++i){NxWheelShapeDesc&w=wheels[i];w.localPose.t=NxVec3(i<2?1.f:-1.f,i%2?1.f:-1.f,-.5f);NxQuat q;q.setXYZW(.5f,.5f,.5f,.5f);w.localPose.M.fromQuat(q);w.radius=.5f;w.suspensionTravel=.4f;w.suspension.spring=w.suspension.damper=0;w.inverseWheelMass=1;w.wheelFlags=0;w.longitudalTireForceFunction.stiffnessFactor=w.lateralTireForceFunction.stiffnessFactor=0;ad.shapes.pushBack(&w);}
  _controlfp(_PC_24,_MCW_PC);NxActor*actor=scene->createActor(ad);if(!actor)throw std::runtime_error("Actor failed");actor->setLinearVelocity(initialVelocity);
  {nativegamev130::Control car(*actor);const bool contact=ct!=0;field(car.object,L"_anyWheelContact",contact);field(car.object,L"_springTime",spring);field(car.object,L"_flyYTorque",pitch);_controlfp(_PC_24,_MCW_PC);unsigned short cw=0;__asm fnstcw cw
   out<<"{\"event\":\"jump.sample\",\"case\":"<<id<<",\"contact\":"<<(contact?"true":"false")<<",\"method_delta_word\":"<<word(methodDelta)<<",\"integration_delta_word\":"<<word(1.f/60)<<",\"gravity_word\":"<<word(gravity)<<",\"pitch_word\":"<<word(pitch)<<",\"spring_before_word\":"<<word(spring)<<",\"mass_word\":"<<word(actor->getMass())<<",\"inertia\":";vec(out,actor->getMassSpaceInertiaTensor());out<<",\"velocity_before\":";vec(out,actor->getLinearVelocity());out<<",\"rotation_before\":";quat(out,actor->getGlobalOrientationQuat());out<<",\"raw_fp_control\":"<<cw<<",\"method_rva\":"<<jump;
   typedef void(__thiscall*Jump)(void*,float);((Jump)(nativegamev130::base+jump))(car.object,methodDelta);
   out<<",\"spring_after_word\":"<<word(scalar(car.object,L"_springTime"))<<",\"velocity_after_method\":";vec(out,actor->getLinearVelocity());out<<",\"omega_after_method\":";vec(out,actor->getAngularVelocity());
   scene->simulate(1.f/60);scene->flushStream();if(!scene->fetchResults(NX_RIGID_BODY_FINISHED,true))throw std::runtime_error("Fetch failed");
   out<<",\"position_after_step\":";vec(out,actor->getGlobalPosition());out<<",\"velocity_after_step\":";vec(out,actor->getLinearVelocity());out<<",\"rotation_after_step\":";quat(out,actor->getGlobalOrientationQuat());out<<",\"omega_after_step\":";vec(out,actor->getAngularVelocity());out<<",\"momentum_after_step\":";vec(out,actor->getAngularMomentum());out<<",\"awake_after_step\":"<<(actor->isSleeping()?"false":"true")<<"}\n";out.flush();
  }sdk->releaseScene(*scene);
 }
 if(!in.eof())throw std::runtime_error("Invalid fixture tail");sdk->release();return out.good()?0:4;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 3;}}
