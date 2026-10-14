from pathlib import Path
import json,os,subprocess,hashlib,shutil
r=Path.cwd();w=r/'work/tiny-contact-v123';f=r/'work/frozen-v123-candidate';build=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10');manifest=json.load(open(f/'manifest.json'));spring=build/'Debug/rrr3d_physics_spring_probe';shutil.copy2(spring,f/spring.name);manifest['hashes'][spring.name]=hashlib.sha256((f/spring.name).read_bytes()).hexdigest();(f/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
reports=[]
for mode in ['default','candidate']:
 env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))}
 if mode=='candidate':env.update(manifest['environment'])
 with (w/f'ctest-{mode}.log').open('wb') as log:subprocess.run(['ctest','--test-dir',str(build),'-C','Debug','--output-on-failure','-j','6'],env=env,stdout=log,stderr=log,check=True,timeout=300)
 reports.append({'mode':mode,'success':True});print('CTest',mode,'passed',flush=True)
(w/'ctest-report.json').write_text(json.dumps({'runs':reports,'goal_complete':False},indent=2)+'\n')
for script in ['run-game-smoke.py','run-game-render-race.py']:
 with (w/(script+'.log')).open('wb') as log:subprocess.run(['python3',str(w/script)],stdout=log,stderr=log,check=True,timeout=300)
 print(script,'passed',flush=True)
