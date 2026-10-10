#!/usr/bin/env python3
"""Run the installed PhysX reference via Parallels Tools; copy a fresh trace."""
import argparse
import base64
from pathlib import Path
import subprocess
import uuid


def ps(value): return "'"+str(value).replace("'","''")+"'"

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--out',required=True);p.add_argument('--vm',default='Windows 11');p.add_argument('--root',default=r'C:\MotorRock\calibration');p.add_argument('--case',default='all');p.add_argument('--acceleration',type=float,default=3);p.add_argument('--timeout',type=int,default=300);a=p.parse_args()
    host=Path(a.out).expanduser().resolve();host.parent.mkdir(parents=True,exist_ok=True)
    # Parallels exposes full host paths through AllFiles. No fixed username.
    share='\\\\Mac\\AllFiles'+str(host).replace('/','\\')
    local=r'C:\MotorRock\logs\calibration-'+uuid.uuid4().hex+'.jsonl'
    code=f'''$ErrorActionPreference='Stop'
$env:RRR3D_CALIBRATION_ACCELERATION={ps(a.acceleration)}
Set-Location {ps(a.root)}
& (Join-Path {ps(a.root)} 'MotorRock-PhysicsCalibration.exe') {ps(local)} {ps(a.case)}
if($LASTEXITCODE -ne 0){{exit $LASTEXITCODE}}
Copy-Item {ps(local)} {ps(share)} -Force
'''
    encoded=base64.b64encode(code.encode('utf-16-le')).decode('ascii')
    result=subprocess.run(['prlctl','exec',a.vm,'--current-user','powershell.exe','-NoProfile','-NonInteractive','-EncodedCommand',encoded],capture_output=True,text=True,timeout=a.timeout)
    if result.returncode:raise SystemExit(result.stderr or result.stdout or f'Parallels exit {result.returncode}')
    if not host.exists() or not host.stat().st_size:raise SystemExit('Reference produced no capture')
    print(host)
if __name__=='__main__':main()
