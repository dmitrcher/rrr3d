#!/usr/bin/env python3
"""Start/collect Windows parity captures through Parallels' supported CLI."""
import argparse
import base64
from pathlib import Path
import subprocess


def psquote(value):
    return "'"+str(value).replace("'", "''")+"'"


def unc(path):
    home = Path.home()
    path = Path(path).expanduser().resolve()
    try:
        relative = path.relative_to(home)
    except ValueError as e:
        raise ValueError(f'Path must be under {home} for Parallels Home sharing') from e
    return '\\\\Mac\\Home\\'+str(relative).replace('/','\\')


def run(vm, script):
    encoded = base64.b64encode(script.encode('utf-16le')).decode('ascii')
    subprocess.run(['prlctl','exec',vm,'--current-user','powershell.exe','-NoProfile','-NonInteractive','-ExecutionPolicy','Bypass','-EncodedCommand',encoded],check=True,timeout=60)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--vm', default='Windows 11')
    sub = p.add_subparsers(dest='command',required=True)
    launch = sub.add_parser('launch')
    launch.add_argument('--seed',type=int,default=42)
    launch.add_argument('--ai-driver',action='store_true')
    launch.add_argument('--guest-output',required=True)
    launch.add_argument('--game-dir',default='C:\\MotorRock\\game-debug')
    collect = sub.add_parser('collect')
    collect.add_argument('--guest-output',required=True)
    collect.add_argument('--out',required=True)
    args = p.parse_args()
    try:
        if args.command == 'launch':
            if not 0 <= args.seed <= 0xffffffff:
                raise ValueError('seed must be uint32')
            script = Path(__file__).with_name('launch-windows.ps1')
            run(args.vm, '& '+psquote(unc(script))+' -GameDir '+psquote(args.game_dir)+' -Seed '+str(args.seed)+' -OutputDir '+psquote(args.guest_output)+(' -AiDriver' if args.ai_driver else ''))
        else:
            target = Path(args.out).expanduser().resolve()
            target.mkdir(parents=True,exist_ok=True)
            run(args.vm, "$ErrorActionPreference='Stop'; Copy-Item -LiteralPath "+psquote(args.guest_output)+" -Destination "+psquote(unc(target))+" -Recurse -Force")
    except (ValueError, subprocess.SubprocessError, OSError) as e:
        p.exit(1,str(e)+'\n')

if __name__ == '__main__':
    main()
