@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\controller-motor-v175
cl /nologo /O2 /EHsc /MD /DWIN32 /DNOMINMAX stack-probe.cpp /link /OUT:StackProbeV175.exe dbghelp.lib
