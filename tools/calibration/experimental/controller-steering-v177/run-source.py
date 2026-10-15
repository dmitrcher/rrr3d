from pathlib import Path
import subprocess,os,json,struct,gzip,hashlib
p=Path('outputs/controller-steering-v177');p.mkdir(exist_ok=True);exe=Path('work/controller-steering-v177/SourceSteeringProbe').resolve();native=[json.loads(x) for x in gzip.open('outputs/controller-steering-v176/native0.jsonl.gz','rt')];reports=[]
def word(f):return struct.unpack('<I',struct.pack('<f',f))[0] if f else 0
for mode in ('off','on','raw'):
 for rep in range(2):
  commands=p/f'{mode}{rep}.jsonl';trace=p/f'{mode}{rep}-angles.jsonl';assert not trace.exists()
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env['RRR3D_EXPERIMENT_GAMECAR_STEER_V177']='0' if mode=='off' else '1';env['RRR3D_GAMECAR_STEERING_TRACE_FILE_V176']=str(trace.resolve())
  subprocess.run([str(exe),str(commands.resolve()),*(['--raw'] if mode=='raw' else [])],env=env,check=True,timeout=60)
  cmds=[json.loads(x) for x in commands.read_text().splitlines()];angles=[json.loads(x) for x in trace.read_text().splitlines()];assert len(cmds)==15 and len(angles)==30;bad=[];visual=0
  for i,(n,c) in enumerate(zip(native,cmds)):
   before=[word(x) for x in angles[i*2]['physical']];after=[word(x) for x in angles[i*2+1]['physical']];assert before==n['before'];assert c['visual_word']==n['visual'][0];visual+=1
   target=n['after_step'] if mode!='raw' else n['visual']
   if target!=after:bad.append(dict(case=i,expected=target,actual=after))
  reports.append(dict(mode=mode,repeat=rep,cases=15,visual_exact=visual,physical_exact=15-len(bad),differences=bad));print(mode,rep,'physical exact',15-len(bad),flush=True)
  assert len(bad)==(14 if mode=='off' else 0)
 for suffix in ('.jsonl','-angles.jsonl'):assert (p/f'{mode}0{suffix}').read_bytes()==(p/f'{mode}1{suffix}').read_bytes()
(p/'comparison.json').write_text(json.dumps(dict(scope='Actual original WheelsProgress versus actual Source GameCar/Jolt; stationary airborne four-wheel car, zero gravity/motor/brake, physical baseline +/- quarter lock and zero. Raw SDK steering negative control is separately checked. No moving/contact full-race parity claim.',runs=reports,repeats_identical=True,source_probe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),goal_complete=False),indent=2)+'\n')
