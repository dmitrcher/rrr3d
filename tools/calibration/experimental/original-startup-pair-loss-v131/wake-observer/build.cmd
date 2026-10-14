@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\startup-wake-v130
cl /nologo /O2 /EHsc trace-world.cpp /link /OUT:TraceWakeV130.exe psapi.lib
