@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\controller-fp-v172
cl /nologo /O2 /EHsc /MD /DWIN32 /DNOMINMAX fp-debugger.cpp /link /OUT:FpDebuggerV172.exe dbghelp.lib psapi.lib
