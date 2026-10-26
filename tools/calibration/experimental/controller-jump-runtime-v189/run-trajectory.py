from pathlib import Path
import gzip,hashlib,json,os,signal,subprocess,time
R=Path.cwd();W=R/'work/controller-jump-runtime-v189';O=R/'outputs/controller-jump-runtime-v189/trajectories';O.mkdir(exist_ok=True);P=(R/'work/crossover-reference-v188/prefix').resolve();D=P/'drive_c/MotorRock/jump-trajectory-v189';B=json.loads((W/'trajectory-build.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();assert sha(D/'JumpTrajectoryV189.exe')==B['observer_sha256'] and sha(W/'trajectory/jump-trajectory.cpp')==B['source_sha256'];runs=[]
for mode in ['sdk','actual-method']:
 for rep in range(2):
  dst=D/f'{mode}{rep}.jsonl';dst.unlink(missing_ok=True);env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_','MOTORROCK_'))};env.update(WINEPREFIX=str(P),CX_BOTTLE=str(P));
  if mode=='actual-method':env['MOTORROCK_ACTUAL_JUMP_V189']='1'
  cases='list:gravity,game-air-gravity,game-air-pitch' if mode=='sdk' else 'list:game-air-gravity,game-air-pitch';cmd=['/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine','--bottle',str(P),'--no-gui','--dll','msvcr90,msvcp90,msvcm90=n;d3dx9_43=n','--debugmsg','-all,+loaddll','--no-update','--workdir',r'C:\MotorRock\jump-trajectory-v189','--cx-app',r'C:\MotorRock\jump-trajectory-v189\JumpTrajectoryV189.exe',fr'C:\MotorRock\jump-trajectory-v189\{dst.name}',cases];t=time.monotonic()
  with (W/f'trajectory-{mode}{rep}.log').open('w') as log:
   p=subprocess.Popen(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
   try:code=p.wait(timeout=90)
   except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGTERM);p.wait(timeout=10);raise
  assert code==0,(mode,rep,code);raw=dst.read_bytes();rows=[json.loads(s) for s in raw.splitlines()];identity=[x['sample'] for x in rows if x['event']=='calibration.jump-route-v189'];assert len(identity)==(3 if mode=='sdk' else 2);assert all(x['source_sha256']==B['source_sha256'] and x['build_id']==B['build_id'] and x['actual_method']==(mode=='actual-method') for x in identity);states=[x for x in rows if x['event']=='calibration.sample'];assert len(states)==(360 if mode=='sdk' else 240)
  dest=O/f'{mode}{rep}.jsonl.gz';dest.write_bytes(gzip.compress(raw,mtime=0));runs.append(dict(mode=mode,repeat=rep,states=len(states),returncode=code,elapsed_seconds=time.monotonic()-t,command=cmd,raw_sha256=hashlib.sha256(raw).hexdigest(),compressed_sha256=sha(dest)));print(mode,rep,len(states),'states',flush=True)
(O/'compatibility-manifest.json').write_text(json.dumps(dict(build=B,runs=runs,scope='Same saved SDK airborne fixtures. Baseline raw API; actual method route only replaces original extra-gravity/pitch calls. Actor creation PC53, actual method PC24. Not a complete original game fixed step.',vm_started=False,full_goal_complete=False),indent=2)+'\n')
