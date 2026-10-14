from pathlib import Path
import os,json,subprocess
out=Path('outputs/game-control-wake-v133/source-step210').resolve();out.mkdir(parents=True,exist_ok=False)
base=json.load(open('outputs/startup-import-v130/source-pair-loss-fixed/manifest.json'))['profile']
for track,tick in [(0,210)]:
 env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env.update(RRR3D_EXPERIMENT_ORIGINAL_STARTUP_V130='1',RRR3D_EXPERIMENT_NATIVE_PAIR_LOSS_WAKE_V131='1',RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128='1',RRR3D_EXPERIMENT_NATIVE_PLACED_MESH_V126='1')
 env['RRR3D_TIRE_ANCHOR_TRACE_FILE_V91']=str(out/f'track{track}-anchor.jsonl')
 env['RRR3D_TIRE_ANCHOR_TRACE_ALL_V132']='1'
 env['RRR3D_REAL_WORLD_GAMECAR_WAKE_V133']='1'
 env['RRR3D_EXPERIMENT_NATIVE_WAKE_RETAIN_ANCHOR_V132']='1'
 env['RRR3D_NATIVE_TIRE_V55_TRACE']=str(out/f'track{track}-tire.jsonl')
 env['RRR3D_NATIVE_TIRE_TRACE_STEP_V86']=str(tick+1)
 env['RRR3D_SLEEP_TRACE_FILE_V83']=str(out/f'track{track}-sleep.jsonl')
 env['RRR3D_TRACE_FILE']=str(out/f'track{track}-physical.jsonl');env['RRR3D_SLEEP_POSE_TRACE_FILE_V127']=str(out/f'track{track}-pose.jsonl')
 for kind,version in [('PREP','V67'),('SOLVER','V62')]:
  env[f'RRR3D_WHEEL_{kind}_TRACE_FILE_{version}']=str(out/f'track{track}-{kind.lower()}.jsonl');env[f'RRR3D_WHEEL_{kind}_TRACE_STEP_{version}']=str(tick+1)
 with (out/f'track{track}-console.log').open('w') as log:subprocess.run(['/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/RRR3d.app/Contents/MacOS/RRR3d','--solo-physics-replay',f'--track={track}','--car=marauder'],env=env,stdout=log,stderr=log,check=True,timeout=90)
 def samples(p):return [x['sample'] for x in map(json.loads,open(p)) if x['event']=='solo.sample']
 assert samples(out/f'track{track}-physical.jsonl')==samples(f'outputs/game-control-wake-v133/source-staged/track{track}-on0.jsonl')
 print('Source readonly phases',track,'900states unchanged',flush=True)
