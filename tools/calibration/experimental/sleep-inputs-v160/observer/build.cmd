@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d "%~dp0"
cl /nologo /O2 /EHsc trace-energy.cpp /link /OUT:TraceEnergyV160.exe psapi.lib
