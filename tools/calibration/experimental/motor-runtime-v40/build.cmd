@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /Fe"%~dp0MotorRuntime.exe" /Fo"%~dp0runtime.obj" "%~dp0runtime.cpp"
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /Fe"%~dp0Symbols.exe" /Fo"%~dp0symbols.obj" "%~dp0symbols.cpp"
exit /b %ERRORLEVEL%
