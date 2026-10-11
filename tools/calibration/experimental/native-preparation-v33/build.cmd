@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /c /EHsc /MD /O2 /DWIN32 /DNOMINMAX /Fo"%~dp0trace-contact.obj" "%~dp0trace-contact.cpp"
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:"%~dp0TraceContact.exe" "%~dp0trace-contact.obj" Psapi.lib
exit /b %ERRORLEVEL%
