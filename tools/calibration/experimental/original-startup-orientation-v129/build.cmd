@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
cd /d C:\MotorRock\calibration-probes\startup-v129
cl /nologo /O2 /EHsc /MD startup-audit.cpp /link /OUT:StartupAuditV129.exe dbghelp.lib d3d9.lib user32.lib
