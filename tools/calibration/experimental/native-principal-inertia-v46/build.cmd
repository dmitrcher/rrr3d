@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
set "task_sdk=C:\MotorRock\source\prog\Third Party"
cl.exe /nologo /EHsc /MD /O2 /DWIN32 /DNOMINMAX /I"%task_sdk%\include\px\Physics\include" /I"%task_sdk%\include\px\Foundation\include" /I"%task_sdk%\include\px\PhysXLoader\include" /Fo"%~dp0diagonal-probe.obj" /Fe"%~dp0DiagonalProbe.exe" "%~dp0diagonal-probe.cpp" /link /LIBPATH:"%task_sdk%\bin\px" PhysXLoader.lib
exit /b %ERRORLEVEL%
