@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\wake-sleep-v128
cl /nologo /O2 /EHsc trace-world.cpp /link /OUT:TraceWorldV124.exe psapi.lib
