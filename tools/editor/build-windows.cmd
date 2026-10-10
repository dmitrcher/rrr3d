@echo off
setlocal
if "%~1"=="" (
  echo Usage: build-windows.cmd C:\MotorRock\source\prog [Release^|Debug]
  exit /b 2
)
set "editor_source=%~f1"
set "editor_config=%~2"
if "%editor_config%"=="" set "editor_config=Release"
if not "%editor_config%"=="Release" if not "%editor_config%"=="Debug" (
  echo Configuration must be Release or Debug.
  exit /b 2
)
set "editor_vc=C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC"
if defined MOTORROCK_VC90_ROOT set "editor_vc=%MOTORROCK_VC90_ROOT%"
if not exist "%editor_vc%\atlmfc\include\afxcontrolbars.h" (
  echo Visual C++ 2008 SP1 with MFC Feature Pack headers is required. Express does not include MFC.
  exit /b 3
)
if not exist "%editor_source%\MapEditor\MapEditor.vcproj" (
  echo Expected the original Windows SDK layout with MapEditor\MapEditor.vcproj under the source directory.
  exit /b 4
)
call "%editor_vc%\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
copy /y "%~dp0RRR3d-editor.sln" "%editor_source%\RRR3d-editor.sln" >nul
if errorlevel 1 exit /b 1
pushd "%editor_source%"
"%editor_vc%\vcpackages\VCBuild.exe" /nologo /rebuild RRR3d-editor.sln "%editor_config%|Win32"
set "editor_exit=%ERRORLEVEL%"
popd
exit /b %editor_exit%
