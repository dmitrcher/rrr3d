"""Recorded SDK trajectories plus Source-derived JumpProgress command semantics.
No native GameCar runtime or full contact/timer execution is claimed.
"""
from pathlib import Path
import json,os,subprocess,hashlib,importlib.util
root=Path.cwd();out=root/'outputs/controller-jump-v187/recorded-air';out.mkdir(exist_ok=True)
exe=root/'work/controller-jump-v187/SourceJumpGravityProbe';base=json.loads((root/'outputs/controller-clutch-v186/final-broad/manifest.json').read_text())['environment'];spec=importlib.util.spec_from_file_location('words',root/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
cases={'gravity':'0','game-air-gravity':'-20','game-air-pitch':'-20'};native={n:[] for n in cases}
with (root/'outputs/calibration-v14-source-windows.jsonl').open() as f:
 for r in map(json.loads,f):
  if r['event']=='calibration.sample' and r['detail'] in native:native[r['detail']].append(r['sample'])
assert all(len(v)==120 for v in native.values());reports=[]
for name,scene in cases.items():
 for mode in ['off','on']:
  previous=None
  for rep in range(2):
   env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_','MOTORROCK_JUMP_'))};env.update(base);env.update(MOTORROCK_JUMP_SCENE_GRAVITY=scene,RRR3D_EXPERIMENT_GAMECAR_JUMP_V187='1' if mode=='on' else '0');path=out/f'{name}-{mode}{rep}.jsonl'
   with path.with_suffix('.log').open('w') as log:subprocess.run([str(exe),str(path),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=90)
   rows=[r['sample'] for r in map(json.loads,path.read_text().splitlines()) if r['event']=='calibration.sample'];assert len(rows)==120;semantic=[words.physical(r) for r in rows]
   if previous is not None:assert semantic==previous
   previous=semantic;diff=[dict(tick=i,fields=[k for k in words.physical(n) if words.physical(n)[k]!=m[k]],windows=n,mac=rows[i]) for i,(n,m) in enumerate(zip(native[name],semantic)) if words.physical(n)!=m]
   reports.append(dict(case=name,mac_scene_gravity=float(scene),windows_sdk_scene_gravity=-20,mode=mode,repeat=rep,exact=120-len(diff),first_difference=diff[0] if diff else None));print(name,mode,rep,reports[-1]['exact'],'/120',flush=True)
# A caller lacking the Session provenance marker must retain the old behavior.
path=out/'unmarked.jsonl';env.update(MOTORROCK_JUMP_SCENE_GRAVITY='0',MOTORROCK_JUMP_UNMARKED='1',RRR3D_EXPERIMENT_GAMECAR_JUMP_V187='1')
with path.with_suffix('.log').open('w') as log:subprocess.run([str(exe),str(path),'list:gravity'],env=env,stdout=log,stderr=log,check=True,timeout=90)
physical=lambda p:[words.physical(r['sample']) for r in map(json.loads,p.read_text().splitlines()) if r['event']=='calibration.sample'];assert physical(path)==physical(out/'gravity-off0.jsonl')
r=dict(scope=__doc__.strip(),runs=reports,repeats_identical=True,unmarked_preserves_old_behavior=True,probe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),goal_complete=False);(out/'comparison.json').write_text(json.dumps(r,indent=2)+'\n');assert all(r['exact']==120 for r in reports if r['mode']=='on')
