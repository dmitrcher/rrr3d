from pathlib import Path
import os,subprocess,json
p=Path('outputs/controller-torque-v173/source-extended');p.mkdir(parents=True,exist_ok=False);base=json.load(open('outputs/scene-composition-v169/final-broad/manifest.json'))['environment']
for mode in ['off','on']:
 for rep in [0,1]:
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env['RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173']='1' if mode=='on' else '0';out=p/f'{mode}-run{rep}.jsonl'
  with out.with_suffix('.log').open('w') as log:subprocess.run([str(Path('work/controller-torque-v173/SourceTorqueExtended').resolve()),str(out.resolve())],env=env,stdout=log,stderr=log,check=True,timeout=90)
  print(mode,rep,'complete',flush=True)
(p/'manifest.json').write_text(json.dumps(dict(profile=base,repeats=2,goal_complete=False),indent=2)+'\n')
