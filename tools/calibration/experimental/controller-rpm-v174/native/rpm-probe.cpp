#include <NxPhysics.h>
#include <windows.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstring>
#include "NativeGameStartupV130.h"
static DWORD rpmRva=0;static std::ostream* symbolsOut=0;
static BOOL CALLBACK method(PSYMBOL_INFO info,ULONG,void*){
 if(strcmp(info->Name,"r3d::game::CarMotorDesc::CalcRPM")==0)rpmRva=(DWORD)info->Address-nativegamev130::base;
 if(strstr(info->Name,"r3d::game::CarWheels::Add")){DWORD n=999;SymGetTypeInfo(nativegamev130::process,nativegamev130::base,info->TypeIndex,TI_GET_COUNT,&n);*symbolsOut<<"{\"event\":\"wheel.symbol\",\"name\":\""<<info->Name<<"\",\"rva\":"<<((DWORD)info->Address-nativegamev130::base)<<",\"type\":"<<info->TypeIndex<<",\"argument_count\":"<<n<<"}\n";}
 return TRUE;
}
static float value(unsigned u){float f;memcpy(&f,&u,4);return f;}static unsigned word(float f){unsigned u;memcpy(&u,&f,4);return u;}
int main(int argc,char**argv){try{if(argc!=3)return 2;std::ifstream input(argv[1]);std::string magic;unsigned count=0;input>>magic>>count;if(magic!="MOTORROCK_RPM_V174")return 2;std::ofstream out(argv[2]);nativegamev130::init();symbolsOut=&out;SymEnumSymbols(nativegamev130::process,nativegamev130::base,"*",method,0);if(!rpmRva)throw std::runtime_error("CalcRPM unavailable");ULONG gt=nativegamev130::type("r3d::game::GameCar"),mt=nativegamev130::type("r3d::game::CarMotorDesc");void*obj=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,nativegamev130::length(gt));typedef void(__thiscall*Ctor)(void*,int);((Ctor)(nativegamev130::base+nativegamev130::gamector))(obj,1);unsigned char*m=(unsigned char*)obj+nativegamev130::field(gt,L"_motor");unsigned maxOffset=nativegamev130::field(mt,L"maxRPM"),idleOffset=nativegamev130::field(mt,L"idlingRPM"),diffOffset=nativegamev130::field(mt,L"gearDiff");typedef float(__thiscall*Calc)(const void*,float,unsigned);Calc calc=(Calc)(nativegamev130::base+rpmRva);unsigned previous=_controlfp(0,0);_controlfp(_PC_24,_MCW_PC);
 for(unsigned i=0;i<count;++i){int maxRpm,idle,gear;unsigned diff,axle;input>>maxRpm>>idle>>diff>>gear>>axle;if(!input)throw std::runtime_error("case read failed");*(int*)(m+maxOffset)=maxRpm;*(int*)(m+idleOffset)=idle;*(float*)(m+diffOffset)=value(diff);float rpm=calc(m,value(axle),(unsigned)gear);out<<"{\"event\":\"rpm.sample\",\"case\":"<<i<<",\"result_word\":"<<word(rpm)<<"}\n";}
 _controlfp(previous,_MCW_PC);typedef void(__thiscall*Dtor)(void*);((Dtor)(nativegamev130::base+nativegamev130::gamedtor))(obj);HeapFree(GetProcessHeap(),0,obj);return out.good()?0:4;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 3;}}
