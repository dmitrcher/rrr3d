from pathlib import Path
import json,gzip,subprocess
r=Path.cwd();w=r/'work/steer-transition-v118';fixtures=[]
for name,folder in [('steer-left','native-basis'),('steer-right','native-basis-right')]:
 for rep in [0,1]:
  rows=list(map(json.loads,gzip.open(r/f'outputs/steer-transition-v118/{folder}/basis-{name}-240-{rep}-private.jsonl.gz','rt')))
  inputs={x['ebx']:x for x in rows if x.get('function')=='steer-basis-input'};trig={x['ebx']:x for x in rows if x.get('function')=='steer-sincos-ready'};ready={x['ebx']:x for x in rows if x.get('function')=='steer-basis-ready'}
  for owner,x in inputs.items():
   base=[x['prepare_frame']['words'][31],x['xmm_words'][4][0],x['xmm_words'][7][0]];long=x['prepare_frame']['words'][3:6];angle=x['xmm_words'][0][0];y=ready[owner];t=trig[owner]
   fixtures.append({'case':name,'repeat':rep,'input':[angle]+base+long,'expected':y['basis_wheel']['words'][122:128]+[t['prepare_frame']['words'][16],t['prepare_frame']['words'][30]]})
assert len(fixtures)==8
include='/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics';checks=[]
for opt in ['O0','O2']:
 exe=w/('verify-basis-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-I'+include,str(w/'verify-basis.cpp'),'-o',str(exe)],check=True)
 text='\n'.join(' '.join(map(str,x['input'])) for x in fixtures)+'\n';actual=[list(map(int,line.split())) for line in subprocess.check_output([str(exe)],input=text,text=True).splitlines()];assert actual==[x['expected'] for x in fixtures]
 checks.append({'optimization':opt,'actual_Native_steered_basis_and_sincos_results_exact':len(actual)})
(w/'basis-proof.json').write_text(json.dumps({'scope':'Production basis rotation on measured original projected unit base direction, using original sin/cos float stores. Constructed pose carries that measured unit direction; this checks rotation arithmetic, while actual actor/shape composition is verified separately by complete live prepared-row comparison.','fixtures':fixtures,'checks':checks,'goal_complete':False},indent=2)+'\n');print(checks)
