@echo off
rem Task-owned extraction only. Run in an isolated task environment after CRT activation is qualified.
set "PATH=%~dp0VC\bin;%~dp0Common7\IDE;%PATH%"
set "INCLUDE=%~dp0VC\include;%~dp0WindowsSDK\Include"
set "LIB=%~dp0VC\lib;%~dp0WindowsSDK\Lib"
