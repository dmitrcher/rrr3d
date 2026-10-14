@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\shape-origin-v152
cl /nologo /O2 /EHsc /MD /DWIN32 /DNOMINMAX /I"C:\MotorRock\source\prog\Third Party\include\px\Physics\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Foundation\include" /I"C:\MotorRock\source\prog\Third Party\include\px\PhysXLoader\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Cooking\include" /I"C:\MotorRock\calibration-probes\contact-pose-v45" shape-origin.cpp /link /OUT:ShapeOriginV152.exe /LIBPATH:"C:\MotorRock\source\prog\Third Party\bin\px" PhysXLoader.lib dbghelp.lib
