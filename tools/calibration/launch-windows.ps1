param([string]$Root=$PSScriptRoot,[string]$Case='all',[string]$Output='',[double]$Acceleration=3)
$ErrorActionPreference='Stop'
if(!$Output){$dir=Join-Path $env:LOCALAPPDATA 'MotorRock\calibration';New-Item -ItemType Directory $dir -Force | Out-Null;$Output=Join-Path $dir ((Get-Date -Format 'yyyyMMdd-HHmmss')+'-'+[Guid]::NewGuid().ToString('N').Substring(0,8)+'.jsonl')}
$exe=Join-Path $Root 'MotorRock-PhysicsCalibration.exe'
if(!(Test-Path $exe)){throw "Calibration binary not found: $exe"}
$parent=Split-Path $Output -Parent
if($parent){New-Item -ItemType Directory $parent -Force | Out-Null}
$previousAcceleration=$env:RRR3D_CALIBRATION_ACCELERATION
$env:RRR3D_CALIBRATION_ACCELERATION=$Acceleration.ToString([Globalization.CultureInfo]::InvariantCulture)
Push-Location $Root
try{& $exe $Output $Case;if($LASTEXITCODE -ne 0){throw "Calibration failed: $LASTEXITCODE"}}finally{Pop-Location;$env:RRR3D_CALIBRATION_ACCELERATION=$previousAcceleration}
Write-Output ('Calibration trace: '+$Output)
