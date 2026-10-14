from pathlib import Path
import subprocess,json,gzip
r=Path.cwd();h='/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/_deps/rrr3d_jolt-src/Jolt/Physics/Constraints/ConstraintPart';fixtures=[]
for tick in [174,175]:
 for rep in [0,1]:
  n=list(map(json.loads,gzip.open(r/f'outputs/body-pitch-v119/native-contact-next/v119-body-tilt-pitch-{tick}-{rep}-private.jsonl.gz','rt')))
  for i,x in enumerate(v for v in n if v.get('function')=='solver-initialize-exit'):
   fixtures.append({'tick':tick,'repeat':rep,'row':i,'input':x['native_solver_row']['words']+x['native_body0']['words']+x['native_body1']['words'],'expected':x['native_solver_row']['words'][20:23]})
assert len(fixtures)==40
for opt in ['O0','O2']:
 exe=r/f'work/body-pitch-v119/verify-mass-{opt}';subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+h,'work/body-pitch-v119/verify-mass.cpp','-o',str(exe)],check=True)
 data='\n'.join(' '.join(map(str,x['input'])) for x in fixtures)+'\n';out=subprocess.check_output([str(exe)],input=data,text=True)
 for x,l in zip(fixtures,out.splitlines()):
  got=list(map(int,l.split()));assert got[1::2]==x['expected'],(x['tick'],x['row'],got[1::2],x['expected']);x['XYZ_mass']=got[::2]
  if got[::2]!=x['expected'] and x['repeat']==0 and x['row']<2:print(opt,x['tick'],x['row'],'XYZ',got[::2],'Native',x['expected'])
(r/'work/body-pitch-v119/mass-next-proof.json').write_text(json.dumps({'scope':'All40 actual Native prepared contact rows: production NativePairPreparation mass helper exact O0/O2. XYZ-only old association compared separately. Runtime not modified.','fixtures':fixtures,'goal_complete':False},indent=2)+'\n')
