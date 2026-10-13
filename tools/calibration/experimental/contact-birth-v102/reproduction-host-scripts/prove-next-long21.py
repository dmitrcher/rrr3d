from pathlib import Path
import json,gzip,struct
root=Path.cwd();out=root/'outputs/contact-birth-v102';bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];reports=[]
for rep in range(2):
 tag=f'slope-longitudinal-21-{rep}';n=list(map(json.loads,gzip.open(out/'native-next-long21'/f'{tag}-private.jsonl.gz','rt')));miroot=out/'mac-next-long21/slope-longitudinal';m=list(map(json.loads,(miroot/f'{tag}-solver.jsonl').open()));prep=list(map(json.loads,(miroot/f'{tag}-prep.jsonl').open()));ni=[r for r in n if r.get('function')=='spring-solve-input'];nr=[r for r in n if r.get('function')=='spring-solve-result'];mi=[r for r in m if r['phase']=='spring-before'];mr=[r for r in m if r['phase']=='spring-after'];assert len(ni)==len(nr)==60 and len(mi)==len(mr)==20
 native_order=[];bound=[]
 for row in ni[:4]:
  words=row['solve_row']['words'];matches=[r for r in prep if bits(r['spring']['mass'])==words[11]];assert len(matches)==1;p=matches[0];native_order.append(p['wheel'])
  fields={'world_axis':(p['world_axis'],words[8:11]),'jacobian':(p['spring']['jacobian'],words[12:15]),'inverse_jacobian':(p['spring']['inverse_jacobian'],words[20:23])}
  for key,(mac,native) in fields.items():assert list(map(bits,mac))==native,(rep,p['wheel'],key)
  bound.append({'wheel':p['wheel'],'effective_mass_and_axis3_jacobian3_inverse_jacobian3_words_exact':True})
 assert native_order==[3,1,0,2] and [r['wheel'] for r in mi[:4]]==[3,2,1,0]
 assert list(map(bits,mi[0]['velocity']))==ni[0]['solve_body0']['words'][:3];assert list(map(bits,mi[0]['angular']))==ni[0]['solve_body0']['words'][4:7];assert bits(mr[0]['lambda'])==nr[0]['solve_row']['words'][28]
 # The actual next Native input is the post-first-impulse state; result breakpoint itself precedes body application.
 assert list(map(bits,mr[0]['velocity']))==ni[1]['solve_body0']['words'][:3];assert list(map(bits,mr[0]['angular']))==ni[1]['solve_body0']['words'][4:7]
 reports.append({'repeat':rep,'physical_tick':21,'Native_spring_order':native_order,'Mac_spring_order':[r['wheel'] for r in mi[:4]],'per_wheel_bound_properties':bound,'first_spring_incoming_velocity3_angular3_lambda_words_exact':True,'actual_next_Native_input_matches_Mac_after_first_spring':True})
(out/'next-long21-order-lead.json').write_text(json.dumps({'scope':'Read-only fresh original Windows600step Golden-exact and MacV102600step unchanged histories. All four per-wheel current prepared masses/axes/Jacobians/inverseJacobian exact. First spring matches, second selects different wheel. Pair lifetime cause needs actual endpoint traces; no V103 source edit. Native result body is preapplication; next row input is used for actual post-first comparison.','repeats':reports,'goal_complete':False},indent=2)+'\n');print('Both repeats: four per-wheel prepared properties exact, first spring and following input exact; second wheel order differs.')
