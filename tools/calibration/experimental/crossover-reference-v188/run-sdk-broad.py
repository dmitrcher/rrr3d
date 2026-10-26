from pathlib import Path
import os,subprocess,json,hashlib,gzip,shutil,time,signal
root=Path.cwd();w=root/'work/crossover-reference-v188';prefix=(w/'prefix').resolve();stage=prefix/'drive_c/MotorRock/sdk-reference';out=root/'outputs/crossover-reference-v188/broad';out.mkdir(exist_ok=False)
exe=stage/'MotorRock-PhysicsCalibration-v14.exe';identity=hashlib.sha256(exe.read_bytes()).hexdigest();env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(WINEPREFIX=str(prefix),CX_BOTTLE=str(prefix),WINEDEBUG='-all,+loaddll',WINEDLLOVERRIDES='winemenubuilder.exe=d;winedbg.exe=d');runs=[]
for rep in range(2):
 path=stage/f'broad{rep}.jsonl';assert not path.exists();cmd=['/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine','--bottle',str(prefix),'--no-gui','--no-update','--workdir',r'C:\MotorRock\sdk-reference','--cx-app',r'C:\MotorRock\sdk-reference\MotorRock-PhysicsCalibration-v14.exe',fr'C:\MotorRock\sdk-reference\broad{rep}.jsonl','all'];t=time.monotonic()
 with (out/f'run{rep}.log').open('w') as log:
  p=subprocess.Popen(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
  try:code=p.wait(timeout=900)
  except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGTERM);p.wait(timeout=10);raise
 assert code==0,(rep,code)
 assert hashlib.sha256(exe.read_bytes()).hexdigest()==identity
 dest=out/f'run{rep}.jsonl.gz'
 with path.open('rb') as f,gzip.open(dest,'wb',compresslevel=5) as z:shutil.copyfileobj(f,z)
 rawsha=hashlib.sha256(path.read_bytes()).hexdigest();size=path.stat().st_size;path.unlink()
 runs.append(dict(repeat=rep,elapsed_seconds=time.monotonic()-t,returncode=code,uncompressed_bytes=size,uncompressed_sha256=rawsha,file=dest.name));(out/'manifest.json').write_text(json.dumps(dict(observer_sha256=identity,runs=runs,environment={k:env[k] for k in ['WINEPREFIX','CX_BOTTLE','WINEDEBUG','WINEDLLOVERRIDES']},vm_started=False),indent=2)+'\n');print(rep,'all608 finished, compressed',dest.stat().st_size,flush=True)
