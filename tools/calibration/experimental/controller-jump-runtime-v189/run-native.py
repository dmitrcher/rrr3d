from pathlib import Path
import gzip,hashlib,json,os,shutil,signal,subprocess,time
R=Path.cwd();W=R/'work/controller-jump-runtime-v189';O=R/'outputs/controller-jump-runtime-v189';P=(R/'work/crossover-reference-v188/prefix').resolve();D=P/'drive_c/MotorRock/jump-runtime-v189';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();b=json.loads((W/'native-build.json').read_text());assert sha(D/'JumpProbeV189.exe')==b['observer_sha256'];assert sha(W/'native/jump-probe.cpp')==b['source_sha256'];f=json.loads((O/'cases.json').read_text());assert sha(O/'input.words')==f['input_sha256'];shutil.copyfile(O/'input.words',D/'input.words');env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(WINEPREFIX=str(P),CX_BOTTLE=str(P));runs=[]
assert sha(P/'drive_c/MotorRock/game-debug/Rock3dGame.dll')=='4483e8f402edb5c4639609dcf3fd805386e80f1d25b1c4829a8236b40a6b8a15'
for rep in range(2):
 dst=D/f'run{rep}.jsonl';dst.unlink(missing_ok=True);cmd=['/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine','--bottle',str(P),'--no-gui','--dll','msvcr90,msvcp90,msvcm90=n;d3dx9_43=n','--debugmsg','-all,+loaddll','--no-update','--workdir',r'C:\MotorRock\jump-runtime-v189','--cx-app',r'C:\MotorRock\jump-runtime-v189\JumpProbeV189.exe',r'C:\MotorRock\jump-runtime-v189\input.words',fr'C:\MotorRock\jump-runtime-v189\run{rep}.jsonl'];t=time.monotonic()
 with (W/f'native-run{rep}.log').open('w') as log:
  p=subprocess.Popen(cmd,env=env,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
  try:code=p.wait(timeout=180)
  except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGTERM);p.wait(timeout=10);raise
 assert code==0,(rep,code)
 raw=dst.read_bytes();rows=[json.loads(s) for s in raw.splitlines()];assert rows[0]['source_sha256']==b['source_sha256'] and rows[0]['build_id']==b['build_id'];states=[x for x in rows if x['event']=='jump.sample'];assert len(states)==f['count'];assert [x['case'] for x in states]==list(range(f['count']));assert all(x['raw_fp_control']==127 for x in states)
 dest=O/f'compatibility-run{rep}.jsonl.gz';dest.write_bytes(gzip.compress(raw,mtime=0));runs.append(dict(repeat=rep,returncode=code,elapsed_seconds=time.monotonic()-t,command=cmd,raw_sha256=hashlib.sha256(raw).hexdigest(),archive_sha256=sha(dest),states=len(states)));print('Compatibility method capture',rep,len(states),'states',flush=True)
(O/'compatibility-manifest.json').write_text(json.dumps(dict(scope='Fresh original GameCar JumpProgress execution on actual SDK actor in CrossOver, provisional compatibility evidence, not new native Windows gold. No active-code hooks.',build=b,input_sha256=f['input_sha256'],runs=runs,vm_started=False,goal_complete=False),indent=2)+'\n')
