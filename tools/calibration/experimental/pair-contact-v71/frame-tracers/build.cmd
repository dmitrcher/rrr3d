@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /DWIN32 /DNOMINMAX /Fo"%~dp0trace-wheel.obj" /Fe"%~dp0TraceWheelFrameV71.exe" "%~dp0trace-wheel.cpp" Psapi.lib
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /DWIN32 /DNOMINMAX /Fo"%~dp0trace-contact.obj" /Fe"%~dp0TraceContactFrameV71.exe" "%~dp0trace-contact.cpp" Psapi.lib
exit /b %ERRORLEVEL%
