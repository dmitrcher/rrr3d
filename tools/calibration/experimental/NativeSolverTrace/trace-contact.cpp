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
 fprintf(traceFile,"{\"function\":\"%s\",\"hit\":%u,\"offset\":%lu,\"eip\":%lu,\"eax\":%lu,\"ebx\":%lu,\"ecx\":%lu,\"edx\":%lu,\"esi\":%lu,\"edi\":%lu,\"esp\":%lu",b.name,b.hits,b.offset,c.Eip,c.Eax,c.Ebx,c.Ecx,c.Edx,c.Esi,c.Edi,c.Esp);
 fprintf(traceFile,",\"ebp\":%lu",c.Ebp);wordsAt("frame",c.Ebp,8);
 vectorAt("eax_memory",c.Eax);vectorAt("ecx_memory",c.Ecx);vectorAt("edx_memory",c.Edx);vectorAt("esi_memory",c.Esi);vectorAt("edi_memory",c.Edi);vectorAt("ebx_memory",c.Ebx);
 DWORD stack[12];if(memory(c.Esp,stack,sizeof(stack))){fprintf(traceFile,",\"stack\":[");for(int i=0;i<12;++i){if(i)fprintf(traceFile,",");fprintf(traceFile,"%lu",stack[i]);}fprintf(traceFile,"]");for(int i=1;i<=4;++i){char n[32];sprintf_s(n,"stack%d_memory",i);vectorAt(n,stack[i]);}}
 if(b.offset==0x207440){DWORD arg=0,handle=0;if(memory(c.Esp+4,&arg,4)&&memory(arg,&handle,4)){backend(handle);wordsAt("dispatcher_arguments",c.Esp,5);}}
 if(b.offset==0x841e0||b.offset==0x84c74||b.offset==0x1e7e50){DWORD row=c.Ecx;if(b.offset==0x1e7e50&&!memory(c.Esp+4,&row,4))row=0;wordsAt("native_solver_row",row,32);wordsAt("native_bias_scale",coreBase+0x331624,1);DWORD body=0;for(unsigned i=0;i<2;++i)if(memory(row+4+i*4,&body,4)){char field[32];sprintf_s(field,"native_body%u",i);wordsAt(field,body,32);}}
 if(b.offset==0x751ab){wordsAt("solver_context",c.Ebx,8);DWORD array=0,size=0;if(memory(c.Ebx+0x10,&array,4)&&memory(c.Ebx+4,&size,4)&&size<=8192){fprintf(traceFile,",\"solver_bytes\":%lu",size);for(unsigned i=0;i<size/128&&i<8;++i){char field[32];sprintf_s(field,"final_row_%u",i);wordsAt(field,array+i*128,32);}}}
 if(b.offset==0x75158||b.offset==0x7518f){wordsAt("prepared_row",c.Esi,32);wordsAt("input_constraint",c.Edi,34);DWORD body=0;for(unsigned i=0;i<2;++i)if(memory(c.Esi+4+i*4,&body,4)){char field[32];sprintf_s(field,"solver_body%u",i);wordsAt(field,body,32);}}
 if(b.offset==0x20478d){DWORD arg=0,handle=0;if(memory(c.Esp,&arg,4)&&memory(arg,&handle,4))backend(handle);}
 if(b.offset==0x8d8fe){wordsAt("resolved_contact",c.Eax,128);DWORD vt=0;if(memory(c.Eax,&vt,4))wordsAt("resolved_vtable",vt,40);}
 if(b.offset==0x204330){DWORD object=0;if(memory(c.Esp+4,&object,4))wordsAt("parent_contact_object",object,32);}
 fprintf(traceFile,"}\n");fflush(traceFile);
}
int main(int argc,char** argv){
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
   const DWORD offsets[]={0x204330,0x207440,0x207775,0x1df250,0x8d8fe,0x20478d,0x75158,0x7518f,0x751ab,0x841e0,0x84c74,0x1e7e50};const char* names[]={"observed-parent-entry","observed-dispatcher-entry","observed-user-callback-return","core-create-entry","observed-contact-lookup-return","observed-dispatcher-return","prepared-row-before-response","prepared-row-after-response","prepared-rows-final-buffer","solver-initialize-entry","solver-initialize-exit","solver-iteration-entry"};
   for(int i=0;i<12;++i){Breakpoint b={offsets[i],base+offsets[i],0,0,names[i]};if(!memory(b.address,&b.original,1)){fprintf(stderr,"Read code failed\n");return 3;}fprintf(traceFile,"{\"event\":\"breakpoint\",\"offset\":%lu,\"original_byte\":%u}\n",b.offset,(unsigned)b.original);points.push_back(b);patch(points.back(),true);}fprintf(stderr,"Instrumented %s base=%lu\n",name,base);
  }if(event.u.LoadDll.hFile)CloseHandle(event.u.LoadDll.hFile);}
  else if(event.dwDebugEventCode==EXCEPTION_DEBUG_EVENT){const DWORD code=event.u.Exception.ExceptionRecord.ExceptionCode;const DWORD address=(DWORD)event.u.Exception.ExceptionRecord.ExceptionAddress;HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT,FALSE,event.dwThreadId);CONTEXT context;ZeroMemory(&context,sizeof(context));context.ContextFlags=CONTEXT_ALL;bool own=false;
   if(code==EXCEPTION_BREAKPOINT){for(unsigned i=0;i<points.size();++i)if(points[i].address==address){if(!GetThreadContext(thread,&context))return 3;Breakpoint& b=points[i];++b.hits;record(b,context);if(b.offset==0x751ab&&b.hits==1&&!watchArmed){DWORD array=0;if(!memory(context.Ebx+0x10,&array,4))return 3;watchAddress[0]=array+0x60;watchAddress[1]=array+0x10;watchAddress[2]=array+0x40;watchAddress[3]=array+0x74;watchArmed=true;configureWatch(true);context.Dr0=watchAddress[0];context.Dr1=watchAddress[1];context.Dr2=watchAddress[2];context.Dr3=watchAddress[3];context.Dr6=0;context.Dr7=0xffff0055;}patch(b,false);context.Eip=address;context.EFlags|=0x100;if(!SetThreadContext(thread,&context))return 3;pending[event.dwThreadId]=(int)i;own=true;break;}}
   else if(code==EXCEPTION_SINGLE_STEP&&pending.count(event.dwThreadId)){int index=pending[event.dwThreadId];pending.erase(event.dwThreadId);if(points[index].hits<(points[index].offset==0x8d8fe?160:(points[index].offset==0x1e7e50)?64:(points[index].offset==0x75158||points[index].offset==0x7518f||points[index].offset==0x841e0||points[index].offset==0x84c74)?32:16))patch(points[index],true);if(!GetThreadContext(thread,&context))return 3;context.EFlags&=~0x100;SetThreadContext(thread,&context);own=true;}
   if(code==EXCEPTION_SINGLE_STEP&&watchArmed){if(!GetThreadContext(thread,&context))return 3;if(context.Dr6&15){Breakpoint w={context.Eip-coreBase,context.Eip,0,++watchHits,"solver-row-watch"};fprintf(traceFile,"{\"event\":\"watch-status\",\"dr6\":%lu,\"dr7\":%lu}\n",context.Dr6,context.Dr7);record(w,context);context.Dr6=0;if(watchHits>=160){context.Dr7=0;configureWatch(false);}if(!SetThreadContext(thread,&context))return 3;own=true;}}
   if(thread)CloseHandle(thread);if(!own&&code!=EXCEPTION_BREAKPOINT)status=DBG_EXCEPTION_NOT_HANDLED;
  }else if(event.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT){result=event.u.ExitProcess.dwExitCode;done=true;}
  ContinueDebugEvent(event.dwProcessId,event.dwThreadId,status);
 }
 CloseHandle(processHandle);fclose(traceFile);return (int)result;
}
