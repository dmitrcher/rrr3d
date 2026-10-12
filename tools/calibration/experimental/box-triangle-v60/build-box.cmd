@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /fp:precise /DWIN32 /DNOMINMAX /I"C:\MotorRock\source\prog\Third Party\include\px\Physics\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Foundation\include" /I"C:\MotorRock\source\prog\Third Party\include\px\PhysXLoader\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Cooking\include" /Fo"%~dp0BoxRay.obj" /Fe"%~dp0BoxRay.exe" "%~dp0mesh-api-windows.cpp" /link /LIBPATH:"C:\MotorRock\source\prog\Third Party\bin\px" PhysXLoader.lib
exit /b %ERRORLEVEL%
