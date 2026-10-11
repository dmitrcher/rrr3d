#include <windows.h>
#include <dbghelp.h>
#include <cstdio>
#include <cstring>
#pragma comment(lib,"dbghelp.lib")
BOOL CALLBACK found(PSYMBOL_INFO symbol,ULONG,PVOID){
 if(std::strstr(symbol->Name,"CalcTorque")||std::strstr(symbol->Name,"CarMotorDesc::CarMotorDesc")||std::strstr(symbol->Name,"MotorProgress")||std::strstr(symbol->Name,"WheelsProgress"))
 std::printf("%s RVA=0x%llx size=%u\n",symbol->Name,symbol->Address-symbol->ModBase,symbol->Size);
 return TRUE;
}
int main(int argc,char**argv){if(argc!=2)return 2;HANDLE p=GetCurrentProcess();SymSetOptions(SYMOPT_UNDNAME|SYMOPT_DEFERRED_LOADS);if(!SymInitialize(p,0,FALSE))return 3;DWORD64 b=SymLoadModuleEx(p,0,argv[1],0,0,0,0,0);if(!b){std::fprintf(stderr,"load %lu\n",GetLastError());return 4;}if(!SymEnumSymbols(p,b,"*",found,0)){std::fprintf(stderr,"enum %lu\n",GetLastError());return 5;}SymCleanup(p);return 0;}
