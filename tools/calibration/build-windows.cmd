@echo off
setlocal
if "%~2"=="" (
 echo Usage: build-windows.cmd SDK-prog-dir output-dir
 exit /b 2
)
set "cal_source=%~f1"
set "cal_output=%~f2"
if not exist "%cal_output%" mkdir "%cal_output%"
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
pushd "%cal_output%"
cl.exe /nologo /c /EHsc /DWIN32 /DNOMINMAX /MD /O2 /I"%~dp0." /I"%~dp0..\..\src\LexStd\header" /I"%cal_source%\Third Party\include\px\Physics\include" /I"%cal_source%\Third Party\include\px\Foundation\include" /I"%cal_source%\Third Party\include\px\PhysXLoader\include" "%~dp0windows.cpp"
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:"%cal_output%\MotorRock-PhysicsCalibration.exe" windows.obj /LIBPATH:"%cal_source%\Third Party\bin\px" PhysXLoader.lib
set "cal_exit=%ERRORLEVEL%"
popd
exit /b %cal_exit%
