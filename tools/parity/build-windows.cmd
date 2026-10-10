@echo off
setlocal
if "%~1"=="" (
 echo Usage: build-windows.cmd path-to-sdk-prog [Debug^|Release]
 exit /b 2
)
set "parity_source=%~f1"
set "parity_configuration=%~2"
if "%parity_configuration%"=="" set "parity_configuration=Debug"
if not defined VS90COMNTOOLS (
 echo Visual C++ 2008 SP1 is required. Run from its developer command prompt.
 exit /b 2
)
call "%VS90COMNTOOLS%..\..\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
copy /y "%~dp0RRR3d-game.sln" "%parity_source%\RRR3d-game.sln" >nul
pushd "%parity_source%"
"%VS90COMNTOOLS%..\..\VC\vcpackages\VCBuild.exe" /nologo /rebuild RRR3d-game.sln "%parity_configuration%|Win32"
set "parity_result=%ERRORLEVEL%"
popd
exit /b %parity_result%
