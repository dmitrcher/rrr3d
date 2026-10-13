@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /Od /arch:SSE2 /Fo"%~dp0combined-O0.obj" /Fe"%~dp0CombinedX87-O0.exe" "%~dp0verify-x87.cpp"
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /arch:SSE2 /Fo"%~dp0combined-O2.obj" /Fe"%~dp0CombinedX87-O2.exe" "%~dp0verify-x87.cpp"
exit /b %ERRORLEVEL%
