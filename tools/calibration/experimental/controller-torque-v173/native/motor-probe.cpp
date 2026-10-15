#include <NxPhysics.h>
#include <windows.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include "NativeGameStartupV130.h"
static DWORD torqueRva=0;
static BOOL CALLBACK motorMethod(PSYMBOL_INFO info,ULONG,void*) {
 if(strcmp(info->Name,"r3d::game::CarMotorDesc::CalcTorque")==0)torqueRva=(DWORD)info->Address-nativegamev130::base;
 return TRUE;
}
static unsigned word(float f){unsigned u;memcpy(&u,&f,4);return u;}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::ofstream out(argv[1]);nativegamev130::init();
 SymEnumSymbols(nativegamev130::process,nativegamev130::base,"*",motorMethod,0);if(!torqueRva)throw std::runtime_error("Original CalcTorque unavailable");
 const ULONG gt=nativegamev130::type("r3d::game::GameCar"),mt=nativegamev130::type("r3d::game::CarMotorDesc");
 unsigned char* obj=(unsigned char*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,nativegamev130::length(gt));
 typedef void(__thiscall*Ctor)(void*,int);((Ctor)(nativegamev130::base+nativegamev130::gamector))(obj,1);
 unsigned char* motor=obj+nativegamev130::field(gt,L"_motor");
 const unsigned t=nativegamev130::field(mt,L"maxTorque"),s=nativegamev130::field(mt,L"SEM"),d=nativegamev130::field(mt,L"gearDiff");
 typedef float(__thiscall*Calc)(const void*,float,unsigned);Calc calc=(Calc)(nativegamev130::base+torqueRva);
 out<<"{\"event\":\"metadata\",\"ctor_rva\":"<<nativegamev130::gamector<<",\"torque_rva\":"<<torqueRva<<",\"motor_offset\":"<<(motor-obj)<<",\"motor_size\":"<<nativegamev130::length(mt)<<"}\n";
 unsigned old=_controlfp(0,0);const float torques[]={2000.0f,17.3f,600.0f,3500.0f,1000.0f,1700.0f};
 for(unsigned precision=0;precision<2;++precision){_controlfp(precision?_PC_53:_PC_24,_MCW_PC);
 for(unsigned c=0;c<6;++c){*(float*)(motor+t)=torques[c];*(float*)(motor+s)=.7f;*(float*)(motor+d)=3.42f;
 for(unsigned gear=0;gear<6;++gear){float result=calc(motor,1000.0f,gear);out<<"{\"event\":\"motor.sample\",\"precision\":"<<(precision?53:24)<<",\"cw\":"<<_controlfp(0,0)<<",\"maximum_torque_word\":"<<word(torques[c])<<",\"sem_word\":"<<word(.7f)<<",\"differential_word\":"<<word(3.42f)<<",\"gear\":"<<gear<<",\"result_word\":"<<word(result)<<"}\n";}
 }}_controlfp(old,_MCW_PC);typedef void(__thiscall*Dtor)(void*);((Dtor)(nativegamev130::base+nativegamev130::gamedtor))(obj);HeapFree(GetProcessHeap(),0,obj);return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
