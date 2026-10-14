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
 syncPhysicalTickV71();if(targetSampleTickV71>=0 && completedSamplesV71!=targetSampleTickV71)return;
 if(b.offset==0x7f1e3)return;
 if(targetStep>=0 && currentSimulationStep!=targetStep)return;
 fprintf(traceFile,"{\"function\":\"%s\",\"integration_counter\":%d,\"hit\":%u,\"offset\":%lu,\"eax\":%lu,\"ecx\":%lu,\"edx\":%lu,\"esi\":%lu,\"edi\":%lu,\"ebx\":%lu,\"ebp\":%lu,\"esp\":%lu",b.name,currentSimulationStep,b.hits,b.offset,c.Eax,c.Ecx,c.Edx,c.Esi,c.Edi,c.Ebx,c.Ebp,c.Esp);
 fprintf(traceFile,",\"sample_tick_v71\":%d",completedSamplesV71); wordsAt("stack",c.Esp,48);
 if(b.offset==0x3730){wordsAt("callback_config",c.Ecx,4);DWORD normal=0,force=0;memory(c.Esp+12,&normal,4);memory(c.Esp+20,&force,4);wordsAt("callback_normal",normal,3);wordsAt("callback_input_force",force,1);}
 if(b.offset==0x3770){wordsAt("callback_load",c.Esp+8,1);wordsAt("callback_config",c.Esi,4);wordsAt("callback_input_force",c.Ecx,1);fprintf(traceFile,",\"x87_control\":%lu",c.FloatSave.ControlWord);}
 if(b.offset==0x37d2||b.offset==0x37e4)wordsAt("callback_output_force",c.Ecx,1);
 if(b.offset==0x18df0){wordsAt("tire_parameters",c.Esi,6);wordsAt("tire_slip",c.Esp+4,1);}
 if(b.offset>=0x14b000 && b.offset<=0x14c200){
 wordsAt("wheel_contact",c.Ebx+0x1d0,28);wordsAt("wheel_parameters",c.Ebx+0x2a0,20);wordsAt("wheel_flags",c.Ebx+0x2e8,1);wordsAt("wheel_skin",c.Ebx+0xb8,1);wordsAt("prepare_frame",c.Esp,72);DWORD contact=0,actor=0;
 if(memory(c.Ebx+0x204,&contact,4)&&memory(contact+0x128,&actor,4)){wordsAt("anchor_actor_transform",actor+0x128,7);wordsAt("anchor_actor_previous_transform",actor+0xf0,7);}
 }
 if(b.offset>=0x148883 && b.offset<=0x1488d0){wordsAt("force_wheel",c.Esi,128);wordsAt("force_wheel_tail",c.Esi+0x200,80);wordsAt("force_frame",c.Esp,12);fprintf(traceFile,",\"x87_control\":%lu,\"x87_status\":%lu,\"x87_register_bytes\":[",c.FloatSave.ControlWord,c.FloatSave.StatusWord);for(int i=0;i<80;++i){if(i)fprintf(traceFile,",");fprintf(traceFile,"%u",(unsigned)c.FloatSave.RegisterArea[i]);}fprintf(traceFile,"]");}
 if(b.offset==0xee50f){wordsAt("contact_backend",c.Ecx,128);wordsAt("radius_word",c.Ecx+0x2a0,1);}
 if(b.offset==0xee512){DWORD dest=0;fprintf(traceFile,",\"returned_shape\":%lu",c.Eax);if(memory(c.Esp+4,&dest,4))wordsAt("contact_output",dest,20);}
 if(b.offset==0x192f0 || b.offset==0x192f6){wordsAt("radius_word",c.Ecx+0x2a0,1);fprintf(traceFile,",\"x87_control\":%lu,\"x87_status\":%lu,\"x87_register_bytes\":[",c.FloatSave.ControlWord,c.FloatSave.StatusWord);for(int i=0;i<80;++i){if(i)fprintf(traceFile,",");fprintf(traceFile,"%u",(unsigned)c.FloatSave.RegisterArea[i]);}fprintf(traceFile,"]");}
 fprintf(traceFile,",\"xmm_words\":[");for(int i=0;i<8;++i){DWORD w[4];memcpy(w,c.ExtendedRegisters+160+i*16,sizeof(w));if(i)fprintf(traceFile,",");fprintf(traceFile,"[%lu,%lu,%lu,%lu]",w[0],w[1],w[2],w[3]);}fprintf(traceFile,"]}\n");fflush(traceFile);
}

int main(int argc,char** argv){
 const char* stepFlag=getenv("MOTORROCK_NATIVE_TRACE_STEP");if(stepFlag && *stepFlag){targetStep=atoi(stepFlag);if(targetStep<0 || targetStep>=120)return 2;}
 if(argc<5){fprintf(stderr,"trace-contact output.jsonl SpringProbe.exe native.jsonl list:case\n");return 2;}
 physicalPathV71=argv[3];
 const char* sampleFlagV71=getenv("MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71");if(sampleFlagV71&&*sampleFlagV71){targetSampleTickV71=atoi(sampleFlagV71);if(targetSampleTickV71<0||targetSampleTickV71>=600)return 2;targetStep=-1;physicalPathV71=argv[3];}
 if(fopen_s(&traceFile,argv[1],"w")!=0)return 3;
 std::string command;for(int i=2;i<argc;++i){if(i!=2)command+=' ';command+='"';command+=argv[i];command+='"';}
 std::vector<char> buffer(command.begin(),command.end());buffer.push_back(0);STARTUPINFOA start;ZeroMemory(&start,sizeof(start));start.cb=sizeof(start);PROCESS_INFORMATION pi;
 if(!CreateProcessA(argv[2],&buffer[0],0,0,FALSE,DEBUG_ONLY_THIS_PROCESS,0,0,&start,&pi)){fprintf(stderr,"CreateProcess failed %lu\n",GetLastError());return 3;}
 processHandle=pi.hProcess;targetPid=pi.dwProcessId;CloseHandle(pi.hThread);std::map<DWORD,int> pending;DWORD result=3;bool done=false;
 while(!done){DEBUG_EVENT event;if(!WaitForDebugEvent(&event,30000)){fprintf(stderr,"Debug wait failed %lu\n",GetLastError());TerminateProcess(processHandle,4);break;}DWORD status=DBG_CONTINUE;
  if(event.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT){
 const DWORD executableBase=(DWORD)event.u.CreateProcessInfo.lpBaseOfImage;
 const DWORD callbackOffsets[]={0x3730,0x3770,0x37d2,0x37e4};const char* callbackNames[]={"callback-entry","callback-load-ready","callback-clamp-exit","callback-zero-exit"};
 for(int i=0;i<4;++i){Breakpoint b={callbackOffsets[i],executableBase+callbackOffsets[i],0,0,callbackNames[i]};if(!memory(b.address,&b.original,1))return 3;points.push_back(b);patch(points.back(),true);}
 if(event.u.CreateProcessInfo.hFile)CloseHandle(event.u.CreateProcessInfo.hFile);if(event.u.CreateProcessInfo.hThread)CloseHandle(event.u.CreateProcessInfo.hThread);}
  else if(event.dwDebugEventCode==CREATE_THREAD_DEBUG_EVENT)CloseHandle(event.u.CreateThread.hThread);
  else if(event.dwDebugEventCode==LOAD_DLL_DEBUG_EVENT){char name[MAX_PATH]={0};GetModuleFileNameExA(processHandle,(HMODULE)event.u.LoadDll.lpBaseOfDll,name,MAX_PATH);if(!name[0] && event.u.LoadDll.hFile)GetFinalPathNameByHandleA(event.u.LoadDll.hFile,name,MAX_PATH,FILE_NAME_NORMALIZED);fprintf(traceFile,"{\"event\":\"dll\",\"base\":%lu,\"name_available\":%s}\n",(DWORD)event.u.LoadDll.lpBaseOfDll,name[0]?"true":"false");fflush(traceFile);const char* filename=strrchr(name,'\\');filename=filename?filename+1:name;fprintf(stderr,"DLL %s\n",name);if(_stricmp(filename,"PhysXCore.dll")==0){
   const DWORD base=(DWORD)event.u.LoadDll.lpBaseOfDll;coreBase=base;
   const DWORD offsets[]={0x7f1e3,0xee50f,0xee512,0x192f0,0x192f6};const char* names[]={"integrator-tick","wheel-contact-getter-call","wheel-contact-getter-return","radius-getter-entry","radius-getter-return"};
   for(int i=0;i<5;++i){Breakpoint b={offsets[i],base+offsets[i],0,0,names[i]};if(!memory(b.address,&b.original,1))return 3;points.push_back(b);patch(points.back(),true);}fprintf(stderr,"Instrumented %s base=%lu\n",name,base);
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
