"""Run the packaged bounded jump checks. This does not certify full game parity."""
from pathlib import Path
import gzip,hashlib,importlib.util,json,os,subprocess,sys
B=Path(__file__).resolve().parent;OUT=Path(sys.argv[1]).resolve() if len(sys.argv)>1 else B/'verification';OUT.mkdir(parents=True,exist_ok=True);M=json.loads((B/'manifest.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for name,h in M['files'].items():assert sha(B/name)==h,name
spec=importlib.util.spec_from_file_location('floatwords',B/'compare-float32.py');h=importlib.util.module_from_spec(spec);spec.loader.exec_module(h)
def rows(p):
 with gzip.open(p,'rt') if p.suffix=='.gz' else p.open() as f:return [json.loads(s) for s in f]
def cz(x):return [cz(y) for y in x] if isinstance(x,list) else 0 if type(x)is int and x==2147483648 else x
expected=[x for x in rows(B/'compatibility-jump.jsonl.gz') if x['event']=='jump.sample'];assert len(expected)==530
fields=['spring_after_word','position_after_step','velocity_after_step','rotation_after_step','momentum_after_step','awake_after_step'];incoming=['velocity_before','rotation_before'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_','MOTORROCK_'))};env.update(json.loads((B/'profile.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(B/'windows-source-v14.cfg');reports=[];previous=None
for rep in range(2):
 dst=OUT/f'method{rep}.jsonl'
 with (OUT/f'method{rep}.log').open('w') as log:subprocess.run([str(B/'SourceJumpRuntimeProbe'),str(B/'input.words'),str(dst)],env=env,stdout=log,stderr=log,check=True,timeout=180)
 actual=rows(dst);assert len(actual)==530;assert all(x['case']==y['case'] and all(cz(x[k])==cz(y[k]) for k in incoming+fields) for x,y in zip(expected,actual));assert previous is None or previous==actual;previous=actual;reports.append(dict(check='method-and-step',repeat=rep,exact=530,cases=530));print('method',rep,'530/530',flush=True)
# Same executable/input with the correction disabled must expose the former difference.
dst=OUT/'disabled.jsonl';off=dict(env);off['RRR3D_EXPERIMENT_GAMECAR_JUMP_V187']='0'
with (OUT/'disabled.log').open('w') as log:subprocess.run([str(B/'SourceJumpRuntimeProbe'),str(B/'input.words'),str(dst)],env=off,stdout=log,stderr=log,check=True,timeout=180)
actual=rows(dst);assert len(actual)==530;different=sum(any(cz(x[k])!=cz(y[k]) for k in fields) for x,y in zip(expected,actual));assert different==336;reports.append(dict(check='disabled-negative-control',different=336,cases=530));print('disabled control336 differences',flush=True)
reference=rows(B/'native-sdk.jsonl.gz');ref={n:[h.physical(x['sample']) for x in reference if x['event']=='calibration.sample' and x['detail']==n] for n in ['game-air-gravity','game-air-pitch']};assert all(len(x)==120 for x in ref.values());previous=None
air=dict(env);air['MOTORROCK_JUMP_SCENE_GRAVITY']='-20'
for rep in range(2):
 dst=OUT/f'air{rep}.jsonl'
 with (OUT/f'air{rep}.log').open('w') as log:subprocess.run([str(B/'SourceJumpGravityProbe'),str(dst),'list:game-air-gravity,game-air-pitch'],env=air,stdout=log,stderr=log,check=True,timeout=90)
 actual=rows(dst);got={n:[h.physical(x['sample']) for x in actual if x['event']=='calibration.sample' and x['detail']==n] for n in ref};assert got==ref;assert previous is None or got==previous;previous=got;reports.append(dict(check='saved-windows-sdk-trajectories',repeat=rep,exact=240,states=240));print('saved Windows trajectories',rep,'240/240',flush=True)
r=dict(scope=M['scope'],checks=reports,vm_started=False,full_goal_complete=False);(OUT/'report.json').write_text(json.dumps(r,indent=2)+'\n');print('Bounded checks passed; full game parity remains unverified.')
