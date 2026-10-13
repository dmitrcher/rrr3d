from pathlib import Path
import zipfile
root=Path.cwd();out=root/'work/pair-contact-v71/frame-tracers';out.mkdir(exist_ok=False)
helper=r'''
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
'''
for kind,source in [('wheel',root/'work/wheel-row-v56/tracer/trace-wheel.cpp'),('contact',root/'work/lost-tires-v59/border-contact/trace-contact.cpp')]:
 s=source.read_text();a='static int targetStep=-1,currentSimulationStep=-1;';assert s.count(a)==1;s=s.replace(a,a+helper)
 a='static void record(Breakpoint& b,const CONTEXT& c){';assert s.count(a)==1;s=s.replace(a,a+'\n if(targetSampleTickV71>=0){syncPhysicalTickV71();if(completedSamplesV71!=targetSampleTickV71)return;}')
 # Add the authoritative frame marker to all recorded events.
 a=' wordsAt("stack",c.Esp,48);' if kind=='wheel' else ' fprintf(traceFile,",\\\"simulation_step\\\":%d,\\\"ebp\\\":%lu",currentSimulationStep,c.Ebp);'
 if kind=='wheel':assert s.count(a)==1;s=s.replace(a,' fprintf(traceFile,",\\\"sample_tick_v71\\\":%d",completedSamplesV71);'+a)
 else:assert s.count(a)==1;s=s.replace(a,a+'fprintf(traceFile,",\\\"sample_tick_v71\\\":%d",completedSamplesV71);')
 a=' if(fopen_s(&traceFile,argv[1],"w")!=0)return 3;';assert s.count(a)==1
 s=s.replace(a,' const char* sampleFlagV71=getenv("MOTORROCK_NATIVE_TRACE_SAMPLE_TICK_V71");if(sampleFlagV71&&*sampleFlagV71){targetSampleTickV71=atoi(sampleFlagV71);if(targetSampleTickV71<0||targetSampleTickV71>=600)return 2;targetStep=-1;physicalPathV71=argv[3];}\n'+a)
 if kind=='wheel':
  a='if(targetStep>=0 ? currentSimulationStep<=targetStep : points[index].hits<8192)';b='if(targetSampleTickV71>=0 ? completedSamplesV71<=targetSampleTickV71 : (targetStep>=0 ? currentSimulationStep<=targetStep : points[index].hits<8192))'
 else:
  a='if((targetStep>=0 && currentSimulationStep<=targetStep) || points[index].hits<(points[index].offset==0x1e7e50?512:points[index].offset==0x8d8fe?512:256))';b='if(targetSampleTickV71>=0 ? completedSamplesV71<=targetSampleTickV71 : ((targetStep>=0 && currentSimulationStep<=targetStep) || points[index].hits<(points[index].offset==0x1e7e50?512:points[index].offset==0x8d8fe?512:256)))'
 assert s.count(a)==1;s=s.replace(a,b);(out/f'trace-{kind}.cpp').write_text(s)
(out/'build.cmd').write_text('''@echo off
setlocal
call "C:\\Program Files (x86)\\Microsoft Visual Studio 9.0\\VC\\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /DWIN32 /DNOMINMAX /Fo"%~dp0trace-wheel.obj" /Fe"%~dp0TraceWheelFrameV71.exe" "%~dp0trace-wheel.cpp" Psapi.lib
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /DWIN32 /DNOMINMAX /Fo"%~dp0trace-contact.obj" /Fe"%~dp0TraceContactFrameV71.exe" "%~dp0trace-contact.cpp" Psapi.lib
exit /b %ERRORLEVEL%
''')
with zipfile.ZipFile(out/'source.zip','w',zipfile.ZIP_DEFLATED) as z:
 for n in ['trace-wheel.cpp','trace-contact.cpp','build.cmd']:z.write(out/n,n)
print('Created frame-aligned read-only debugger sources')
