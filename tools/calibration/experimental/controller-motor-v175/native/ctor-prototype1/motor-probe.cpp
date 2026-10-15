#include <NxPhysics.h>
#include <windows.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstring>
#include "NativeGameStartupV130.h"
static DWORD wheelCtor=0,wheelDtor=0,createShape=0,addWheel=0,setLead=0,getShape=0,motorStep=0,transmission=0,setGear=0,getSpeed=0;
static BOOL CALLBACK method(PSYMBOL_INFO s,ULONG,void*){DWORD r=(DWORD)s->Address-nativegamev130::base,n=999;SymGetTypeInfo(nativegamev130::process,nativegamev130::base,s->TypeIndex,TI_GET_COUNT,&n);
#define M(name,slot) if(strcmp(s->Name,name)==0)slot=r
 if(strcmp(s->Name,"r3d::game::CarWheels::Add")==0 && n==1)addWheel=r;
 M("r3d::game::CarWheel::CarWheel",wheelCtor);M("r3d::game::CarWheel::`vbase destructor'",wheelDtor);if(strcmp(s->Name,"r3d::game::CarWheel::CreateWheelShape")==0&&n==1)createShape=r;
 M("r3d::game::CarWheel::SetLead",setLead);M("r3d::game::CarWheel::GetShape",getShape);M("r3d::game::GameCar::MotorProgress",motorStep);M("r3d::game::GameCar::TransmissionProgress",transmission);M("r3d::game::GameCar::SetCurGear",setGear);
 if(strcmp(s->Name,"r3d::game::GameCar::GetSpeed")==0&&n==1)getSpeed=r;
#undef M
 return TRUE;}
static unsigned word(float f){unsigned u;memcpy(&u,&f,4);return u;}
static void advance(NxScene*scene){scene->simulate(1.0f/60);scene->flushStream();scene->fetchResults(NX_RIGID_BODY_FINISHED,true);}
struct WheelBinding {nativegamev130::Control car;void*ownedWheel;NxShape**slot;WheelBinding(NxActor&actor,NxWheelShape&wheel):car(actor),ownedWheel(0),slot(0){NxActor**root=(NxActor**)((unsigned char*)car.actor+car.nxOffset);*root=0;
 ULONG gt=nativegamev130::type("r3d::game::GameCar");void*wheels=*(void**)(car.object+nativegamev130::field(gt,L"_wheels"));ownedWheel=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,nativegamev130::length(nativegamev130::type("r3d::game::CarWheel")));typedef void(__thiscall*WCtor)(void*,void*,int);((WCtor)(nativegamev130::base+wheelCtor))(ownedWheel,wheels,1);typedef void(__thiscall*Create)(void*);((Create)(nativegamev130::base+createShape))(ownedWheel);
 typedef void*(__thiscall*Get)(void*);void*shape=((Get)(nativegamev130::base+getShape))(ownedWheel);if(!shape)throw std::runtime_error("Original owned shape missing");slot=(NxShape**)((unsigned char*)shape+nativegamev130::field(nativegamev130::type("r3d::px::Shape"),L"_nxShape"));if(*slot)throw std::runtime_error("Unexpected owned SDK shape");*slot=&wheel;
 typedef void(__thiscall*Lead)(void*,bool);((Lead)(nativegamev130::base+setLead))(ownedWheel,true);*root=&actor;}
 ~WheelBinding(){if(slot)*slot=0;if(ownedWheel){typedef void(__thiscall*Dtor)(void*);((Dtor)(nativegamev130::base+wheelDtor))(ownedWheel);HeapFree(GetProcessHeap(),0,ownedWheel);}}
};
int main(int argc,char**argv){try{if(argc!=2)return 2;std::ofstream out(argv[1]);nativegamev130::init();SymEnumSymbols(nativegamev130::process,nativegamev130::base,"*",method,0);if(!wheelCtor||!wheelDtor||!createShape||!setLead||!getShape||!motorStep||!transmission||!setGear||!getSpeed)throw std::runtime_error("Original controller method unavailable");
 NxSDKCreateError error;NxPhysicsSDK*sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk)throw std::runtime_error("SDK failed");NxSceneDesc sd;sd.gravity=NxVec3(0,0,0);sd.timeStepMethod=NX_TIMESTEP_VARIABLE;NxScene*scene=sdk->createScene(sd);if(!scene)throw std::runtime_error("Scene failed");NxPlaneShapeDesc floor;floor.normal=NxVec3(0,0,1);floor.d=0;NxActorDesc floorActor;floorActor.shapes.pushBack(&floor);scene->createActor(floorActor);
 NxActor*actors[2];NxWheelShape*wheels[2];for(int contact=0;contact<2;++contact){NxBodyDesc body;body.mass=1000;body.angularDamping=0;body.linearDamping=0;NxBoxShapeDesc box;box.dimensions=NxVec3(.1f,.1f,.1f);NxWheelShapeDesc wd;wd.radius=.5f;wd.suspensionTravel=.4f;wd.suspension.spring=0;wd.suspension.damper=0;wd.inverseWheelMass=1;wd.wheelFlags=64;NxQuat q;q.setXYZW(.5f,.5f,.5f,.5f);wd.localPose.M.fromQuat(q);NxActorDesc ad;ad.body=&body;ad.globalPose.t=NxVec3(0,contact*10.0f,contact?.6f:5.f);ad.shapes.pushBack(&box);ad.shapes.pushBack(&wd);actors[contact]=scene->createActor(ad);if(!actors[contact])throw std::runtime_error("SDK actor failed");wheels[contact]=actors[contact]->getShapes()[1]->isWheel();if(!wheels[contact])throw std::runtime_error("SDK wheel failed");}
 advance(scene);advance(scene);unsigned old=_controlfp(0,0);_controlfp(_PC_24,_MCW_PC);
 for(int contact=0;contact<2;++contact){NxWheelContactData data;bool actualContact=wheels[contact]->getContact(data)!=0;if(actualContact!=(contact!=0))throw std::runtime_error("SDK contact setup differs");out<<"{\"event\":\"motor.contact\",\"contact\":"<<contact<<"}\n";out.flush();
 {WheelBinding binding(*actors[contact],*wheels[contact]);ULONG gt=nativegamev130::type("r3d::game::GameCar");unsigned gearOffset=nativegamev130::field(gt,L"_curGear");float torque=0,brake=0,rpm=0;typedef void(__thiscall*Set)(void*,int);typedef void(__thiscall*Motor)(void*,float,float&,float&,float&);typedef void(__thiscall*Transmission)(void*,float,float);typedef float(__thiscall*Speed)(void*);
 for(int move=0;move<4;++move){binding.car.set(0);((Set)(nativegamev130::base+setGear))(binding.car.object,1);binding.car.set(move);actors[contact]->setLinearVelocity(NxVec3(10,0,0));wheels[contact]->setAxleSpeed(100);float speed=((Speed)(nativegamev130::base+getSpeed))(binding.car.object);((Motor)(nativegamev130::base+motorStep))(binding.car.object,1.0f/60,torque,brake,rpm);int usedGear=*(int*)(binding.car.object+gearOffset);((Transmission)(nativegamev130::base+transmission))(binding.car.object,1.0f/60,rpm);out<<"{\"event\":\"motor.sample\",\"contact\":"<<contact<<",\"move\":"<<move<<",\"speed_word\":"<<word(speed)<<",\"axle_word\":"<<word(wheels[contact]->getAxleSpeed())<<",\"motor_word\":"<<word(torque)<<",\"brake_word\":"<<word(brake)<<",\"rpm_word\":"<<word(rpm)<<",\"used_gear\":"<<usedGear<<",\"gear\":"<<*(int*)(binding.car.object+gearOffset)<<"}\n";out.flush();}
 }}_controlfp(old,_MCW_PC);sdk->releaseScene(*scene);sdk->release();return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 3;}}
