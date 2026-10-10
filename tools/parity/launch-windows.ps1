param([string]$GameDir='C:\MotorRock\game-debug', [uint32]$Seed=42, [switch]$AiDriver, [string]$OutputDir='')
$ErrorActionPreference='Stop'
if (!$OutputDir) { $OutputDir=Join-Path 'C:\MotorRock\logs\parity' ('windows-'+(Get-Date -Format 'yyyyMMdd-HHmmss')) }
New-Item -ItemType Directory (Join-Path $OutputDir 'screenshots') -Force | Out-Null
$env:RRR3D_TRACE_FILE=Join-Path $OutputDir 'trace.jsonl'
$env:RRR3D_CAPTURE_DIR=Join-Path $OutputDir 'screenshots'
$env:RRR3D_SEED=$Seed.ToString()
$env:RRR3D_AI_DRIVER=if ($AiDriver) {'1'} else {'0'}
# Startup/race options come from the original _DEBUG code.
Write-Output ('Trace: '+$env:RRR3D_TRACE_FILE)
Start-Process (Join-Path $GameDir 'MR.exe') -WorkingDirectory $GameDir
