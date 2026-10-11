@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /c /EHsc /MD /O2 /DWIN32 /DNOMINMAX /Fo"%~dp0trace-mass.obj" "%~dp0trace-mass.cpp"
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:"%~dp0TraceMass.exe" "%~dp0trace-mass.obj" Psapi.lib
exit /b %ERRORLEVEL%
