"""Qualify unchanged V133 Windows observer against saved Windows captures; no VM."""
from pathlib import Path
import collections,gzip,hashlib,json,os,signal,struct,subprocess,time
ROOT=Path.cwd(); W=ROOT/'work/crossover-reference-v188'; O=ROOT/'outputs/crossover-reference-v188/maps'; O.mkdir(exist_ok=True)
PREFIX=(W/'prefix').resolve();STAGE=PREFIX/'drive_c/MotorRock/sdk-reference'
EXE=STAGE/'WorldProbeV133.exe'; GAME=PREFIX/'drive_c/MotorRock/game-debug/Rock3dGame.dll'; D3DX=STAGE/'D3DX9_43.dll'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(EXE)=='3148e303225de65e706398c02b7855711b6a45ceaab141fa38446fd38910b89f'
assert sha(GAME)=='4483e8f402edb5c4639609dcf3fd805386e80f1d25b1c4829a8236b40a6b8a15'
assert sha(D3DX)=='0b28546be22c71834501f7d7185ede5d79742457331c7ee09efc14490dd64f5f'
def load(p):
 with gzip.open(p,'rt') if p.suffix=='.gz' else p.open() as f:return [json.loads(s) for s in f]
def words(a):
 if isinstance(a,bool) or a is None or isinstance(a,str):return a
 if isinstance(a,(int,float)):return struct.pack('<f',a).hex()
 if isinstance(a,list):return [words(x) for x in a]
 return {k:words(v) for k,v in a.items()}
results=[];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(WINEPREFIX=str(PREFIX),CX_BOTTLE=str(PREFIX),WINEDLLOVERRIDES='winemenubuilder.exe=d;winedbg.exe=d')
for track in [0,16,48,64]:
 previous=None
 for rep in range(2):
  name=f'track{track}-rep{rep}'; dst=STAGE/f'{name}-v188.jsonl'; assert not dst.exists(),dst
  cmd=['/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine','--bottle',str(PREFIX),'--no-gui','--dll','d3dx9_43=n','--debugmsg','-all,+loaddll','--no-update','--workdir',r'C:\MotorRock\sdk-reference','--cx-app',r'C:\MotorRock\sdk-reference\WorldProbeV133.exe',fr'C:\MotorRock\sdk-reference\track{track}.words',fr'C:\MotorRock\sdk-reference\{dst.name}']
  t=time.monotonic();log=W/f'{name}.log'
  with log.open('w') as f:
   p=subprocess.Popen(cmd,env=env,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
   try:code=p.wait(timeout=120)
   except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGTERM);p.wait(timeout=10);raise
  assert code==0,(name,code)
  native_loaded=any('d3dx9_43.dll' in s.lower() and 'native' in s for s in log.read_text().splitlines());assert native_loaded
  ref=ROOT/f'outputs/game-control-wake-v133/native/track{track}-rep{rep}.jsonl.gz';actual=load(dst);expected=load(ref)
  a=[x for x in actual if x['event']=='world.sample'];b=[x for x in expected if x['event']=='world.sample'];assert len(a)==len(b)==900
  bad=[dict(tick=x['tick'],fields=[k for k in x if words(x[k])!=words(y[k])]) for x,y in zip(a,b) if words(x)!=words(y)]
  am=[x for x in actual if x['event']!='world.sample'];bm=[x for x in expected if x['event']!='world.sample'];assert len(am)==len(bm)==3
  metadata=[dict(event=x['event'],actual=x,windows=y) for x,y in zip(am,bm) if words(x)!=words(y)]
  aw=words(a);same_repeat=previous is None or aw==previous;assert same_repeat;previous=aw
  raw=dst.read_bytes();capture=O/f'{name}.jsonl.gz'
  with gzip.GzipFile(filename=str(capture),mode='wb',mtime=0) as f:f.write(raw)
  entry=dict(track=track,repeat=rep,states=900,exact=900-len(bad),first_differences=bad[:5],field_counts=dict(collections.Counter(k for z in bad for k in z['fields'])),metadata_differences=metadata,repeats_identical=same_repeat,returncode=code,elapsed_seconds=time.monotonic()-t,reference=str(ref.relative_to(ROOT)),reference_sha256=sha(ref),capture=str(capture.relative_to(ROOT)),raw_sha256=hashlib.sha256(raw).hexdigest(),compressed_sha256=sha(capture),fixture_sha256=sha(STAGE/f'track{track}.words'),native_d3dx_loaded=native_loaded,command=cmd)
  (O/f'{name}-receipt.json').write_text(json.dumps(entry,indent=2)+'\n');results.append(entry);dst.unlink();print(name,entry['exact'],'/900',flush=True)
report=dict(scope='Four preserved one-car map SDK/control-wake trajectories, unchanged V133 observer, original GameDLL startup/control setter and native D3DX. No complete GameCar OnFixedStep, AI or rendered race equivalence.',vm_started=False,full_goal_complete=False,observer_sha256=sha(EXE),game_sha256=sha(GAME),d3dx_sha256=sha(D3DX),runs=results,exact_states=sum(x['exact'] for x in results),states=sum(x['states'] for x in results))
(O/'comparison.json').write_text(json.dumps(report,indent=2)+'\n');assert all(x['exact']==900 and not x['metadata_differences'] for x in results)
