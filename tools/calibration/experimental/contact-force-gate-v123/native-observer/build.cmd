@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\force-v123
cl /nologo /O2 /EHsc trace-force.cpp /link /OUT:TraceForceV123.exe psapi.lib
