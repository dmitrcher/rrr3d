from pathlib import Path
import json,gzip,subprocess
r=Path.cwd();w=r/'work/static-box-lever-v111';headers=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/_deps/rrr3d_jolt-src/Jolt/Physics/Constraints/ConstraintPart');fixtures=[];checks=[];zeros=[]
def eq(a,b):
 if a==b:return True
 for i,(x,y) in enumerate(zip(a,b)):
  if x==y:continue
  if x&0x7fffffff==0 and y&0x7fffffff==0:zeros.append({'component':i,'actual':x,'Native':y})
  else:return False
 return len(a)==len(b)
selections=[('wall-oblique',r/'outputs/lost-tire-v110/native-next-wall47','v110-wall-oblique-47'),('bump-crossing',r/'outputs/static-box-lever-v111/native-bump49','v111-bump-crossing-49'),('wall-air-straight-24',r/'outputs/static-box-lever-v111/native-air16','v111-wall-air-straight-24-16')]
for opt in ['O0','O2']:
 exe=w/('static-kernel-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(headers),str(w/'verify-static-kernel.cpp'),'-o',str(exe)],check=True)
 for name,folder,prefix in selections:
  for rep in range(2):
   n=list(map(json.loads,gzip.open(folder/f'{prefix}-{rep}-private.jsonl.gz','rt')));rows=[x for x in n if x.get('function')=='solver-initialize-exit'];entries=[x for x in n if x.get('function')=='solver-iteration-entry'];assert len(rows)==len(entries)==20;data=[x['native_solver_row']['words']+x['native_body0']['words']+x['native_body1']['words'] for x in rows];out=[list(map(int,l.split())) for l in subprocess.check_output([str(exe)],input='\n'.join(' '.join(map(str,x)) for x in data)+'\n',text=True).splitlines()];bodychecks=[];lambdachecks=[]
   for i,a in enumerate(out):
    if i%4!=3:
     b=entries[i+1]['native_body1']['words'];expected=b[:3]+b[4:7];assert eq(a[:6],expected),(name,rep,opt,i,'body',a[:6],expected);bodychecks.append({'row':i,'expected':expected,'actual':a[:6]})
    if i<16:
     b=rows[i+4]['native_solver_row']['words'];expected=[b[27]]+b[12:15];assert eq(a[6:10],expected),(name,rep,opt,i,'lambdas',a[6:10],expected);lambdachecks.append({'row':i,'expected':expected,'actual':a[6:10]})
   checks.append({'case':name,'repeat':rep,'optimization':opt,'within_iteration_body_transitions_exact_except_signed_zero':len(bodychecks),'next_iteration_retained_lambda_and_friction_vectors_exact_except_signed_zero':len(lambdachecks)})
   if opt=='O0':fixtures.append({'case':name,'repeat':rep,'input':data,'body_checks':bodychecks,'lambda_checks':lambdachecks})
(w/'static-kernel-fixtures.json').write_text(json.dumps(fixtures,indent=2)+'\n');(r/'outputs/static-box-lever-v111/static-kernel-proof.json').write_text(json.dumps({'scope':'Actual original Native contact preparation and body inputs, with post-solve bodies bound by the next contact entry inside each iteration. Inter-iteration transitions include wheel constraints and are deliberately not used as contact-only outputs. Retained impulses/vector values are independently bound at the same contact next iteration. Wall47,bump49,air16; two Native repeats; unchanged runtime never consumes these inputs.','checks':checks,'raw_signed_zero_differences':zeros,'raw_ABI_equality_claimed':not zeros,'goal_complete':False},indent=2)+'\n');print('Three original cases x2 repeats xO0/O2:15 within-iteration body transitions and16 retained lambda/friction vectors each exact; signed zeros',len(zeros))
