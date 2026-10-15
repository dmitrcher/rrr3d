@echo off
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
if errorlevel 1 exit /b 1
cl.exe /nologo /EHsc /MD /O2 /Fe"%~dp0MotorFormula.exe" /Fo"%~dp0motor-formula.obj" "%~dp0motor-formula.cpp"
exit /b %ERRORLEVEL%
