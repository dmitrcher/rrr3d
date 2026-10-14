from pathlib import Path
import json,gzip,struct,subprocess
r=Path.cwd();w=r/'work/lost-tire-v110';p=r/'outputs/lost-tire-v110';bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
def load(p):return [json.loads(line,parse_int=lambda s:-0.0 if s=='-0' else int(s)) for line in (gzip.open(p,'rt') if str(p).endswith('.gz') else p.open())]
zero=[]
def eq(a,b):
 if isinstance(a,int):a=[a];b=[b]
 if a==b:return True
 for i,(x,y) in enumerate(zip(a,b)):
  if x==y:continue
  if x&0x7fffffff==0 and y&0x7fffffff==0:zero.append({'index':i,'Native':x,'Mac':y})
  else:return False
 return len(a)==len(b)
proof=[];fixtures=[]
for rep in range(2):
 n=load(r/f'outputs/contact-bias-v109/native-next-target79/target-half-79-{rep}-private.jsonl.gz');n=[x for x in n if x.get('sample_tick_v71')==79];ni=[x for x in n if x.get('function')=='spring-solve-input'];nr=[x for x in n if x.get('function')=='spring-solve-result'];mac=load(p/f'dispatch79/lost-{rep}.jsonl');inputs=[x for x in mac if x['event'].endswith('-input')];result=[x for x in mac if x['event'].endswith('-result')];assert len(inputs)==len(result)==40;assert len(ni)==len(nr)==60;tests=[]
 for it in range(5):
  visits=[]
  for i in range(8):
   mn=inputs[it*8+i];mr=result[it*8+i];native=ni[it*12+4+i];res=nr[it*12+4+i];row=native['solve_row']['words'];body=native['solve_body0']['words'];kind='lateral' if i%2==0 else 'longitudinal';assert mn['event']=='lost-'+kind+'-input' and mn['wheel']==i//2;visits.append([mn['wheel'],kind]);m=mn[kind+'_row'];checks={'dynamic_axis':eq(row[4:7],list(map(bits,m['axis']))),'jacobian':eq(row[12:15],list(map(bits,m['jacobian']))),'inverse_jacobian':eq(row[20:23],list(map(bits,mn[kind+'_inverse_jacobian']))),'mass':eq(row[11],bits(m['mass'])),'incoming_body':eq(body[:3]+body[4:7],list(map(bits,mn['linear']+mn['angular']))),'incoming_lambda':eq(row[28],bits(m['lambda'])),'result_lambda':eq(res['solve_row']['words'][28],bits(mr[kind+'_row']['lambda']))};assert all(checks.values()),(rep,it,i,checks);tests.append(checks);fixtures.append({'input':row[4:7]+row[12:15]+body[:3]+body[4:7],'Native_jv_word':native['xmm_words'][1][0]})
  assert visits==[[i//2,'lateral' if i%2==0 else 'longitudinal'] for i in range(8)]
 proof.append({'repeat':rep,'physical_tick':79,'five_iterations_cached_tire_order_equal':True,'all_40_prepared_geometry_mass_incoming_body_and_lambdas_equal_except_signed_zero':True,'checks':tests})
header=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/_deps/rrr3d_jolt-src/Jolt/Physics/Constraints/ConstraintPart');inputs='\n'.join(' '.join(map(str,x['input'])) for x in fixtures)+'\n';expected=[x['Native_jv_word'] for x in fixtures];arith=[]
for opt in ['O0','O2']:
 exe=w/('jv-proof-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(header),str(w/'verify-jv.cpp'),'-o',str(exe)],check=True);actual=list(map(int,subprocess.check_output([str(exe)],input=inputs,text=True).splitlines()));assert actual==expected;arith.append({'optimization':opt,'actual_Native_Jv_words_exact':len(fixtures)})
(w/'jv-fixtures.json').write_text(json.dumps(fixtures,indent=2)+'\n');(p/'live79-proof.json').write_text(json.dumps({'scope':'Actual original Native vs actual live V110 retained tires at formerly divergent target-half79, all five iterations and both original/fresh Source repeats. Native dynamic-body axis is row4..6; row8..10 belongs to static body. Same-input production Jv reconstruction binds the role distinction. No axis or solver arithmetic edits needed: only cached-object dispatch eligibility changes.','proof':proof,'same_input_Jv_checks':arith,'raw_signed_zero_differences':zero,'raw_ABI_equality_claimed':not zero,'goal_complete':False},indent=2)+'\n');print('Two repeats x40 tire rows: all preparation/incoming bodies/lambdas exact except signedzero:',len(zero),'O0/O2 80 actual Native Jv words exact')
