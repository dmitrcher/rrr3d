from pathlib import Path
import os,json,subprocess,gzip,struct,sys,importlib.util
root=Path.cwd();p=root/'work/scene-composition-v169';out=root/'outputs/scene-composition-v169/focused';out.mkdir(parents=True,exist_ok=False)
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');exe=active/'build/macos-arm64-m10/Debug/rrr3d_physics_calibration';base=json.load(open('outputs/sleep-inputs-v160/final-broad/manifest.json'))['environment']
for v,name in [(163,'ACTOR_REACTIVATION'),(164,'PLANE_BROADPHASE'),(165,'PAIR_PUBLICATION'),(166,'PAIR_RETIREMENT'),(168,'LIVE_MANAGER')]:base[f'RRR3D_EXPERIMENT_NATIVE_{name}_V{v}']='1'
spec=importlib.util.spec_from_file_location('words',root/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
names=['slope-longitudinal','slope-lateral'];ref={name:[] for name in names}
for r in map(json.loads,open('outputs/calibration-v14-source-windows.jsonl')):
 if r['event']=='calibration.sample' and r['detail'] in ref:ref[r['detail']].append(r['sample'])
checks=[]
for mode in ['off','on']:
 env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env['RRR3D_EXPERIMENT_NATIVE_SCENE_COMPOSITION_V169']='1' if mode=='on' else '0';path=out/f'{mode}.jsonl'
 with path.with_suffix('.log').open('w') as log:subprocess.run([str(exe),str(path),'list:'+','.join(names)],env=env,stdout=log,stderr=log,check=True,timeout=120)
 actual={name:[] for name in names}
 for r in map(json.loads,open(path)):
  if r['event']=='calibration.sample':actual[r['detail']].append(r['sample'])
 for name in names:
  assert len(actual[name])==len(ref[name]);bad=[i for i,(a,b) in enumerate(zip(ref[name],actual[name])) if words.physical(a)!=words.physical(b)];checks.append(dict(mode=mode,name=name,samples=len(ref[name]),first_difference=bad[0] if bad else None,difference_count=len(bad)));print(checks[-1],flush=True)
  if mode=='on':assert not bad
(p/'focused-proof.json').write_text(json.dumps(dict(scope='Identical commands, dt, parameters and immutable Windows expected states. Only explicit death-plane scene composition differs. Original SDK SpringProbe never creates this plane; ordinary Map.cpp does.',checks=checks,goal_complete=False),indent=2)+'\n')
