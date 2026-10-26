from pathlib import Path
import os,json,subprocess,time,signal
root=Path.cwd();w=root/'work/crossover-reference-v188';prefix=(w/'prefix').resolve();stage=prefix/'drive_c/MotorRock/game-debug';env=dict(os.environ);env.update(WINEPREFIX=str(prefix),CX_BOTTLE=str(prefix),WINEDEBUG='-all,+loaddll',WINEDLLOVERRIDES='winemenubuilder.exe=d;winedbg.exe=d;msvcr90d,msvcp90d,msvcm90d,msvcr90,msvcp90,msvcm90=n')
cmd=['/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine','--bottle',str(prefix),'--no-gui','--dll','msvcr90,msvcp90,msvcm90=n;d3dx9_43=n','--debugmsg','-all,+loaddll','--no-update','--workdir',r'C:\MotorRock\game-debug','--cx-app',r'C:\MotorRock\game-debug\StabilizeProbeV185PaddingControl.exe',r'C:\MotorRock\game-debug\native-d3dx-four0.jsonl'];t=time.monotonic()
with (w/'native-d3dx-four0.log').open('w') as log:
 p=subprocess.Popen(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
 try:code=p.wait(timeout=180)
 except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGTERM);p.wait(timeout=10);raise
out=stage/'native-d3dx-four0.jsonl';r=dict(returncode=code,elapsed_seconds=time.monotonic()-t,command=cmd,environment={k:env[k] for k in ['WINEPREFIX','CX_BOTTLE','WINEDEBUG','WINEDLLOVERRIDES']},output_exists=out.exists(),bytes=out.stat().st_size if out.exists() else None,vm_started=False);(w/'native-d3dx-four0-receipt.json').write_text(json.dumps(r,indent=2)+'\n');print(r,flush=True);assert code==0,'Original game control failed'
