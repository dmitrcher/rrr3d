@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /DWIN32 /DNOMINMAX /Fo"%~dp0trace-callback.obj" /Fe"%~dp0TraceCallbackV84.exe" "%~dp0trace-callback.cpp" Psapi.lib
exit /b %ERRORLEVEL%
