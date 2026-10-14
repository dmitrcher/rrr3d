from pathlib import Path
import json,os,subprocess
r=Path.cwd();w=r/'work/static-pair-order-v108';active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');samples=[]
for row in map(json.loads,open(r/'outputs/calibration-v14-source-windows.jsonl')):
 if row.get('event')=='calibration.sample' and row['detail']=='curb-side':samples.append(row['sample'])
rows=[]
for tick in range(225):
 pose=([0,0,12,0,0,0,1] if tick==0 else samples[tick-1]['position']+samples[tick-1]['rotation']);rows.append([tick]+pose+[int(w['contact']) for w in samples[tick]['wheels']])
inputs='\n'.join(' '.join(map(str,row)) for row in rows)+'\n';env=dict(os.environ,RRR3D_EXPERIMENT_NATIVE_INITIAL_MAX_SCAN_V102='1')
results=[]
for opt in ['O0','O2']:
 exe=w/('shadow-scene-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(active/'src/Rock3dEngine/header/physics'),str(w/'shadow-scene.cpp'),'-o',str(exe)],check=True);out=subprocess.check_output([str(exe)],input=inputs,text=True,env=env);(w/(opt+'-ranks.txt')).write_text(out);data=[list(map(int,row.split())) for row in out.splitlines()];results.append(data)
 for tick in [13,19,20,223,224]:
  row=data[tick];ranks=row[3::3];visits=sorted([i for i,x in enumerate(ranks) if x],key=lambda i:ranks[i]);print(opt,tick,'ranks',ranks,'visits',visits,flush=True)
assert results[0]==results[1];(w/'shadow-input.txt').write_text(inputs)
