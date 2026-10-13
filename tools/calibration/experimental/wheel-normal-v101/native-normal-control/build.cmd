@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cd /d "%~dp0"
for %%N in (control-normals) do (
cl.exe /nologo /c /EHsc /DWIN32 /DNOMINMAX /MD /O2 /I"." /I"C:\MotorRock\source\prog\LexStd\header" /I"C:\MotorRock\source\prog\Third Party\include\px\Cooking\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Physics\include" /I"C:\MotorRock\source\prog\Third Party\include\px\Foundation\include" /I"C:\MotorRock\source\prog\Third Party\include\px\PhysXLoader\include" /Fo"%%N.obj" "%%N.cpp"
if errorlevel 1 exit /b 1
link.exe /nologo /OUT:"%%N.exe" "%%N.obj" C:\MotorRock\calibration\ReferenceEuler.obj /LIBPATH:"C:\MotorRock\source\prog\Third Party\bin\px" PhysXLoader.lib
if errorlevel 1 exit /b 1
)
exit /b 0
