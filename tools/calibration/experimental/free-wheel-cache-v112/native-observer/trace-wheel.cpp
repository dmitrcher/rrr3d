#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <cstdio>
#include <float.h>
#include <cstring>
#include <string>
#include <vector>
#include <map>
struct Breakpoint { DWORD offset; DWORD address; BYTE original; unsigned hits; const char* name; };
static std::vector<Breakpoint> points;
static HANDLE processHandle;
static DWORD coreBase=0;
static DWORD targetPid=0,watchAddress[4]={0,0,0,0};static unsigned watchHits=0;static bool watchArmed=false;
static FILE* traceFile;
static int targetStep=-1,currentSimulationStep=-1;
static int targetSampleTickV71=-1,completedSamplesV71=0;
static std::string physicalPathV71,physicalPendingV71;
static long physicalOffsetV71=0;
static void syncPhysicalTickV71() {
 FILE* file=0;
 if(fopen_s(&file,physicalPathV71.c_str(),"rb")!=0)return;
 if(fseek(file,physicalOffsetV71,SEEK_SET)!=0){fclose(file);ExitProcess(5);}
 char buffer[8192];size_t count=0;
 while((count=fread(buffer,1,sizeof(buffer),file))!=0)physicalPendingV71.append(buffer,count);
 physicalOffsetV71=ftell(file);fclose(file);
 size_t newline=0;
 while((newline=physicalPendingV71.find('\n'))!=std::string::npos) {
  std::string line=physicalPendingV71.substr(0,newline);
  if(line.find("\"event\":\"calibration.sample\"")!=std::string::npos)++completedSamplesV71;
  physicalPendingV71.erase(0,newline+1);
 }
}

static bool memory(DWORD address,void* output,SIZE_T size){SIZE_T read=0;return ReadProcessMemory(processHandle,(LPCVOID)address,output,size,&read)!=0&&read==size;}
static void patch(Breakpoint& b,bool enabled){SIZE_T written=0;BYTE value=enabled?0xcc:b.original;if(!WriteProcessMemory(processHandle,(LPVOID)b.address,&value,1,&written)||written!=1){fprintf(stderr,"Breakpoint write failed %lu\n",GetLastError());ExitProcess(3);}FlushInstructionCache(processHandle,(LPCVOID)b.address,1);}
static void wordsAt(const char* name,DWORD address,unsigned count){
 DWORD words[128];if(count>128||!memory(address,words,count*4))return;
 fprintf(traceFile,",\"%s\":{\"address\":%lu,\"words\":[",name,address);
 for(unsigned i=0;i<count;++i){if(i)fprintf(traceFile,",");fprintf(traceFile,"%lu",words[i]);}fprintf(traceFile,"]}");
}
static void configureWatch(bool enable){
 HANDLE snap=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);THREADENTRY32 entry;ZeroMemory(&entry,sizeof(entry));entry.dwSize=sizeof(entry);
 if(Thread32First(snap,&entry))do{if(entry.th32OwnerProcessID!=targetPid)continue;
 HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT,FALSE,entry.th32ThreadID);CONTEXT c;ZeroMemory(&c,sizeof(c));c.ContextFlags=CONTEXT_DEBUG_REGISTERS;
 bool ok=thread&&GetThreadContext(thread,&c)!=0;
 if(ok){c.Dr0=watchAddress[0];c.Dr1=watchAddress[1];c.Dr2=watchAddress[2];c.Dr3=watchAddress[3];c.Dr6=0;c.Dr7=enable?0x000d0001:0;ok=SetThreadContext(thread,&c)!=0;}
 fprintf(traceFile,"{\"event\":\"watch-config\",\"thread\":%lu,\"enabled\":%s,\"success\":%s}\n",entry.th32ThreadID,enable?"true":"false",ok?"true":"false");if(thread)CloseHandle(thread);
 }while(Thread32Next(snap,&entry));CloseHandle(snap);fflush(traceFile);
}
static void record(Breakpoint& b,const CONTEXT& c){


 if(targetSampleTickV71>=0){syncPhysicalTickV71();if(completedSamplesV71!=targetSampleTickV71)return;}
 if(b.offset==0x7f1e3)return;
 if(b.offset==0x3df45 && (!watchArmed || c.Esi!=watchAddress[0]-0x1c))return;
 if(targetStep>=0 && currentSimulationStep!=targetStep && b.offset!=0x3df45 && strcmp(b.name,"solver-row-watch")!=0)return;
 fprintf(traceFile,"{\"function\":\"%s\",\"integration_counter\":%d,\"hit\":%u,\"offset\":%lu,\"eax\":%lu,\"ecx\":%lu,\"edx\":%lu,\"esi\":%lu,\"edi\":%lu,\"ebx\":%lu,\"ebp\":%lu,\"esp\":%lu",b.name,currentSimulationStep,b.hits,b.offset,c.Eax,c.Ecx,c.Edx,c.Esi,c.Edi,c.Ebx,c.Ebp,c.Esp);
 fprintf(traceFile,",\"sample_tick_v71\":%d",completedSamplesV71); wordsAt("stack",c.Esp,48);
 if((b.offset>=0x1486d1 && b.offset<=0x1487b8)||(b.offset>=0x14b51c && b.offset<=0x14b583)) {
  const DWORD wheel=(b.offset>=0x14b51c)?c.Ebx:c.Esi;
  wordsAt("wheel_backend",wheel,128);wordsAt("wheel_tail",wheel+0x200,80);
  wordsAt("axle",wheel+0x21c,1);wordsAt("inverse_wheel_mass",wheel+0x2e4,1);wordsAt("motor_torque",wheel+0x2ec,1);wordsAt("brake_torque",wheel+0x2f0,1);wordsAt("wheel_flags",wheel+0x2e8,1);
  fprintf(traceFile,",\"wheel_address\":%lu",wheel);
 }
 fprintf(traceFile,",\"xmm_words\":[");for(int i=0;i<8;++i){DWORD w[4];memcpy(w,c.ExtendedRegisters+160+i*16,sizeof(w));if(i)fprintf(traceFile,",");fprintf(traceFile,"[%lu,%lu,%lu,%lu]",w[0],w[1],w[2],w[3]);}fprintf(traceFile,"]}\n");fflush(traceFile);
}

int main(int argc,char** argv){
 const char* stepFlag=getenv("MOTORROCK_NATIVE_TRACE_STEP");if(stepFlag && *stepFlag){targetStep=atoi(stepFlag);if(targetStep<0 || targetStep>=120)return 2;}
 if(argc<5){fprintf(stderr,"trace-contact output.jsonl SpringProbe.exe native.jsonl list:case\n");return 2;}
 const char* sampleFlagV71=getenv("MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71");if(sampleFlagV71&&*sampleFlagV71){targetSampleTickV71=atoi(sampleFlagV71);if(targetSampleTickV71<0||targetSampleTickV71>=600)return 2;targetStep=-1;physicalPathV71=argv[3];}
 if(fopen_s(&traceFile,argv[1],"w")!=0)return 3;
 std::string command;for(int i=2;i<argc;++i){if(i!=2)command+=' ';command+='"';command+=argv[i];command+='"';}
 std::vector<char> buffer(command.begin(),command.end());buffer.push_back(0);STARTUPINFOA start;ZeroMemory(&start,sizeof(start));start.cb=sizeof(start);PROCESS_INFORMATION pi;
 if(!CreateProcessA(argv[2],&buffer[0],0,0,FALSE,DEBUG_ONLY_THIS_PROCESS,0,0,&start,&pi)){fprintf(stderr,"CreateProcess failed %lu\n",GetLastError());return 3;}
 processHandle=pi.hProcess;targetPid=pi.dwProcessId;CloseHandle(pi.hThread);std::map<DWORD,int> pending;DWORD result=3;bool done=false;
 while(!done){DEBUG_EVENT event;if(!WaitForDebugEvent(&event,30000)){fprintf(stderr,"Debug wait failed %lu\n",GetLastError());TerminateProcess(processHandle,4);break;}DWORD status=DBG_CONTINUE;
  if(event.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT){if(event.u.CreateProcessInfo.hFile)CloseHandle(event.u.CreateProcessInfo.hFile);if(event.u.CreateProcessInfo.hThread)CloseHandle(event.u.CreateProcessInfo.hThread);}
  else if(event.dwDebugEventCode==CREATE_THREAD_DEBUG_EVENT)CloseHandle(event.u.CreateThread.hThread);
  else if(event.dwDebugEventCode==LOAD_DLL_DEBUG_EVENT){char name[MAX_PATH]={0};GetModuleFileNameExA(processHandle,(HMODULE)event.u.LoadDll.lpBaseOfDll,name,MAX_PATH);if(!name[0] && event.u.LoadDll.hFile)GetFinalPathNameByHandleA(event.u.LoadDll.hFile,name,MAX_PATH,FILE_NAME_NORMALIZED);fprintf(traceFile,"{\"event\":\"dll\",\"base\":%lu,\"name_available\":%s}\n",(DWORD)event.u.LoadDll.lpBaseOfDll,name[0]?"true":"false");fflush(traceFile);const char* filename=strrchr(name,'\\');filename=filename?filename+1:name;fprintf(stderr,"DLL %s\n",name);if(_stricmp(filename,"PhysXCore.dll")==0){
   const DWORD base=(DWORD)event.u.LoadDll.lpBaseOfDll;coreBase=base;
   const DWORD offsets[]={0x7f1e3,0x148759,0x148781,0x1487b8,0x14b51c,0x14b544,0x14b583,0x1486d1,0x1486e0};const char* names[]={"integrator-tick","wheel-motor-input","wheel-motor-result","wheel-motor-brake-final","contact-motor-input","contact-motor-result","contact-motor-brake-final","public-axle-ready","free-wheel-entry-v112"};
   for(int i=0;i<9;++i){Breakpoint b={offsets[i],base+offsets[i],0,0,names[i]};if(!memory(b.address,&b.original,1)){fprintf(stderr,"Read code failed\n");return 3;}fprintf(traceFile,"{\"event\":\"breakpoint\",\"offset\":%lu,\"original_byte\":%u}\n",b.offset,(unsigned)b.original);points.push_back(b);patch(points.back(),true);}fprintf(stderr,"Instrumented %s base=%lu\n",name,base);
  }if(event.u.LoadDll.hFile)CloseHandle(event.u.LoadDll.hFile);}
  else if(event.dwDebugEventCode==EXCEPTION_DEBUG_EVENT){const DWORD code=event.u.Exception.ExceptionRecord.ExceptionCode;const DWORD address=(DWORD)event.u.Exception.ExceptionRecord.ExceptionAddress;HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT,FALSE,event.dwThreadId);CONTEXT context;ZeroMemory(&context,sizeof(context));context.ContextFlags=CONTEXT_ALL;bool own=false;
   if(code==EXCEPTION_BREAKPOINT){for(unsigned i=0;i<points.size();++i)if(points[i].address==address){if(!GetThreadContext(thread,&context))return 3;Breakpoint& b=points[i];++b.hits;if(b.offset==0x7f1e3)++currentSimulationStep;bool alreadyWatching=watchArmed;record(b,context);if(!alreadyWatching && watchArmed)configureWatch(true);if(watchArmed){context.Dr0=watchAddress[0];context.Dr1=watchAddress[1];context.Dr2=watchAddress[2];context.Dr3=watchAddress[3];context.Dr7=watchHits>=512?0:0x000d0001;}patch(b,false);context.Eip=address;context.EFlags|=0x100;if(!SetThreadContext(thread,&context))return 3;pending[event.dwThreadId]=(int)i;own=true;break;}}
   else if(code==EXCEPTION_SINGLE_STEP&&pending.count(event.dwThreadId)){int index=pending[event.dwThreadId];pending.erase(event.dwThreadId);if(targetSampleTickV71>=0 ? completedSamplesV71<=targetSampleTickV71 : (targetStep>=0 ? currentSimulationStep<=targetStep : points[index].hits<8192))patch(points[index],true);if(!GetThreadContext(thread,&context))return 3;context.EFlags&=~0x100;SetThreadContext(thread,&context);own=true;}
   if(code==EXCEPTION_SINGLE_STEP&&watchArmed){if(!GetThreadContext(thread,&context))return 3;if(context.Dr6&15){Breakpoint w={context.Eip-coreBase,context.Eip,0,++watchHits,"solver-row-watch"};fprintf(traceFile,"{\"event\":\"watch-status\",\"dr6\":%lu,\"dr7\":%lu}\n",context.Dr6,context.Dr7);record(w,context);context.Dr6=0;if(watchHits>=512){context.Dr7=0;configureWatch(false);}if(!SetThreadContext(thread,&context))return 3;own=true;}}
   if(thread)CloseHandle(thread);if(!own&&code!=EXCEPTION_BREAKPOINT)status=DBG_EXCEPTION_NOT_HANDLED;
  }else if(event.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT){result=event.u.ExitProcess.dwExitCode;done=true;}
  ContinueDebugEvent(event.dwProcessId,event.dwThreadId,status);
 }
 CloseHandle(processHandle);fclose(traceFile);return (int)result;
}
