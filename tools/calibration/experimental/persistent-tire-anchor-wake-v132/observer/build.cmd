@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\post-impulse-v132
cl /nologo /O2 /EHsc trace-world.cpp /link /OUT:TraceWorldV132.exe psapi.lib
