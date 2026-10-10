@echo off
setlocal
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Launch-Editor.ps1" -EditorRoot "%~dp0." -LogRoot "%LOCALAPPDATA%\MotorRock\editor\logs" %*
exit /b %ERRORLEVEL%
