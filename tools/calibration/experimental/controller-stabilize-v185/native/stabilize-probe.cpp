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
static DWORD stabilize=0;
static BOOL CALLBACK method(PSYMBOL_INFO s,ULONG,void*) {if(strcmp(s->Name,"r3d::game::GameCar::StabilizeForce")==0)stabilize=(DWORD)s->Address-nativegamev130::base;return TRUE;}
struct Checkpoint {DWORD address;BYTE byte;bool hit;DWORD control;unsigned words[32];};
static Checkpoint checkpoints[9];
static void patch(Checkpoint&c,bool arm){DWORD old;VirtualProtect((void*)c.address,1,PAGE_EXECUTE_READWRITE,&old);*(BYTE*)c.address=arm?0xcc:c.byte;FlushInstructionCache(GetCurrentProcess(),(void*)c.address,1);DWORD ignored;VirtualProtect((void*)c.address,1,old,&ignored);}
static LONG CALLBACK observe(EXCEPTION_POINTERS*e){if(e->ExceptionRecord->ExceptionCode!=EXCEPTION_BREAKPOINT)return EXCEPTION_CONTINUE_SEARCH;for(int i=0;i<9;++i){Checkpoint&c=checkpoints[i];if((DWORD)e->ExceptionRecord->ExceptionAddress!=c.address)continue;c.hit=true;c.control=e->ContextRecord->FloatSave.ControlWord;DWORD fp=e->ContextRecord->Ebp;if(i==0){memcpy(c.words,(void*)(fp-0x54),36);memcpy(c.words+9,(void*)(fp-0x80),36);memcpy(c.words+18,(void*)(fp-0x28),12);}else if(i==3){for(int j=0;j<9;++j)memcpy(c.words+j,(void*)(fp-0x14-j*12),4);memcpy(c.words+9,(void*)(fp-0x80),4);}else if(i==6)memcpy(c.words,(void*)(fp-0x44),64);else if(i>=7){for(int j=0;j<(i==7?6:10);++j)memcpy(c.words+j*2,(void*)(fp-0x74-j*16),8);}else if(i==4)memcpy(c.words,(void*)(fp-0xd4),16);else if(i==5)memcpy(c.words,(void*)(fp-0xbc),16);else memcpy(c.words,(void*)(fp-0x28),12);patch(c,false);e->ContextRecord->Eip=c.address;return EXCEPTION_CONTINUE_EXECUTION;}return EXCEPTION_CONTINUE_SEARCH;}
static void arm(){const DWORD offsets[]={0x49847a,0x498496,0x498994,0x4aa815,0x498856,0x49895c,0x94dd0a,0x94cc45,0x94cc87};for(int i=0;i<9;++i){Checkpoint&c=checkpoints[i];c.address=nativegamev130::base+offsets[i];c.byte=*(BYTE*)c.address;c.hit=false;memset(c.words,0,sizeof(c.words));patch(c,true);}}
static void finish(std::ostream&out){for(int i=0;i<9;++i){Checkpoint&c=checkpoints[i];patch(c,false);out<<",\"checkpoint"<<i<<"_hit\":"<<(c.hit?"true":"false")<<",\"checkpoint"<<i<<"\":[";for(int j=0;j<(i==0?21:i==3?10:i==6?16:i==7?12:i==8?20:i>=4?4:3);++j){if(j)out<<',';out<<c.words[j];}out<<']'<<",\"checkpoint"<<i<<"_control\":"<<c.control;}}
static unsigned word(float f){unsigned u;memcpy(&u,&f,4);return u;}
static void vec(std::ostream&o,NxVec3 v){o<<'['<<word(v.x)<<','<<word(v.y)<<','<<word(v.z)<<']';}
static void quat(std::ostream&o,NxQuat q){o<<'['<<word(q.x)<<','<<word(q.y)<<','<<word(q.z)<<','<<word(q.w)<<']';}
template<class T>static void field(void*object,const wchar_t*name,const T&value){ULONG offset=nativegamev130::field(nativegamev130::type("r3d::game::GameCar"),name);memcpy((char*)object+offset,&value,sizeof(value));}
int main(int argc,char**argv){try{
 if(argc<2||argc>3)return 2;const bool boxOnly=argc==3;std::ofstream out(argv[1]);out<<"{\"event\":\"observer.build\",\"source_sha256\":\""<<kProbeSourceSha<<"\",\"build_id\":\""<<kProbeBuildId<<"\"}\n";
 nativegamev130::init();SymEnumSymbols(nativegamev130::process,nativegamev130::base,"*",method,0);AddVectoredExceptionHandler(1,observe);if(!stabilize)throw std::runtime_error("Original StabilizeForce missing");
 NxSDKCreateError error;NxPhysicsSDK*sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk)throw std::runtime_error("SDK failed");
 const char*names[]={"Rock3dGame.dll","PhysXCore.dll","PhysXLoader.dll"};for(int i=0;i<3;++i){char path[MAX_PATH]={0};HMODULE module=GetModuleHandleA(names[i]);if(!module||!GetModuleFileNameA(module,path,MAX_PATH))throw std::runtime_error("Loaded module missing");out<<"{\"event\":\"runtime.module\",\"name\":\""<<names[i]<<"\",\"path\":\"";for(char*p=path;*p;++p){if(*p=='\\'||*p=='\"')out<<'\\';out<<*p;}out<<"\"}\n";}
 const float angles[]={0,.2f,.7f,1.5707963705062866f,2.2f,3.1415927410125732f};
 const NxVec3 omegas[]={NxVec3(0,0,0),NxVec3(.3f,0,0),NxVec3(0,-.7f,0),NxVec3(0,0,1.3f),NxVec3(3.1f,-2.7f,1.9f)};
 const NxVec3 damps[]={NxVec3(1,1,1),NxVec3(.8f,.6f,.4f),NxVec3(0,0,0),NxVec3(1,1,1),NxVec3(.8f,.6f,.4f),NxVec3(-1,-1,-1)};
 const float rolls[]={0,0,0,.3f,.3f,0},pitches[]={0,0,0,.2f,.2f,0};
 unsigned c=0;
 for(int a=0;a<6;++a)for(int w=0;w<5;++w)for(int d=0;d<6;++d)for(int ct=0;ct<2;++ct)for(int cl=0;cl<4;++cl){
  NxSceneDesc sd;sd.gravity=NxVec3(0,0,0);sd.timeStepMethod=NX_TIMESTEP_VARIABLE;NxScene*scene=sdk->createScene(sd);if(!scene)throw std::runtime_error("Scene failed");
  NxBodyDesc body;body.mass=1000;body.angularDamping=body.linearDamping=0;NxBoxShapeDesc box;box.dimensions=NxVec3(1,1,.25f);NxActorDesc ad;ad.flags|=NX_AF_LOCK_COM;ad.body=&body;ad.globalPose.t=NxVec3(0,0,5);ad.shapes.pushBack(&box);NxWheelShapeDesc wheels[4];for(int i=0;i<(boxOnly?0:4);++i){wheels[i].localPose.t=NxVec3(i<2?1.f:-1.f,i%2?1.f:-1.f,-.5f);NxQuat wq;wq.setXYZW(.5f,.5f,.5f,.5f);wheels[i].localPose.M.fromQuat(wq);wheels[i].radius=.5f;wheels[i].suspensionTravel=.4f;wheels[i].suspension.spring=wheels[i].suspension.damper=0;wheels[i].inverseWheelMass=1;wheels[i].wheelFlags=0;wheels[i].longitudalTireForceFunction.stiffnessFactor=wheels[i].lateralTireForceFunction.stiffnessFactor=0;ad.shapes.pushBack(&wheels[i]);}_controlfp(_PC_24,_MCW_PC);NxActor*actor=scene->createActor(ad);if(!actor)throw std::runtime_error("Actor failed");
  NxQuat q;q.id();if(a){NxVec3 axis(1,2,3);axis.normalize();q.fromAngleAxisFast(angles[a],axis);}actor->setGlobalOrientationQuat(q);actor->setAngularVelocity(omegas[w]);
  {nativegamev130::Control car(*actor);field(car.object,L"_angDamping",damps[d]);field(car.object,L"_clampXTorque",rolls[d]);field(car.object,L"_clampYTorque",pitches[d]);const bool contact=ct!=0;field(car.object,L"_anyWheelContact",contact);const float clutchTime=cl?0.25f:0,clutchStrength=cl==2?.7f:cl==3?-.7f:0;field(car.object,L"_clutchTime",clutchTime);field(car.object,L"_clutchStrength",clutchStrength);_controlfp(_PC_24,_MCW_PC);
   unsigned short rawControl=0;__asm fnstcw rawControl;
   out<<"{\"event\":\"stabilize.pose\",\"case\":"<<c++<<",\"orientation_case\":"<<a<<",\"omega_case\":"<<w<<",\"damping_case\":"<<d<<",\"contact\":"<<(contact?"true":"false")<<",\"clutch_case\":"<<cl<<",\"clutch_strength_word\":"<<word(clutchStrength)<<",\"clutch_time_word\":"<<word(clutchTime)<<",\"roll_word\":"<<word(rolls[d])<<",\"pitch_word\":"<<word(pitches[d])<<",\"damping\":";vec(out,damps[d]);out<<",\"rotation_before\":";quat(out,actor->getGlobalOrientationQuat());out<<",\"omega_before\":";vec(out,actor->getAngularVelocity());out<<",\"momentum_before\":";vec(out,actor->getAngularMomentum());out<<",\"mass_word\":"<<word(actor->getMass())<<",\"inertia\":";vec(out,actor->getMassSpaceInertiaTensor());out<<",\"inverse_inertia\":";{NxVec3 inertia=actor->getMassSpaceInertiaTensor();vec(out,NxVec3(1.f/inertia.x,1.f/inertia.y,1.f/inertia.z));};out<<",\"fp_control\":"<<_controlfp(0,0)<<",\"raw_fp_control\":"<<rawControl<<",\"method_rva\":"<<stabilize;
   arm();typedef void(__thiscall*Stabilize)(void*,float);((Stabilize)(nativegamev130::base+stabilize))(car.object,1.f/60);finish(out);
   out<<",\"rotation_after\":";quat(out,actor->getGlobalOrientationQuat());out<<",\"omega_after\":";vec(out,actor->getAngularVelocity());out<<",\"momentum_after\":";vec(out,actor->getAngularMomentum());float strength=0;memcpy(&strength,car.object+nativegamev130::field(nativegamev130::type("r3d::game::GameCar"),L"_clutchStrength"),4);out<<",\"clutch_strength_after_word\":"<<word(strength);scene->simulate(1.f/60);scene->flushStream();scene->fetchResults(NX_RIGID_BODY_FINISHED,true);out<<",\"rotation_after_step\":";quat(out,actor->getGlobalOrientationQuat());out<<",\"omega_after_step\":";vec(out,actor->getAngularVelocity());out<<",\"momentum_after_step\":";vec(out,actor->getAngularMomentum());out<<",\"position_after_step\":";vec(out,actor->getGlobalPosition());out<<"}\n";out.flush();
  }sdk->releaseScene(*scene);
 }
 sdk->release();return out.good()?0:4;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 3;}}
