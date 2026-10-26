from pathlib import Path
import hashlib,json
R=Path.cwd();W=R/'work/controller-jump-runtime-v189';N=W/'trajectory';N.mkdir(exist_ok=True);base=R/'work/sync-rrr3d/tools/calibration/windows.cpp';s=base.read_text();s=s.replace('#include "EulerAngles.h"','#include <windows.h>\n#include "NativeGameStartupV130.h"\n#include "build-id.h"')
pos=s.index('static void execute(')
s=s[:pos]+'''static DWORD actualJump=0;
static BOOL CALLBACK jumpMethod(PSYMBOL_INFO p,ULONG,void*){if(strcmp(p->Name,"r3d::game::GameCar::JumpProgress")==0)actualJump=(DWORD)p->Address-nativegamev130::base;return TRUE;}
template<class T>static void jumpField(void*object,const wchar_t*name,const T&v){ULONG offset=nativegamev130::field(nativegamev130::type("r3d::game::GameCar"),name);memcpy((char*)object+offset,&v,sizeof(v));}
'''+s[pos:]
anchor='static void execute(const cal::Case& c) {';assert s.count(anchor)==1;s=s.replace(anchor,anchor+'''
 const bool actualRoute=std::getenv("MOTORROCK_ACTUAL_JUMP_V189")!=0;
 if(c.name!="gravity" && c.name!="game-air-gravity" && c.name!="game-air-pitch")throw std::runtime_error("Only three qualified airborne controls supported");
 if(actualRoute && c.name=="gravity")throw std::runtime_error("Ordinary gravity has no original JumpProgress extra acceleration");
 _controlfp(_PC_53,_MCW_PC); // Same SDK actor creation context as immutable standalone reference.
''')
a=s.index(' if(c.angularDamping.x!=1');b=s.index(' car->addForce(NxVec3((float)x.acceleration.x',a);removed=s[a:b];s=s[:a]+s[b:]
start=' for(int tick=0;tick<c.steps;++tick){bodyContacts.tick=bodyReport.tick=tick;const cal::Command x=cal::command(c,tick);';assert s.count(start)==1
s=s.replace(start,''' nativegamev130::Control* original=0;
 if(actualRoute){nativegamev130::init();SymEnumSymbols(nativegamev130::process,nativegamev130::base,"*",jumpMethod,0);if(actualJump!=0x498110)throw std::runtime_error("Original JumpProgress identity mismatch");original=new nativegamev130::Control(*car);jumpField(original->object,L"_springTime",0.f);jumpField(original->object,L"_flyYTorque",(float)c.airPitch);}
 std::ostringstream identity;identity<<"{\\"actual_method\\":"<<(actualRoute?"true":"false")<<",\\"source_sha256\\":\\""<<kProbeSourceSha<<"\\",\\"build_id\\":\\""<<kProbeBuildId<<"\\",\\"method_rva\\":"<<actualJump<<"}";motorrock::parity::event("calibration.jump-route-v189",c.name,identity.str());
'''+start)
raw=' if(c.extraGravity)car->addForce(NxVec3(0,0,(float)c.gravity),NX_ACCELERATION);\n if(c.airPitch)car->addLocalTorque(NxVec3(0,(float)c.airPitch,0),NX_ACCELERATION);';assert s.count(raw)==1;s=s.replace(raw,''' if(actualRoute){bool contact=false;for(int wi=0;wi<4;++wi){NxWheelContactData data;contact=contact||wheels[wi]->getContact(data)!=0;}if(contact)throw std::runtime_error("Unexpected contact in airborne control");jumpField(original->object,L"_anyWheelContact",contact);_controlfp(_PC_24,_MCW_PC);typedef void(__thiscall*Jump)(void*,float);((Jump)(nativegamev130::base+actualJump))(original->object,1.f/60);}
 else {
'''+raw+'\n }')
end=' if(peer)scene->releaseActor(*peer);scene->releaseActor(*car);';assert s.count(end)==1;s=s.replace(end,' delete original;'+end)
(N/'jump-trajectory.cpp').write_text(s);(W/'trajectory-preparation.json').write_text(json.dumps(dict(base=str(base),base_sha256=hashlib.sha256(base.read_bytes()).hexdigest(),derived_sha256=hashlib.sha256(s.encode()).hexdigest(),scope='Owned V14 SDK harness restricted to three existing airborne cases; optional original JumpProgress replaces only standalone extra-gravity/pitch API calls. Original GameDLL/SDK unchanged. Actor creation PC53, actual method PC24; not complete game fixed step.',removed_unexercised_stabilization_block_sha256=hashlib.sha256(removed.encode()).hexdigest()),indent=2)+'\n')
builder=(W/'build-native.py').read_text().replace("N=W/'native'","N=W/'trajectory'").replace("jump-probe.cpp","jump-trajectory.cpp").replace('JumpProbeV189.exe','JumpTrajectoryV189.exe').replace('jump-runtime-v189\';D.mkdir','jump-trajectory-v189\';D.mkdir').replace("jump-runtime-v189','--cx-app'","jump-trajectory-v189','--cx-app'").replace('jump-runtime-v189\\\\build.cmd','jump-trajectory-v189\\\\build.cmd').replace("W/'native-build.json'","W/'trajectory-build.json'").replace("W/'native-build.log'","W/'trajectory-build.log'")
# Pin extra, already-owned harness headers and parity logger explicitly.
builder=builder.replace("includes=[original/", "includes=[R/'work/sync-rrr3d/tools/calibration',R/'work/sync-rrr3d/src/LexStd/header']+[original/")
builder=builder.replace(r'C:\MotorRock\jump-runtime-v189',r'C:\MotorRock\jump-trajectory-v189').replace("sha(includes[0]/'NxPhysics.h')","sha(original/'include/px/Physics/include/NxPhysics.h')")
(W/'build-trajectory.py').write_text(builder)
print('Prepared derived three-case harness')
