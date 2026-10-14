@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\interpolation-v122
cl /nologo /O2 /EHsc trace-interpolation.cpp /link /OUT:TraceInterpolationV122.exe psapi.lib
