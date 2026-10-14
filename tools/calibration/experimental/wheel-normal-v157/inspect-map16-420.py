from pathlib import Path
import json,gzip,struct
b=lambda f:struct.unpack('<I',struct.pack('<f',f))[0];v=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
r=list(map(json.loads,gzip.open('outputs/contact-reference-v156/native-wheel-firsts/track16-run0-private.jsonl.gz','rt')));n=[x for x in r if x.get('function')=='spring-solve-input'];s=list(map(json.loads,open('outputs/retained-friction-v154/source-track16-step420/track16-solver.jsonl')));sp=[x for x in s if x['phase']=='spring-before'];ti=[x for x in map(json.loads,open('outputs/retained-friction-v154/source-track16-step420/track16-tire.jsonl')) if x['event'].endswith('-input')];spidx=tiidx=0;fail=[]
for index,a in enumerate(n):
 w=a['solve_row']['words'];expected=a['solve_body0']['words'];expected=expected[:3]+expected[4:7]
 if v(w[6])>.9:c=sp[spidx];spidx+=1;actual=list(map(b,c['velocity']+c['angular']))
 else:
  c=ti[tiidx];tiidx+=1;actual=list(map(b,[c['linear'][0],c['linear'][2],c['linear'][1],-c['angular'][0],-c['angular'][2],-c['angular'][1]]))
 if actual!=expected:fail.append({'row':index,'expected':expected,'actual':actual})
print('Native rows',len(n),'Source spring/tire',spidx,tiidx,'failures',len(fail),fail[:3]);Path('work/wheel-normal-v157/map16-420-solver-differences.json').write_text(json.dumps(dict(rows=len(n),differences=fail,goal_complete=False),indent=2)+'\n');assert len(n)==60 and not fail
prep=[json.loads(l.replace('nan','NaN')) for l in open('outputs/retained-friction-v154/source-track16-step420/track16-prep.jsonl')];byjac={tuple(map(b,x['spring']['jacobian'])):x for x in prep};rows=[x for x in n[:4]];order=[]
for a in rows:
 w=a['solve_row']['words'];m=byjac[tuple(w[12:15])];assert w[11]==b(m['spring']['mass']);assert w[20:23]==list(map(b,m['spring']['inverse_jacobian']));order.append(m['wheel'])
assert order==[c['wheel'] for c in sp[:4]]
report={'scope':'Readonly actual Native original314 vs Source geometry-derived general scene candidate. Prepared spring rows bound semantically by Jacobian, not index.','exact_intermediate_solver_body_inputs':60,'exact_prepared_spring_masses_jacobians_inverse_jacobians':4,'native_source_spring_order':order,'queue_order_all_other_steps_proven':False,'goal_complete':False};Path('work/wheel-normal-v157/map16-420-solver-proof.json').write_text(json.dumps(report,indent=2)+'\n');print(report)
