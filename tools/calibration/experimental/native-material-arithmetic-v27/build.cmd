@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
pushd "%~dp0"
cl.exe /nologo /c /EHsc /DWIN32 /DNOMINMAX /MT /O2 /fp:precise /arch:SSE2 /Foroots.obj roots.cpp
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:roots.exe roots.obj
if errorlevel 1 exit /b 1
cl.exe /nologo /c /EHsc /DWIN32 /DNOMINMAX /MT /O2 /fp:precise /arch:SSE2 /Foseeds.obj seeds.cpp
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:seeds.exe seeds.obj
if errorlevel 1 exit /b 1
popd
exit /b 0
