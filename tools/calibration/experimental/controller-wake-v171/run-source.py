from pathlib import Path
import os,subprocess,json
p=Path('outputs/controller-wake-v171/source-d');p.mkdir(parents=True,exist_ok=False);base=json.load(open('outputs/scene-composition-v169/final-broad/manifest.json'))['environment']
for mode in ['off','on']:
 for rep in [0,1]:
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env['RRR3D_EXPERIMENT_GAMECAR_WAKE_V171']='1' if mode=='on' else '0';out=p/f'{mode}-run{rep}.jsonl'
  with out.with_suffix('.log').open('w') as log:subprocess.run([str(Path('work/controller-wake-v171/SourceWakeProbe').resolve()),str(out.resolve())],env=env,stdout=log,stderr=log,check=True,timeout=90)
  print(mode,rep,'complete',flush=True)
(p/'manifest.json').write_text(json.dumps(dict(profile=base,repeats=2,goal_complete=False),indent=2)+'\n')
