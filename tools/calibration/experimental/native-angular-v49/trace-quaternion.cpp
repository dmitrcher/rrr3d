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
static bool memory(DWORD address,void* output,SIZE_T size){SIZE_T read=0;return ReadProcessMemory(processHandle,(LPCVOID)address,output,size,&read)!=0&&read==size;}
static void patch(Breakpoint& b,bool enabled){SIZE_T written=0;BYTE value=enabled?0xcc:b.original;if(!WriteProcessMemory(processHandle,(LPVOID)b.address,&value,1,&written)||written!=1){fprintf(stderr,"Breakpoint write failed %lu\n",GetLastError());ExitProcess(3);}FlushInstructionCache(processHandle,(LPCVOID)b.address,1);}
static void vectorAt(const char* name,DWORD address){float v[3];if(memory(address,v,sizeof(v))){fprintf(traceFile,",\"%s\":{\"address\":%lu,\"values\":[",name,address);for(int i=0;i<3;++i){if(i)fprintf(traceFile,",");if(_finite(v[i]))fprintf(traceFile,"%.9g",v[i]);else fprintf(traceFile,"null");}fprintf(traceFile,"]}");}}
static void wordsAt(const char* name,DWORD address,unsigned count){
 DWORD words[128];if(count>128||!memory(address,words,count*4))return;
 fprintf(traceFile,",\"%s\":{\"address\":%lu,\"words\":[",name,address);
 for(unsigned i=0;i<count;++i){if(i)fprintf(traceFile,",");fprintf(traceFile,"%lu",words[i]);}fprintf(traceFile,"]}");
}
static void arrayAt(const char* name,DWORD object,DWORD pointerOffset,DWORD countOffset,unsigned stride,unsigned count){
 DWORD data=0,size=0;if(!memory(object+pointerOffset,&data,4)||!memory(object+countOffset,&size,4)||size>64)return;
 fprintf(traceFile,",\"%s_size\":%lu",name,size);
 for(unsigned i=0;i<size&&i<8;++i){char field[64];sprintf_s(field,"%s_%u",name,i);wordsAt(field,data+i*stride,count);}
}
static void configureWatch(bool enable){
 HANDLE snap=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);THREADENTRY32 entry;ZeroMemory(&entry,sizeof(entry));entry.dwSize=sizeof(entry);
 if(Thread32First(snap,&entry))do{if(entry.th32OwnerProcessID!=targetPid)continue;
 HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT,FALSE,entry.th32ThreadID);CONTEXT c;ZeroMemory(&c,sizeof(c));c.ContextFlags=CONTEXT_DEBUG_REGISTERS;
 bool ok=thread&&GetThreadContext(thread,&c)!=0;
 if(ok){c.Dr0=watchAddress[0];c.Dr1=watchAddress[1];c.Dr2=watchAddress[2];c.Dr3=watchAddress[3];c.Dr6=0;c.Dr7=enable?0xffff0055:0;ok=SetThreadContext(thread,&c)!=0;}
 fprintf(traceFile,"{\"event\":\"watch-config\",\"thread\":%lu,\"enabled\":%s,\"success\":%s}\n",entry.th32ThreadID,enable?"true":"false",ok?"true":"false");if(thread)CloseHandle(thread);
 }while(Thread32Next(snap,&entry));CloseHandle(snap);fflush(traceFile);
}
static void backend(DWORD handle){
 DWORD manager=0,table=0;const DWORD kind=handle>>22;
 if(!memory(coreBase+0x3507f8+kind*4,&manager,4)||!memory(manager,&table,4))return;
 fprintf(traceFile,",\"contact_handle\":%lu,\"handle_kind\":%lu",handle,kind);
 wordsAt("backend_manager",manager,32);wordsAt("backend_vtable",table,32);
 DWORD storage=0,entries=0,owner=0,implementation=0,vtable=0;
 if(memory(manager+0x38,&storage,4)){wordsAt("backend_storage",storage,64);
 if(memory(storage+0xc0,&entries,4)&&memory(entries+(handle&0x3ffff)*8,&owner,4)){
 wordsAt("contact_entry",entries+(handle&0x3ffff)*8,2);wordsAt("contact_owner",owner,32);
 if(memory(owner+0x40,&implementation,4)&&memory(implementation,&vtable,4)){wordsAt("contact_implementation",implementation,64);wordsAt("contact_implementation_vtable",vtable,46);
 arrayAt("raw_point",implementation,0x14,0x1c,100,25);
 arrayAt("normal_record",implementation,0x2c,0x34,16,4);
 arrayAt("row_record",implementation,0x38,0x40,136,34);
 if(!watchArmed){DWORD raw=0,modified=0;if(memory(implementation+0x14,&raw,4)&&memory(implementation+0x38,&modified,4)){watchAddress[0]=modified+0x38;watchAddress[1]=raw+0x3c;watchAddress[2]=modified+0x0c;watchAddress[3]=modified;}}}}}
 DWORD lookup=0;if(memory(table+0x50,&lookup,4))fprintf(traceFile,",\"lookup_rva\":%lu",lookup-coreBase);
}
static void record(Breakpoint& b,const CONTEXT& c){
 fprintf(traceFile,"{\"function\":\"%s\",\"hit\":%u,\"offset\":%lu,\"eax\":%lu,\"ecx\":%lu,\"edx\":%lu,\"esi\":%lu,\"esp\":%lu",b.name,b.hits,b.offset,c.Eax,c.Ecx,c.Edx,c.Esi,c.Esp);
 wordsAt("stack",c.Esp,12);
 if(b.offset==0x12accd){DWORD table=0;wordsAt("backend",c.Ecx,128);if(memory(c.Ecx,&table,4))wordsAt("backend_vtable",table,103);fprintf(traceFile,",\"getter_rva\":%lu",c.Eax-coreBase);}
 if(b.offset==0x12accf||b.offset==0x12670f)wordsAt("returned_quaternion",c.Eax,4);
 fprintf(traceFile,"}\n");fflush(traceFile);
}

int main(int argc,char** argv){
 const char* stepFlag=getenv("MOTORROCK_NATIVE_TRACE_STEP");if(stepFlag && *stepFlag){targetStep=atoi(stepFlag);if(targetStep<0 || targetStep>=120)return 2;}
 if(argc<5){fprintf(stderr,"trace-contact output.jsonl SpringProbe.exe native.jsonl list:case\n");return 2;}
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
   const DWORD offsets[]={0x12accd,0x12accf,0x12670f};const char* names[]={"backend-quaternion-call","backend-quaternion-return","mass-quaternion-return"};
   for(int i=0;i<3;++i){Breakpoint b={offsets[i],base+offsets[i],0,0,names[i]};if(!memory(b.address,&b.original,1)){fprintf(stderr,"Read code failed\n");return 3;}fprintf(traceFile,"{\"event\":\"breakpoint\",\"offset\":%lu,\"original_byte\":%u}\n",b.offset,(unsigned)b.original);points.push_back(b);patch(points.back(),true);}fprintf(stderr,"Instrumented %s base=%lu\n",name,base);
  }if(event.u.LoadDll.hFile)CloseHandle(event.u.LoadDll.hFile);}
  else if(event.dwDebugEventCode==EXCEPTION_DEBUG_EVENT){const DWORD code=event.u.Exception.ExceptionRecord.ExceptionCode;const DWORD address=(DWORD)event.u.Exception.ExceptionRecord.ExceptionAddress;HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT,FALSE,event.dwThreadId);CONTEXT context;ZeroMemory(&context,sizeof(context));context.ContextFlags=CONTEXT_ALL;bool own=false;
   if(code==EXCEPTION_BREAKPOINT){for(unsigned i=0;i<points.size();++i)if(points[i].address==address){if(!GetThreadContext(thread,&context))return 3;Breakpoint& b=points[i];++b.hits;if(b.offset==0x76fe2)++currentSimulationStep;record(b,context);if(false&&b.offset==0x751ab&&b.hits==1&&!watchArmed){DWORD array=0;if(!memory(context.Ebx+0x10,&array,4))return 3;watchAddress[0]=array+0x60;watchAddress[1]=array+0x10;watchAddress[2]=array+0x40;watchAddress[3]=array+0x74;watchArmed=true;configureWatch(true);context.Dr0=watchAddress[0];context.Dr1=watchAddress[1];context.Dr2=watchAddress[2];context.Dr3=watchAddress[3];context.Dr6=0;context.Dr7=0xffff0055;}patch(b,false);context.Eip=address;context.EFlags|=0x100;if(!SetThreadContext(thread,&context))return 3;pending[event.dwThreadId]=(int)i;own=true;break;}}
   else if(code==EXCEPTION_SINGLE_STEP&&pending.count(event.dwThreadId)){int index=pending[event.dwThreadId];pending.erase(event.dwThreadId);if((targetStep>=0 && currentSimulationStep<=targetStep) || points[index].hits<(points[index].offset==0x1e7e50?512:points[index].offset==0x8d8fe?512:8192))patch(points[index],true);if(!GetThreadContext(thread,&context))return 3;context.EFlags&=~0x100;SetThreadContext(thread,&context);own=true;}
   if(code==EXCEPTION_SINGLE_STEP&&watchArmed){if(!GetThreadContext(thread,&context))return 3;if(context.Dr6&15){Breakpoint w={context.Eip-coreBase,context.Eip,0,++watchHits,"solver-row-watch"};fprintf(traceFile,"{\"event\":\"watch-status\",\"dr6\":%lu,\"dr7\":%lu}\n",context.Dr6,context.Dr7);record(w,context);context.Dr6=0;if(watchHits>=160){context.Dr7=0;configureWatch(false);}if(!SetThreadContext(thread,&context))return 3;own=true;}}
   if(thread)CloseHandle(thread);if(!own&&code!=EXCEPTION_BREAKPOINT)status=DBG_EXCEPTION_NOT_HANDLED;
  }else if(event.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT){result=event.u.ExitProcess.dwExitCode;done=true;}
  ContinueDebugEvent(event.dwProcessId,event.dwThreadId,status);
 }
 CloseHandle(processHandle);fclose(traceFile);return (int)result;
}
