param(
 [string]$EditorRoot='C:\MotorRock\editor',
 [string]$Map,
 [string]$LogRoot='C:\MotorRock\logs\editor'
)
$ErrorActionPreference='Stop'
$EditorRoot=(Resolve-Path $EditorRoot).Path
$exe=Join-Path $EditorRoot 'MapEditor.exe'
if(-not(Test-Path $exe)){throw "Editor not found: $exe"}
if($Map){$Map=(Resolve-Path $Map).Path}
$run=Join-Path $LogRoot ((Get-Date -Format 'yyyyMMdd-HHmmss-fff')+'-'+[Guid]::NewGuid().ToString('N').Substring(0,8))
New-Item -ItemType Directory $run -Force | Out-Null
$oldTrace=$env:RRR3D_TRACE_FILE
try{
 $env:RRR3D_TRACE_FILE=Join-Path $run 'trace.jsonl'
 $arguments=@{FilePath=$exe;WorkingDirectory=$EditorRoot;PassThru=$true}
 if($Map){$arguments.ArgumentList='"'+$Map+'"'}
 $p=Start-Process @arguments
 @{pid=$p.Id;editor=$exe;map=$Map;trace=$env:RRR3D_TRACE_FILE;started_utc=[DateTime]::UtcNow.ToString('o')} | ConvertTo-Json | Set-Content (Join-Path $run 'launch.json') -Encoding UTF8
 'EDITOR_PID='+$p.Id
 'EDITOR_RUN='+$run
}finally{$env:RRR3D_TRACE_FILE=$oldTrace}
