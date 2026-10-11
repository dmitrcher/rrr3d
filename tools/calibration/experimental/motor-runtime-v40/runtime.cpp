#include <windows.h>
#include <float.h>
#include <cstdio>
#include <cstring>
unsigned bits(float v){unsigned u;std::memcpy(&u,&v,4);return u;}
int main(){
 SetDllDirectoryA("C:\\MotorRock\\game-debug");
 HMODULE game=LoadLibraryA("C:\\MotorRock\\game-debug\\Rock3dGame.dll");
 if(!game){std::fprintf(stderr,"LoadLibrary failed %lu\n",GetLastError());return 3;}
 typedef void*(__thiscall* Constructor)(void*);
 typedef float(__thiscall* Calculate)(void*,float,unsigned);
 unsigned char* base=reinterpret_cast<unsigned char*>(game);
 if(base[0x4926b0]!=0x55||base[0x4926b1]!=0x8b||base[0x4926b2]!=0xec)return 4;
 Constructor constructor=reinterpret_cast<Constructor>(base+0x492380);
 Calculate calculate=reinterpret_cast<Calculate>(base+0x4926b0);
 __declspec(align(16)) unsigned char motor[128]={0};constructor(motor);
 // This buffer is constructed by the original DLL. The three scalar offsets
 // are independently identified in its CalcTorque machine code.
 const float torques[]={300,600,2000,5000};const float sems[]={.7f,.9f,1.f};
 unsigned control=0;_controlfp_s(&control,0,0);
 for(int precision=0;precision<2;++precision){
  _controlfp_s(&control,precision?_PC_24:_PC_53,_MCW_PC);
  for(int t=0;t<4;++t)for(int s=0;s<3;++s)for(unsigned g=0;g<6;++g){
   std::memcpy(motor+8,&torques[t],4);std::memcpy(motor+12,&sems[s],4);
   float result=calculate(motor,1000.f,g);
   std::printf("{\"precision\":%d,\"control\":%u,\"torque_index\":%d,\"sem_index\":%d,\"gear\":%u,\"source_bits\":%u,\"source\":%.9g}\n",precision?24:53,control,t,s,g,bits(result),result);
  }
 }
 return 0;
}
