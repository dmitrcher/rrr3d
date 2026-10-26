from pathlib import Path
import subprocess,os,json,time,signal
root=Path.cwd();w=root/'work/crossover-reference-v188';prefix=(w/'prefix').resolve();env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(WINEPREFIX=str(prefix),CX_BOTTLE=str(prefix),WINEDLLOVERRIDES='winemenubuilder.exe=d;winedbg.exe=d');cmd=['/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine','--bottle',str(prefix),'--no-gui','--dll','d3dx9_43=n','--debugmsg','-all,+loaddll','--no-update','--workdir',r'C:\MotorRock\sdk-reference','--cx-app',r'C:\MotorRock\sdk-reference\WorldProbeV133.exe',r'C:\MotorRock\sdk-reference\track0.words',r'C:\MotorRock\sdk-reference\map-native-d3dx0.jsonl'];t=time.monotonic()
with (w/'map-native-d3dx0.log').open('w') as log:
 p=subprocess.Popen(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
 try:code=p.wait(timeout=120)
 except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGTERM);p.wait(timeout=10);raise
r=dict(returncode=code,elapsed_seconds=time.monotonic()-t,command=cmd,vm_started=False);(w/'map-native-d3dx0-receipt.json').write_text(json.dumps(r,indent=2)+'\n');print(r,flush=True);assert code==0
