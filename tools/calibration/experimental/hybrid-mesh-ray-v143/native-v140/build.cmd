@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d "%~dp0"
cl /nologo /O2 /EHsc trace-world.cpp /link /OUT:TraceTreeV140.exe psapi.lib
