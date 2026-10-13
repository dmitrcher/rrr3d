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
static void record(Breakpoint& b,const CONTEXT& c){
 if(targetSampleTickV71>=0){syncPhysicalTickV71();if(completedSamplesV71>targetSampleTickV71)return;}
 if(b.offset==0x7f1e3)return;
 if(targetStep>=0 && currentSimulationStep!=targetStep)return;
 fprintf(traceFile,"{\"function\":\"%s\",\"integration_counter\":%d,\"hit\":%u,\"offset\":%lu,\"eax\":%lu,\"ecx\":%lu,\"edx\":%lu,\"esi\":%lu,\"edi\":%lu,\"ebx\":%lu,\"ebp\":%lu,\"esp\":%lu",b.name,currentSimulationStep,b.hits,b.offset,c.Eax,c.Ecx,c.Edx,c.Esi,c.Edi,c.Ebx,c.Ebp,c.Esp);
 fprintf(traceFile,",\"sample_tick_v71\":%d",completedSamplesV71); unsigned short controlV95=0,statusV95=0;unsigned mxcsrV95=0;memcpy(&controlV95,c.ExtendedRegisters,2);memcpy(&statusV95,c.ExtendedRegisters+2,2);memcpy(&mxcsrV95,c.ExtendedRegisters+24,4);fprintf(traceFile,",\"x87_control_word_v95\":%u,\"x87_status_word_v95\":%u,\"mxcsr_v95\":%u",(unsigned)controlV95,(unsigned)statusV95,mxcsrV95);wordsAt("stack",c.Esp,48);
 if(b.offset==0x149610){DWORD wheel=0,mode=0;if(memory(c.Esp+4,&wheel,4)&&memory(c.Esp+8,&mode,4)){fprintf(traceFile,",\"query_mode\":%lu",mode);wordsAt("queried_wheel_backend",wheel,128);wordsAt("queried_wheel_tail",wheel+0x200,80);}}
 if(b.offset==0x149675){wordsAt("query_caller_wheel",c.Ebx+0x200,80);wordsAt("reset_target_wheel",c.Esi+0x200,80);wordsAt("query_caller_frame",c.Ebp,8);}
 if(b.offset==0x1484d0||b.offset==0x1484f7||b.offset==0x149bef||b.offset==0x14ae1f){DWORD wheel=(b.offset==0x1484d0||b.offset==0x1484f7)?c.Esi:c.Ebx;wordsAt("released_wheel_backend",wheel,128);wordsAt("released_wheel_tail",wheel+0x200,80);}
 if(b.offset==0x14aa3d||b.offset==0x14ace7||b.offset==0x14acec){wordsAt("wheel_backend",c.Ebx,128);wordsAt("wheel_tail",c.Ebx+0x200,80);DWORD owner=0;if(memory(c.Ebx+0x12c,&owner,4))wordsAt("wheel_owner",owner,128);}
 if(b.offset==0x14ace7)wordsAt("spring_descriptor",c.Edx,25);
 if(b.offset==0x14acec){fprintf(traceFile,",\"spring_handle\":%lu",c.Eax);DWORD manager=0,storage=0,blocks=0,block=0,capacity=0,shift=0;const DWORD kind=c.Eax>>22,index=c.Eax&0x3ffff;
 if(memory(coreBase+0x3507f8+kind*4,&manager,4)&&memory(manager+0x38,&storage,4)&&memory(storage+0x1c,&blocks,4)&&memory(storage+0x04,&capacity,4)&&memory(storage+0x10,&shift,4)&&capacity&&shift<20&&memory(blocks+(index>>shift)*4,&block,4)){DWORD object=block+(index&(capacity-1))*0xd4;wordsAt("spring_object",object,53);}}
 if(b.offset==0x67a60){wordsAt("spring_object",c.Ecx,34);DWORD body=0;if(memory(c.Ecx+4,&body,4))wordsAt("body0",body,64);if(memory(c.Ecx+8,&body,4))wordsAt("body1",body,64);}
 if(b.offset==0x681c1||b.offset==0x68397||b.offset==0x6858e||b.offset==0x68d1e){wordsAt("spring_object",c.Esi,34);wordsAt("constraint_row",c.Ecx,32);wordsAt("prepare_frame",c.Esp,36);}
 if(b.offset==0x1e5164||b.offset==0x1e51f3){wordsAt("solve_row",c.Ecx-0x34,32);DWORD body=0;if(memory(c.Ecx-0x30,&body,4))wordsAt("solve_body0",body,64);if(memory(c.Ecx-0x2c,&body,4))wordsAt("solve_body1",body,64);}
 if(b.offset==0x1e57ac)wordsAt("final_row",c.Esi-0x1c,32);
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
   const DWORD offsets[]={0x7f1e3,0x67a60,0x681c1,0x68397,0x6858e,0x68d1e,0x1e5164,0x1e51f3,0x1e57ac,0x14aa3d,0x14ace7,0x14acec,0x1484d0,0x1484f7,0x149bef,0x14ae1f,0x149610,0x149675};const char* names[]={"integrator-tick","spring-row-entry","spring-row-jacobians","spring-row-mass-start","spring-row-soft-start","spring-row-ready","spring-solve-input","spring-solve-result","spring-final-ready","wheel-spring-prepare","wheel-spring-create","wheel-spring-created","wheel-release-entry","wheel-release-clearspring","wheel-unhit-clearspring","wheel-inactive-clearspring","wheel-query-entry","wheel-query-group-reset"};
   for(int i=0;i<18;++i){Breakpoint b={offsets[i],base+offsets[i],0,0,names[i]};if(!memory(b.address,&b.original,1)){fprintf(stderr,"Read code failed\n");return 3;}fprintf(traceFile,"{\"event\":\"breakpoint\",\"offset\":%lu,\"original_byte\":%u}\n",b.offset,(unsigned)b.original);points.push_back(b);patch(points.back(),true);}fprintf(stderr,"Instrumented %s base=%lu\n",name,base);
  }if(event.u.LoadDll.hFile)CloseHandle(event.u.LoadDll.hFile);}
  else if(event.dwDebugEventCode==EXCEPTION_DEBUG_EVENT){const DWORD code=event.u.Exception.ExceptionRecord.ExceptionCode;const DWORD address=(DWORD)event.u.Exception.ExceptionRecord.ExceptionAddress;HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT,FALSE,event.dwThreadId);CONTEXT context;ZeroMemory(&context,sizeof(context));context.ContextFlags=CONTEXT_ALL;bool own=false;
   if(code==EXCEPTION_BREAKPOINT){for(unsigned i=0;i<points.size();++i)if(points[i].address==address){if(!GetThreadContext(thread,&context))return 3;Breakpoint& b=points[i];++b.hits;if(b.offset==0x7f1e3)++currentSimulationStep;record(b,context);patch(b,false);context.Eip=address;context.EFlags|=0x100;if(!SetThreadContext(thread,&context))return 3;pending[event.dwThreadId]=(int)i;own=true;break;}}
   else if(code==EXCEPTION_SINGLE_STEP&&pending.count(event.dwThreadId)){int index=pending[event.dwThreadId];pending.erase(event.dwThreadId);if(targetSampleTickV71>=0 ? completedSamplesV71<=targetSampleTickV71 : (targetStep>=0 ? currentSimulationStep<=targetStep : points[index].hits<8192))patch(points[index],true);if(!GetThreadContext(thread,&context))return 3;context.EFlags&=~0x100;SetThreadContext(thread,&context);own=true;}
   if(thread)CloseHandle(thread);if(!own&&code!=EXCEPTION_BREAKPOINT)status=DBG_EXCEPTION_NOT_HANDLED;
  }else if(event.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT){result=event.u.ExitProcess.dwExitCode;done=true;}
  ContinueDebugEvent(event.dwProcessId,event.dwThreadId,status);
 }
 CloseHandle(processHandle);fclose(traceFile);return (int)result;
}
