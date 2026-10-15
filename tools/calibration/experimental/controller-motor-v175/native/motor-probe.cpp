#include <NxPhysics.h>
#include <windows.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstring>
#include "NativeGameStartupV130.h"
static DWORD registerShapes=0,getNxShape=0,addWheel=0,setLead=0,getShape=0,motorStep=0,transmission=0,setGear=0,getSpeed=0;
static BOOL CALLBACK method(PSYMBOL_INFO s,ULONG,void*){DWORD r=(DWORD)s->Address-nativegamev130::base,n=999;SymGetTypeInfo(nativegamev130::process,nativegamev130::base,s->TypeIndex,TI_GET_COUNT,&n);
#define M(name,slot) if(strcmp(s->Name,name)==0)slot=r
 if(strcmp(s->Name,"r3d::game::CarWheels::Add")==0 && n==1)addWheel=r;
 M("r3d::px::Shapes::RegisterClasses",registerShapes);M("r3d::px::WheelShape::GetNxShape",getNxShape);
 M("r3d::game::CarWheel::SetLead",setLead);M("r3d::game::CarWheel::GetShape",getShape);M("r3d::game::GameCar::MotorProgress",motorStep);M("r3d::game::GameCar::TransmissionProgress",transmission);M("r3d::game::GameCar::SetCurGear",setGear);
 if(strcmp(s->Name,"r3d::game::GameCar::GetSpeed")==0&&n==1)getSpeed=r;
#undef M
 return TRUE;}
static float value(unsigned u){float f;memcpy(&f,&u,4);return f;}
static unsigned word(float f){unsigned u;memcpy(&u,&f,4);return u;}
static void advance(NxScene*scene){scene->simulate(1.0f/60);scene->flushStream();scene->fetchResults(NX_RIGID_BODY_FINISHED,true);}
struct WheelBinding {nativegamev130::Control car;void*ownedWheel;NxShape**slot;WheelBinding(NxActor&actor,NxWheelShape&wheel):car(actor),ownedWheel(0),slot(0){NxActor**root=(NxActor**)((unsigned char*)car.actor+car.nxOffset);*root=0;
 ULONG gt=nativegamev130::type("r3d::game::GameCar");void*wheels=*(void**)(car.object+nativegamev130::field(gt,L"_wheels"));typedef void*(__thiscall*Add)(void*);ownedWheel=((Add)(nativegamev130::base+addWheel))(wheels);if(!ownedWheel)throw std::runtime_error("Original Add wheel failed");
 typedef void*(__thiscall*Get)(void*);void*shape=((Get)(nativegamev130::base+getShape))(ownedWheel);if(!shape)throw std::runtime_error("Original owned shape missing");slot=(NxShape**)((unsigned char*)shape+nativegamev130::field(nativegamev130::type("r3d::px::Shape"),L"_nxShape"));if(*slot)throw std::runtime_error("Unexpected owned SDK shape");*slot=&wheel;typedef NxWheelShape*(__thiscall*GetNx)(void*);if(((GetNx)(nativegamev130::base+getNxShape))(shape)!=&wheel)throw std::runtime_error("Original wheel wrapper binding differs");
 typedef void(__thiscall*Lead)(void*,bool);((Lead)(nativegamev130::base+setLead))(ownedWheel,true);*root=&actor;}
 ~WheelBinding(){if(slot)*slot=0;}
};
int main(int argc,char**argv){try{if(argc!=3)return 2;std::ifstream input(argv[1]);std::string magic;unsigned count;input>>magic>>count;if(magic!="MOTORROCK_MOTOR_V175")return 2;struct Case{int contact,automatic,gear,move;unsigned torque,speed,axle,k;};std::vector<Case> cases(count);for(unsigned i=0;i<count;++i){Case&c=cases[i];input>>c.contact>>c.torque>>c.automatic>>c.gear>>c.move>>c.speed>>c.axle>>c.k;if(!input)return 3;}std::ofstream out(argv[2]);nativegamev130::init();SymEnumSymbols(nativegamev130::process,nativegamev130::base,"*",method,0);if(!registerShapes||!getNxShape||!addWheel||!setLead||!getShape||!motorStep||!transmission||!setGear||!getSpeed)throw std::runtime_error("Original controller method unavailable");
 typedef void(__cdecl*Register)();((Register)(nativegamev130::base+registerShapes))();
 NxSDKCreateError error;NxPhysicsSDK*sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&error);if(!sdk)throw std::runtime_error("SDK failed");NxSceneDesc sd;sd.gravity=NxVec3(0,0,0);sd.timeStepMethod=NX_TIMESTEP_VARIABLE;NxScene*scene=0;NxScene*scenes[2]={0,0};
 NxActor*actors[2];NxWheelShape*wheels[2];for(int contact=0;contact<2;++contact){scene=scenes[contact]=sdk->createScene(sd);if(!scene)throw std::runtime_error("Scene failed");NxPlaneShapeDesc floor;floor.normal=NxVec3(0,0,1);floor.d=0;NxActorDesc floorActor;floorActor.shapes.pushBack(&floor);scene->createActor(floorActor);NxBodyDesc body;body.mass=1000;body.angularDamping=0;body.linearDamping=0;NxBoxShapeDesc box;box.dimensions=NxVec3(.1f,.1f,.1f);NxWheelShapeDesc wd;wd.radius=.5f;wd.suspensionTravel=.4f;wd.suspension.spring=0;wd.suspension.damper=0;wd.inverseWheelMass=1;wd.wheelFlags=64|NX_WF_AXLE_SPEED_OVERRIDE;NxQuat q;q.setXYZW(.5f,.5f,.5f,.5f);wd.localPose.M.fromQuat(q);NxActorDesc ad;ad.body=&body;ad.globalPose.t=NxVec3(0,contact*10.0f,contact?.6f:5.f);ad.shapes.pushBack(&box);ad.shapes.pushBack(&wd);actors[contact]=scene->createActor(ad);if(!actors[contact])throw std::runtime_error("SDK actor failed");wheels[contact]=actors[contact]->getShapes()[1]->isWheel();if(!wheels[contact])throw std::runtime_error("SDK wheel failed");}
 for(int sceneIndex=0;sceneIndex<2;++sceneIndex){advance(scenes[sceneIndex]);advance(scenes[sceneIndex]);}unsigned old=_controlfp(0,0);_controlfp(_PC_24,_MCW_PC);
 for(int contact=0;contact<2;++contact){scene=scenes[contact];unsigned dynamic=0;for(unsigned actorIndex=0;actorIndex<scene->getNbActors();++actorIndex)if(scene->getActors()[actorIndex]->isDynamic())++dynamic;if(dynamic!=1)throw std::runtime_error("Scene is not single-actor isolated");NxWheelContactData data;bool actualContact=wheels[contact]->getContact(data)!=0;if(actualContact!=(contact!=0))throw std::runtime_error("SDK contact setup differs");out<<"{\"event\":\"motor.contact\",\"contact\":"<<contact<<",\"dynamic_actors\":"<<dynamic<<"}\n";out.flush();
 {WheelBinding binding(*actors[contact],*wheels[contact]);ULONG gt=nativegamev130::type("r3d::game::GameCar");unsigned gearOffset=nativegamev130::field(gt,L"_curGear");float torque=0,brake=0,rpm=0;typedef void(__thiscall*Set)(void*,int);typedef void(__thiscall*Motor)(void*,float,float&,float&,float&);typedef void(__thiscall*Transmission)(void*,float,float);typedef float(__thiscall*Speed)(void*);
 ULONG mt=nativegamev130::type("r3d::game::CarMotorDesc");unsigned char*motor=binding.car.object+nativegamev130::field(gt,L"_motor");unsigned torqueOffset=nativegamev130::field(mt,L"maxTorque"),autoOffset=nativegamev130::field(mt,L"autoGear"),scaleOffset=nativegamev130::field(gt,L"_motorTorqueK");
 for(unsigned i=0;i<count;++i){const Case&c=cases[i];if(c.contact!=contact)continue;binding.car.set(0);((Set)(nativegamev130::base+setGear))(binding.car.object,c.gear);*(float*)(motor+torqueOffset)=value(c.torque);*(bool*)(motor+autoOffset)=c.automatic!=0;*(float*)(binding.car.object+scaleOffset)=value(c.k);binding.car.set(c.move);actors[contact]->setGlobalPosition(NxVec3(0,contact*10.0f,contact?.6f:5.f));actors[contact]->setLinearVelocity(NxVec3(value(c.speed),0,0));wheels[contact]->setAxleSpeed(value(c.axle));advance(scene);_controlfp(_PC_24,_MCW_PC);float speed=((Speed)(nativegamev130::base+getSpeed))(binding.car.object);float axle=wheels[contact]->getAxleSpeed();NxWheelContactData currentContact;if((wheels[contact]->getContact(currentContact)!=0)!=(contact!=0))throw std::runtime_error("Contact changed during preparation");if(word(axle)!=c.axle)throw std::runtime_error("SDK overridden axle input differs");torque=brake=rpm=0;
 ((Motor)(nativegamev130::base+motorStep))(binding.car.object,1.0f/60,torque,brake,rpm);int usedGear=*(int*)(binding.car.object+gearOffset);((Transmission)(nativegamev130::base+transmission))(binding.car.object,1.0f/60,rpm);
 out<<"{\"event\":\"motor.sample\",\"case\":"<<i<<",\"contact\":"<<contact<<",\"maximum_torque_word\":"<<c.torque<<",\"automatic\":"<<c.automatic<<",\"initial_gear\":"<<c.gear<<",\"move\":"<<c.move<<",\"velocity_word\":"<<word(actors[contact]->getLinearVelocity().x)<<",\"speed_word\":"<<word(speed)<<",\"axle_word\":"<<word(axle)<<",\"scale_word\":"<<c.k<<",\"motor_word\":"<<word(torque)<<",\"brake_word\":"<<word(brake)<<",\"rpm_word\":"<<word(rpm)<<",\"used_gear\":"<<usedGear<<",\"gear\":"<<*(int*)(binding.car.object+gearOffset)<<"}\n";if(i%1000==0)out.flush();}

 }}_controlfp(old,_MCW_PC);for(int sceneIndex=0;sceneIndex<2;++sceneIndex)sdk->releaseScene(*scenes[sceneIndex]);sdk->release();return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 3;}}
