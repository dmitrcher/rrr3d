import os,json,subprocess,gzip
from pathlib import Path
out=Path('outputs/live-sleep-v128/source-wake720').resolve();out.mkdir(parents=True,exist_ok=True)
m=json.load(open('outputs/live-sleep-v128/wake-reset-target/manifest.json'));env={k:v for k,v in os.environ.items() if not k.startswith('RRR3D_')};env.update(m['profile']);env['RRR3D_EXPERIMENT_NATIVE_PLACED_MESH_V126']='1';env['RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128']='1';env['RRR3D_TRACE_FILE']=str(out/'physical.jsonl')
for kind,version,step in [('PREP','V67',721),('SOLVER','V62',721)]:
 env[f'RRR3D_WHEEL_{kind}_TRACE_FILE_{version}']=str(out/(kind.lower()+'.jsonl'));env[f'RRR3D_WHEEL_{kind}_TRACE_STEP_{version}']=str(step)
with (out/'console.log').open('w') as f:subprocess.run(['/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/RRR3d.app/Contents/MacOS/RRR3d','--solo-physics-replay','--track=0','--car=marauder'],env=env,stdout=f,stderr=subprocess.STDOUT,check=True,timeout=90)
def samples(p):return [x['sample'] for x in map(json.loads,open(p)) if x['event']=='solo.sample']
assert samples(out/'physical.jsonl')==samples('outputs/live-sleep-v128/wake-reset-target/on0.jsonl')
for x in (json.loads(l.replace('nan','NaN')) for l in open(out/'prep.jsonl')):print(x['wheel'],x['world_point'],x['world_axis'],x['spring'])
