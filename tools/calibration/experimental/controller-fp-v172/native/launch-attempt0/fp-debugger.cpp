#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <dbghelp.h>
#include <psapi.h>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"psapi.lib")
struct Breakpoint { BYTE original; std::string name; };
static HANDLE target=0;static std::map<DWORD,Breakpoint> breaks;static std::ofstream out;static unsigned hits=0;static std::map<std::string,bool> seen;static DWORD torquePointers[3]={0,0,0};
static void read(DWORD address,void*data,unsigned bytes){SIZE_T n=0;if(!ReadProcessMemory(target,(void*)address,data,bytes,&n)||n!=bytes)throw std::runtime_error("target read failed");}
static void write(DWORD address,BYTE value){SIZE_T n=0;if(!WriteProcessMemory(target,(void*)address,&value,1,&n)||n!=1)throw std::runtime_error("breakpoint write failed");FlushInstructionCache(target,(void*)address,1);}
static void arm(DWORD address,const std::string&name){if(breaks.count(address))return;Breakpoint b;b.name=name;read(address,&b.original,1);write(address,0xcc);breaks[address]=b;out<<"{\"event\":\"armed\",\"name\":\""<<name<<"\",\"address\":"<<address<<"}\n";out.flush();}
static BOOL CALLBACK symbol(PSYMBOL_INFO s,ULONG,void*){const std::string n=s->Name;
 if(n=="r3d::game::GameCar::GameCar" || n=="r3d::game::GameCar::OnFixedStep" || n=="r3d::game::GameCar::MotorProgress" || n=="r3d::game::CarMotorDesc::CalcTorque")arm((DWORD)s->Address,n);return TRUE;}
int main(int argc,char**argv){if(argc!=2)return 2;out.open(argv[1]);PROCESS_INFORMATION pi={0};bool live=false,attached=false;try{
 STARTUPINFOA si={sizeof(si)};si.lpDesktop="winsta0\\default";char cmd[]="\"C:\\MotorRock\\game-debug\\MR.exe\"";
 if(!CreateProcessA("C:\\MotorRock\\game-debug\\MR.exe",cmd,0,0,FALSE,DEBUG_ONLY_THIS_PROCESS,0,"C:\\MotorRock\\game-debug",&si,&pi))throw std::runtime_error("CreateProcess debug failed");
 target=pi.hProcess;live=attached=true;DebugSetProcessKillOnExit(FALSE);SymSetOptions(SYMOPT_DEFERRED_LOADS|SYMOPT_LOAD_LINES);if(!SymInitialize(target,"C:\\MotorRock\\game-debug",FALSE))throw std::runtime_error("SymInitialize failed");
 DWORD session=0;ProcessIdToSessionId(pi.dwProcessId,&session);out<<"{\"event\":\"started\",\"pid\":"<<pi.dwProcessId<<",\"session\":"<<session<<"}\n";out.flush();DWORD start=GetTickCount();
 while(GetTickCount()-start<240000 && live){DEBUG_EVENT e={0};if(!WaitForDebugEvent(&e,250))continue;DWORD action=DBG_CONTINUE;
 if(e.dwDebugEventCode==LOAD_DLL_DEBUG_EVENT){char path[MAX_PATH]={0};GetMappedFileNameA(target,e.u.LoadDll.lpBaseOfDll,path,MAX_PATH);if(strstr(path,"Rock3dGame.dll")){
 DWORD64 base=SymLoadModuleEx(target,e.u.LoadDll.hFile,"C:\\MotorRock\\game-debug\\Rock3dGame.dll","Rock3dGame",(DWORD)e.u.LoadDll.lpBaseOfDll,0,0,0);if(!base)throw std::runtime_error("Game symbols load failed");SymEnumSymbols(target,base,"*",symbol,0);
 }if(e.u.LoadDll.hFile)CloseHandle(e.u.LoadDll.hFile);}
 if(e.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT && e.u.CreateProcessInfo.hFile)CloseHandle(e.u.CreateProcessInfo.hFile);
 if(e.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT){live=false;out<<"{\"event\":\"exit\",\"code\":"<<e.u.ExitProcess.dwExitCode<<"}\n";}
 if(e.dwDebugEventCode==EXCEPTION_DEBUG_EVENT){DWORD address=(DWORD)e.u.Exception.ExceptionRecord.ExceptionAddress;DWORD code=e.u.Exception.ExceptionRecord.ExceptionCode;
 if(code==EXCEPTION_BREAKPOINT && breaks.count(address)){
 Breakpoint b=breaks[address];write(address,b.original);breaks.erase(address);HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT|THREAD_QUERY_INFORMATION,FALSE,e.dwThreadId);CONTEXT c={0};c.ContextFlags=CONTEXT_FULL|CONTEXT_FLOATING_POINT|CONTEXT_EXTENDED_REGISTERS;if(!thread||!GetThreadContext(thread,&c))throw std::runtime_error("GetThreadContext failed");DWORD mxcsr=*(DWORD*)(c.ExtendedRegisters+24);DWORD returnAddress=0;
 out<<"{\"event\":\"fp.observed\",\"name\":\""<<b.name<<"\",\"pid\":"<<e.dwProcessId<<",\"thread\":"<<e.dwThreadId<<",\"address\":"<<address<<",\"x87_control\":"<<(c.FloatSave.ControlWord&65535)<<",\"fxsave_control\":"<<(*(WORD*)c.ExtendedRegisters)<<",\"mxcsr\":"<<mxcsr;
 if(b.name=="r3d::game::GameCar::MotorProgress"){DWORD stack[5];read(c.Esp,stack,sizeof(stack));for(unsigned i=0;i<3;++i)torquePointers[i]=stack[i+2];out<<",\"dt_word\":"<<stack[1];returnAddress=stack[0];}
 if(b.name=="MotorProgress.return"){DWORD vals[3];for(unsigned i=0;i<3;++i)read(torquePointers[i],&vals[i],4);out<<",\"motor_word\":"<<vals[0]<<",\"brake_word\":"<<vals[1]<<",\"rpm_word\":"<<vals[2];}
 out<<"}\n";out.flush();++hits;seen[b.name]=true;if(returnAddress)arm(returnAddress,"MotorProgress.return");c.Eip=address;if(!SetThreadContext(thread,&c))throw std::runtime_error("SetThreadContext failed");CloseHandle(thread);
 }else if(code!=EXCEPTION_BREAKPOINT)action=DBG_EXCEPTION_NOT_HANDLED;
 }
 if(!ContinueDebugEvent(e.dwProcessId,e.dwThreadId,action))throw std::runtime_error("ContinueDebugEvent failed");
 if(seen["MotorProgress.return"] && seen["r3d::game::CarMotorDesc::CalcTorque"] && seen["r3d::game::GameCar::OnFixedStep"])break;
 }
 if(live){for(std::map<DWORD,Breakpoint>::const_iterator i=breaks.begin();i!=breaks.end();++i)write(i->first,i->second.original);breaks.clear();if(!DebugActiveProcessStop(pi.dwProcessId))throw std::runtime_error("detach failed");attached=false;}
 out<<"{\"event\":\"finished\",\"hits\":"<<hits<<",\"restored_breakpoints\":true,\"detached\":true}\n";out.flush();SymCleanup(target);CloseHandle(pi.hThread);CloseHandle(pi.hProcess);return seen["MotorProgress.return"] && seen["r3d::game::CarMotorDesc::CalcTorque"]?0:4;
 }catch(const std::exception&e){out<<"{\"event\":\"error\",\"message\":\""<<e.what()<<"\",\"win32_error\":"<<GetLastError()<<"}\n";out.flush();if(live){for(std::map<DWORD,Breakpoint>::const_iterator i=breaks.begin();i!=breaks.end();++i){SIZE_T n;WriteProcessMemory(target,(void*)i->first,&i->second.original,1,&n);}if(attached)DebugActiveProcessStop(pi.dwProcessId);}std::cerr<<e.what()<<'\n';return 3;}}
