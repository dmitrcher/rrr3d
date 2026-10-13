@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /DWIN32 /DNOMINMAX /Fo"%~dp0trace-query.obj" /Fe"%~dp0TraceRowQueryV103.exe" "%~dp0trace-query.cpp" Psapi.lib
exit /b %ERRORLEVEL%
