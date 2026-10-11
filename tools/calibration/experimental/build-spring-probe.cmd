@echo off
setlocal
set "bias_sdk=%~1"
set "bias_out=%~2"
if not exist "%bias_out%" mkdir "%bias_out%"
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
pushd "%bias_out%"
cl.exe /nologo /c /EHsc /DWIN32 /DNOMINMAX /MD /O2 /I"%~dp0.." /I"%~dp0..\..\..\src\LexStd\header" /I"%bias_sdk%\src\LexStd\header" /I"%bias_sdk%\Third Party\include\px\Cooking\include" /I"%bias_sdk%\Third Party\include\px\Physics\include" /I"%bias_sdk%\Third Party\include\px\Foundation\include" /I"%bias_sdk%\Third Party\include\px\PhysXLoader\include" "%~dp0windows-spring-probe.cpp"
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:"%bias_out%\SpringProbe.exe" windows-spring-probe.obj C:\MotorRock\calibration\ReferenceEuler.obj /LIBPATH:"%bias_sdk%\Third Party\bin\px" PhysXLoader.lib
set "bias_exit=%ERRORLEVEL%"
popd
exit /b %bias_exit%
