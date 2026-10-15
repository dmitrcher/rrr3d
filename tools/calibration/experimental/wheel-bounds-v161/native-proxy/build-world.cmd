@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\wheel-bounds-v161c
cl /nologo /O2 /EHsc /MD /DWIN32 /DNOMINMAX /I"C:\MotorRock\source\prog\Third Party\include\px\Physics\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Foundation\include" /I"C:\MotorRock\source\prog\Third Party\include\px\PhysXLoader\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Cooking\include" /I"C:\MotorRock\calibration-probes\contact-pose-v45" world-probe.cpp /link /OUT:WorldProbeProxyBoundsV161c.exe /LIBPATH:"C:\MotorRock\source\prog\Third Party\bin\px" PhysXLoader.lib dbghelp.lib
