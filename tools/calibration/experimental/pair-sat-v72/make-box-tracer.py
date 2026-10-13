from pathlib import Path
import zipfile
root=Path.cwd();folder=root/'work/pair-contact-v72/box-tracer';folder.mkdir(exist_ok=False)
s=(root/'work/pair-contact-v71/frame-tracers/trace-contact.cpp').read_text()
a=''' fprintf(traceFile,"}\\n");fflush(traceFile);
}
int main'''
b=''' if(b.offset==0x229ee0) {
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
 fprintf(traceFile,"}\\n");fflush(traceFile);
}
int main'''
assert s.count(a)==1;s=s.replace(a,b)
s=s.replace('0x7f1e3};const char* names[]','0x7f1e3,0x229ee0,0x22a562,0x22aa53};const char* names[]')
s=s.replace('"integrator-tick"};','"integrator-tick","box-generation-entry","box-overlap-before-cache","box-axis-selected"};')
s=s.replace('i<38','i<41');(folder/'trace-box.cpp').write_text(s)
(folder/'build.cmd').write_text('@echo off\nsetlocal\ncall "C:\\Program Files (x86)\\Microsoft Visual Studio 9.0\\VC\\vcvarsall.bat" x86\nif errorlevel 1 exit /b 1\ncl.exe /nologo /EHsc /MD /O2 /DWIN32 /DNOMINMAX /Fo"%~dp0trace-box.obj" /Fe"%~dp0TraceBoxFrameV72.exe" "%~dp0trace-box.cpp" Psapi.lib\nexit /b %ERRORLEVEL%\n')
with zipfile.ZipFile(folder/'source.zip','w') as z:
 for n in ['trace-box.cpp','build.cmd']:z.write(folder/n,n)
print(folder)
