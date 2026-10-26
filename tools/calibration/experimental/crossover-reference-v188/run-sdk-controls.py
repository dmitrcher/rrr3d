from pathlib import Path
import os,subprocess,json,time,hashlib,signal
root=Path.cwd();w=root/'work/crossover-reference-v188';prefix=(w/'prefix').resolve();stage=prefix/'drive_c/MotorRock/sdk-reference';wine='/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine';env=dict(os.environ);env.update(WINEPREFIX=str(prefix),CX_BOTTLE=str(prefix),CX_BOTTLE_PATH=str(w.resolve()),WINEDEBUG='-all',WINEDLLOVERRIDES='winemenubuilder.exe=d;winedbg.exe=d')
for i in range(2):
 out=stage/f'controls{i}.jsonl';out.unlink(missing_ok=True);cmd=[wine,'--bottle',str(prefix),'--no-gui','--debugmsg','-all,+loaddll','--no-update','--workdir',r'C:\MotorRock\sdk-reference','--cx-app',r'C:\MotorRock\sdk-reference\MotorRock-PhysicsCalibration-v14.exe',fr'C:\MotorRock\sdk-reference\controls{i}.jsonl','list:gravity,game-air-gravity,game-air-pitch']
 t=time.monotonic()
 with (w/f'sdk-controls{i}.log').open('w') as log:
  p=subprocess.Popen(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
  try:code=p.wait(timeout=90)
  except subprocess.TimeoutExpired:
   os.killpg(p.pid,signal.SIGTERM);p.wait(timeout=10);raise
 receipt=dict(returncode=code,elapsed_seconds=time.monotonic()-t,prefix=str(prefix),output_exists=out.exists(),bytes=out.stat().st_size if out.exists() else None,command=cmd,environment={k:env[k] for k in ['WINEPREFIX','CX_BOTTLE','CX_BOTTLE_PATH','WINEDEBUG','WINEDLLOVERRIDES']},vm_started=False)
 (w/f'sdk-controls{i}-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(i,code,receipt['output_exists'],receipt['bytes'],flush=True)
 assert code==0 and out.exists(),'Original SDK control failed'
