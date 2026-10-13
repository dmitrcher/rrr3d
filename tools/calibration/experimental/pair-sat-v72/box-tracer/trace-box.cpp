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
 if(targetSampleTickV71>=0){syncPhysicalTickV71();if(completedSamplesV71!=targetSampleTickV71)return;}
 if(b.offset==0x7f1e3)return;
 if(targetStep>=0 && b.offset!=0x76fe2 && b.offset!=0x76fe5 && (currentSimulationStep<targetStep-1 || currentSimulationStep>targetStep))return;
 fprintf(traceFile,"{\"function\":\"%s\",\"hit\":%u,\"offset\":%lu,\"eip\":%lu,\"eax\":%lu,\"ebx\":%lu,\"ecx\":%lu,\"edx\":%lu,\"esi\":%lu,\"edi\":%lu,\"esp\":%lu",b.name,b.hits,b.offset,c.Eip,c.Eax,c.Ebx,c.Ecx,c.Edx,c.Esi,c.Edi,c.Esp);
 fprintf(traceFile,",\"simulation_step\":%d,\"ebp\":%lu",currentSimulationStep,c.Ebp);fprintf(traceFile,",\"sample_tick_v71\":%d",completedSamplesV71);wordsAt("frame",c.Ebp,8);
 vectorAt("eax_memory",c.Eax);vectorAt("ecx_memory",c.Ecx);vectorAt("edx_memory",c.Edx);vectorAt("esi_memory",c.Esi);vectorAt("edi_memory",c.Edi);vectorAt("ebx_memory",c.Ebx);
 DWORD stack[12];if(memory(c.Esp,stack,sizeof(stack))){fprintf(traceFile,",\"stack\":[");for(int i=0;i<12;++i){if(i)fprintf(traceFile,",");fprintf(traceFile,"%lu",stack[i]);}fprintf(traceFile,"]");for(int i=1;i<=4;++i){char n[32];sprintf_s(n,"stack%d_memory",i);vectorAt(n,stack[i]);}}

 fprintf(traceFile,",\"xmm_words\":[");for(int xi=0;xi<8;++xi){DWORD w[4];memcpy(w,c.ExtendedRegisters+160+xi*16,sizeof(w));if(xi)fprintf(traceFile,",");fprintf(traceFile,"[%lu,%lu,%lu,%lu]",w[0],w[1],w[2],w[3]);}fprintf(traceFile,"]");
 DWORD caller=0;if(memory(c.Esp,&caller,4))fprintf(traceFile,",\"stack_return_rva\":%lu",caller-coreBase);
 if(b.offset==0x75490||b.offset==0x75560||b.offset==0x754aa){
  DWORD wrapper=b.offset==0x754aa?c.Esi:c.Ecx,impl=0,raw=0,index=0,array=0,input=0;
  wordsAt("constraint_wrapper",wrapper,8);
  if(memory(wrapper+4,&impl,4)){wordsAt("constraint_implementation",impl,64);arrayAt("pre_raw_point",impl,0x14,0x1c,100,25);arrayAt("pre_converted",impl,0x38,0x40,136,34);}
  if(memory(wrapper+0x10,&raw,4))wordsAt("wrapper_raw_point",raw,25);
  if(memory(wrapper+0xc,&index,4)&&((LONG)index)>=0&&index<64&&memory(impl+0x38,&array,4))wordsAt("wrapper_converted",array+index*136,34);
  if(b.offset!=0x754aa&&memory(c.Esp+4,&input,4))wordsAt("setter_input",input,b.offset==0x75560?7:3);
 }
 if(b.offset==0x7c5a0||b.offset==0x7c8a2){wordsAt("native_inertia_source",c.Ecx,40);wordsAt("native_inertia_output",c.Eax,32);}
 if(b.offset==0x22ca45){wordsAt("geometry_box_matrix",c.Ebp-0x144,12);DWORD pose=0;if(memory(c.Ebp+0x10,&pose,4))wordsAt("geometry_box_pose",pose,7);}
 if(b.offset==0x76fe2||b.offset==0x76fe5){DWORD pose=0;if(memory(c.Esp+8,&pose,4))wordsAt("geometry_pose0",pose,7);if(memory(c.Esp+12,&pose,4))wordsAt("geometry_pose1",pose,7);wordsAt("geometry_dispatch",c.Ebp,8);wordsAt("geometry_implementation",c.Esi,32);arrayAt("geometry_candidate",c.Esi+0x20,0,8,36,9);wordsAt("geometry_dispatch_stack",c.Esp,32);DWORD function=0;if(memory(c.Ebp+8,&function,4))fprintf(traceFile,",\"geometry_function_rva\":%lu",function-coreBase);}
 if(b.offset==0x2270b5||b.offset==0x227120||b.offset==0x227165||b.offset==0x227237||b.offset==0x22733c){wordsAt("edge_frame",c.Ebp-0x180,128);wordsAt("edge_delta",c.Ebp-0x80,3);wordsAt("edge_plane_normal",c.Ebp-0x30,3);wordsAt("edge_plane_constant",c.Ebp-0x150,1);wordsAt("edge_normalized_direction",c.Ebp-0x4c,3);wordsAt("edge_clipped_point",c.Ebp-0x3c,3);wordsAt("edge_inverse_coefficient",c.Ebp-0x108,1);wordsAt("edge_crossing_denominator",c.Ebp-0xf0,1);DWORD pointer=0;wordsAt("edge_colliding_p3",c.Ebx,3);if(memory(c.Ebp-0x10,&pointer,4))wordsAt("edge_reference_p1",pointer-4,3);if(memory(c.Ebp-0xec,&pointer,4))wordsAt("edge_reference_p2",pointer,3);if(memory(c.Ebp+0x18,&pointer,4))wordsAt("edge_reference_dir",pointer,3);if(memory(c.Ebp+0x14,&pointer,4))wordsAt("edge_world_matrix",pointer,12);}
 if(b.offset==0x2537a0){DWORD pose=0;if(memory(c.Esp+4,&pose,4))wordsAt("transport_pose0",pose,12);if(memory(c.Esp+8,&pose,4))wordsAt("transport_pose1",pose,12);DWORD input=0;wordsAt("transport_entry_stack",c.Esp,32);if(memory(c.Esp+16,&input,4)){wordsAt("transport_input_buffer",input,4);arrayAt("transport_input_point",input,0,8,36,9);}}
 if(b.offset==0x350f0){wordsAt("raw_copy_source",c.Ecx,25);wordsAt("raw_copy_destination",c.Eax,25);wordsAt("raw_copy_stack",c.Esp,64);}
 if(b.offset==0x74070){DWORD impl=0;if(memory(c.Esp+4,&impl,4)){wordsAt("converter_implementation",impl,64);arrayAt("converter_raw",impl,0x14,0x1c,100,25);arrayAt("converter_converted",impl,0x38,0x40,136,34);}wordsAt("converter_current_raw",c.Edi,25);}
 if(b.offset==0x7467b||b.offset==0x74684){wordsAt("converter_current_raw",c.Edi,25);wordsAt("converter_current_output",c.Eax,34);}
 if(b.offset==0x754c6){wordsAt("error_written",c.Eax,3);wordsAt("error_write_input",c.Ecx,3);}

 if(b.offset==0x203979||b.offset==0x203cdf||b.offset==0x203d4f||b.offset==0x203dea||b.offset==0x20418d){
  wordsAt("material_stack",c.Esp,80);
  if(b.offset==0x203979)wordsAt("material_input_error",c.Eax,3);
  if(b.offset==0x203cdf)wordsAt("material_global_error",c.Esp+0xc,3);
  if(b.offset==0x20418d)wordsAt("material_output_error",c.Esp+0x18,3);
 }
 if(b.offset==0x207440){DWORD arg=0,handle=0;if(memory(c.Esp+4,&arg,4)&&memory(arg,&handle,4)){backend(handle);wordsAt("dispatcher_arguments",c.Esp,5);}}
 if(b.offset==0x841e0||b.offset==0x84c74||b.offset==0x1e7e50){DWORD row=c.Ecx;if(b.offset==0x1e7e50&&!memory(c.Esp+4,&row,4))row=0;wordsAt("native_solver_row",row,32);wordsAt("native_bias_scale",coreBase+0x331624,1);DWORD body=0;for(unsigned i=0;i<2;++i)if(memory(row+4+i*4,&body,4)){char field[32];sprintf_s(field,"native_body%u",i);wordsAt(field,body,32);}}
 if(b.offset==0x751ab){wordsAt("solver_context",c.Ebx,8);DWORD array=0,size=0;if(memory(c.Ebx+0x10,&array,4)&&memory(c.Ebx+4,&size,4)&&size<=8192){fprintf(traceFile,",\"solver_bytes\":%lu",size);for(unsigned i=0;i<size/128&&i<8;++i){char field[32];sprintf_s(field,"final_row_%u",i);wordsAt(field,array+i*128,32);}}}
 if(b.offset==0x75158||b.offset==0x7518f){wordsAt("prepared_row",c.Esi,32);wordsAt("input_constraint",c.Edi,34);DWORD body=0;for(unsigned i=0;i<2;++i)if(memory(c.Esi+4+i*4,&body,4)){char field[32];sprintf_s(field,"solver_body%u",i);wordsAt(field,body,32);}}
 if(b.offset==0x20478d){DWORD arg=0,handle=0;if(memory(c.Esp,&arg,4)&&memory(arg,&handle,4))backend(handle);}
 if(b.offset==0x8d8fe){wordsAt("resolved_contact",c.Eax,128);DWORD vt=0;if(memory(c.Eax,&vt,4))wordsAt("resolved_vtable",vt,40);}
 if(b.offset==0x204330){DWORD object=0;if(memory(c.Esp+4,&object,4))wordsAt("parent_contact_object",object,32);}
 if(b.offset==0x229ee0) {
  wordsAt("box_matrix0",c.Edi,12);wordsAt("box_matrix1",c.Eax,12);
  DWORD arg=0;
  if(memory(c.Esp+16,&arg,4))wordsAt("box_half0",arg,3);
  if(memory(c.Esp+20,&arg,4))wordsAt("box_half1",arg,3);
  if(memory(c.Esp+24,&arg,4))wordsAt("box_axis_input",arg,1);
 }
 if(b.offset==0x22a562||b.offset==0x22aa53) {
  wordsAt("box_overlaps",c.Esp+0xb8,6);wordsAt("box_distances",c.Esp+0xd0,6);
  DWORD ptr=0;if(memory(c.Ebp+0x1c,&ptr,4))wordsAt("box_axis_cache",ptr,1);
  wordsAt("box_abs_rotation",c.Esp+0x68,9);
  wordsAt("box_matrix0",c.Edi,12);wordsAt("box_matrix1",c.Esi,12);
  if(memory(c.Ebp+0x14,&ptr,4))wordsAt("box_half0",ptr,3);
  if(memory(c.Ebp+0x18,&ptr,4))wordsAt("box_half1",ptr,3);
 }
 fprintf(traceFile,"}\n");fflush(traceFile);
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
   const DWORD offsets[]={0x204330,0x207440,0x207775,0x1df250,0x8d8fe,0x20478d,0x75158,0x7518f,0x751ab,0x841e0,0x84c74,0x1e7e50,0x74070,0x7467b,0x74684,0x75490,0x754aa,0x754c6,0x75560,0x203900,0x203979,0x203cdf,0x203d4f,0x203dea,0x20418d,0x350f0,0x2537a0,0x76fe2,0x76fe5,0x2270b5,0x227120,0x227165,0x227237,0x22733c,0x22ca45,0x7c5a0,0x7c8a2,0x7f1e3,0x229ee0,0x22a562,0x22aa53};const char* names[]={"observed-parent-entry","observed-dispatcher-entry","observed-user-callback-return","core-create-entry","observed-contact-lookup-return","observed-dispatcher-return","prepared-row-before-response","prepared-row-after-response","prepared-rows-final-buffer","solver-initialize-entry","solver-initialize-exit","solver-iteration-entry","converted-create-entry","converted-error-before-copy","converted-error-after-copy","set-error-entry","set-error-before-write","set-error-after-write","set-frame0-entry","material-contact-entry","material-inputs-ready","material-global-error","material-frame-adjusted","material-quaternion-adjusted","material-error-ready","raw-point-copy-entry","point-transport-entry","geometry-dispatch-before","geometry-dispatch-after","edge-normalized-direction-ready","edge-crossing-before-divide","edge-crossing-after-plane","edge-crossing-accepted","edge-contact-submit","geometry-box-matrix-ready","native-inertia-prepare-entry","native-inertia-prepared","integrator-tick","box-generation-entry","box-overlap-before-cache","box-axis-selected"};
   for(int i=0;i<41;++i){Breakpoint b={offsets[i],base+offsets[i],0,0,names[i]};if(!memory(b.address,&b.original,1)){fprintf(stderr,"Read code failed\n");return 3;}fprintf(traceFile,"{\"event\":\"breakpoint\",\"offset\":%lu,\"original_byte\":%u}\n",b.offset,(unsigned)b.original);points.push_back(b);patch(points.back(),true);}fprintf(stderr,"Instrumented %s base=%lu\n",name,base);
  }if(event.u.LoadDll.hFile)CloseHandle(event.u.LoadDll.hFile);}
  else if(event.dwDebugEventCode==EXCEPTION_DEBUG_EVENT){const DWORD code=event.u.Exception.ExceptionRecord.ExceptionCode;const DWORD address=(DWORD)event.u.Exception.ExceptionRecord.ExceptionAddress;HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT,FALSE,event.dwThreadId);CONTEXT context;ZeroMemory(&context,sizeof(context));context.ContextFlags=CONTEXT_ALL;bool own=false;
   if(code==EXCEPTION_BREAKPOINT){for(unsigned i=0;i<points.size();++i)if(points[i].address==address){if(!GetThreadContext(thread,&context))return 3;Breakpoint& b=points[i];++b.hits;if(b.offset==0x7f1e3)++currentSimulationStep;record(b,context);if(false&&b.offset==0x751ab&&b.hits==1&&!watchArmed){DWORD array=0;if(!memory(context.Ebx+0x10,&array,4))return 3;watchAddress[0]=array+0x60;watchAddress[1]=array+0x10;watchAddress[2]=array+0x40;watchAddress[3]=array+0x74;watchArmed=true;configureWatch(true);context.Dr0=watchAddress[0];context.Dr1=watchAddress[1];context.Dr2=watchAddress[2];context.Dr3=watchAddress[3];context.Dr6=0;context.Dr7=0xffff0055;}patch(b,false);context.Eip=address;context.EFlags|=0x100;if(!SetThreadContext(thread,&context))return 3;pending[event.dwThreadId]=(int)i;own=true;break;}}
   else if(code==EXCEPTION_SINGLE_STEP&&pending.count(event.dwThreadId)){int index=pending[event.dwThreadId];pending.erase(event.dwThreadId);if(targetSampleTickV71>=0 ? completedSamplesV71<=targetSampleTickV71 : ((targetStep>=0 && currentSimulationStep<=targetStep) || points[index].hits<(points[index].offset==0x1e7e50?512:points[index].offset==0x8d8fe?512:256)))patch(points[index],true);if(!GetThreadContext(thread,&context))return 3;context.EFlags&=~0x100;SetThreadContext(thread,&context);own=true;}
   if(code==EXCEPTION_SINGLE_STEP&&watchArmed){if(!GetThreadContext(thread,&context))return 3;if(context.Dr6&15){Breakpoint w={context.Eip-coreBase,context.Eip,0,++watchHits,"solver-row-watch"};fprintf(traceFile,"{\"event\":\"watch-status\",\"dr6\":%lu,\"dr7\":%lu}\n",context.Dr6,context.Dr7);record(w,context);context.Dr6=0;if(watchHits>=160){context.Dr7=0;configureWatch(false);}if(!SetThreadContext(thread,&context))return 3;own=true;}}
   if(thread)CloseHandle(thread);if(!own&&code!=EXCEPTION_BREAKPOINT)status=DBG_EXCEPTION_NOT_HANDLED;
  }else if(event.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT){result=event.u.ExitProcess.dwExitCode;done=true;}
  ContinueDebugEvent(event.dwProcessId,event.dwThreadId,status);
 }
 CloseHandle(processHandle);fclose(traceFile);return (int)result;
}
