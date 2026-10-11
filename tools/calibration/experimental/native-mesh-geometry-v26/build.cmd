@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
pushd "%~dp0"
cl.exe /nologo /c /EHsc /DWIN32 /DNOMINMAX /MT /O2  /Fogeometry-default.obj mesh-geometry.cpp
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:geometry-default.exe geometry-default.obj
if errorlevel 1 exit /b 1
cl.exe /nologo /c /EHsc /DWIN32 /DNOMINMAX /MT /O2 /fp:strict /Fogeometry-strict.obj mesh-geometry.cpp
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:geometry-strict.exe geometry-strict.obj
if errorlevel 1 exit /b 1
cl.exe /nologo /c /EHsc /DWIN32 /DNOMINMAX /MT /O2 /fp:fast /Fogeometry-fast.obj mesh-geometry.cpp
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:geometry-fast.exe geometry-fast.obj
if errorlevel 1 exit /b 1
cl.exe /nologo /c /EHsc /DWIN32 /DNOMINMAX /MT /O2 /arch:SSE2 /Fogeometry-sse2.obj mesh-geometry.cpp
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:geometry-sse2.exe geometry-sse2.obj
if errorlevel 1 exit /b 1
popd
exit /b 0
