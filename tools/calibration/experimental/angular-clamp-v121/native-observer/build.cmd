@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\clamp-v121
cl /nologo /O2 /EHsc trace-clamp.cpp /link /OUT:TraceClampV121.exe psapi.lib
