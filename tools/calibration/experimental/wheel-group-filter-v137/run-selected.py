import os,json,subprocess,hashlib
from pathlib import Path
root=Path.cwd(); out=root/'outputs/group-filter-v137/selected-maps';out.mkdir(parents=True,exist_ok=True)
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d')
exe=active/'build/macos-arm64-m10/Debug/RRR3d.app/Contents/MacOS/RRR3d'
assert b'RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128' in exe.read_bytes(), 'Candidate binary not built'
base=json.load(open(root/'outputs/real-map-v124/mac-actuators/manifest.json'))['environment']
profile={k:v for k,v in base.items() if k.startswith('RRR3D_EXPERIMENT') or k in ('NX_SPRING_ITERATIONS','RRR3D_CALIBRATION_PROFILE','RRR3D_SEED','RRR3D_REAL_WORLD_ACTUATORS_V124')}
profile['RRR3D_REAL_WORLD_ACTUATORS_V124']='1'
profile['RRR3D_CALIBRATION_ISOLATE_NEUTRAL_STABILIZATION']='1'
results=[]
for track in [0,16,48,64]:
 for mode in ('on0','on1'):
  log=out/(f'track{track}-'+mode+'.jsonl');assert not log.exists()
  env={k:v for k,v in os.environ.items() if not k.startswith('RRR3D_')};env.update(profile)
  env['RRR3D_EXPERIMENT_ORIGINAL_STARTUP_V130']='1'
  env['RRR3D_EXPERIMENT_NATIVE_PAIR_LOSS_WAKE_V131']='1'
  env['RRR3D_EXPERIMENT_NATIVE_WAKE_RETAIN_ANCHOR_V132']='1'
  env['RRR3D_REAL_WORLD_GAMECAR_WAKE_V133']='1'
  env['RRR3D_EXPERIMENT_NATIVE_SCENE_DISPATCH_V134']='1'
  env['RRR3D_EXPERIMENT_NATIVE_GROUP_FILTER_V137']='1'
  env.update(RRR3D_EXPERIMENT_NATIVE_ENERGY_SLEEP_V128='0' if mode=='off' else '1',RRR3D_ENERGY_SLEEP_TRACE_FILE_V128=str(log.with_suffix('.energy.jsonl')))
  env.update(RRR3D_TRACE_FILE=str(log),RRR3D_WORLD_FIXTURE_FILE_V124=str(log.with_suffix('.words')),RRR3D_AI_DRIVER='1',RRR3D_EXPERIMENT_NATIVE_PLACED_MESH_V126='0' if mode=='off' else '1')
  with log.with_suffix('.log').open('w') as console:
   p=subprocess.run([str(exe),'--solo-physics-replay',f'--track={track}','--car=marauder'],env=env,stdout=console,stderr=subprocess.STDOUT,timeout=120)
  assert p.returncode==0,(mode,p.returncode)
  rows=list(map(json.loads,log.read_text().splitlines()));samples=[x['sample'] for x in rows if x['event']=='solo.sample'];assert len(samples)==900
  semantic=[{'event':x['event'],'sample':x['sample']} for x in rows if x['event'] in ('solo.sample','solo.drive')]
  digest=hashlib.sha256(json.dumps(semantic,sort_keys=True).encode()).hexdigest()
  results.append(dict(track=track,mode=mode,samples=900,semantic_sha256=digest))
  print(mode,900,digest,flush=True)
(out/'manifest.json').write_text(json.dumps(dict(executable_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),profile=profile,runs=results),indent=2)+'\n')
