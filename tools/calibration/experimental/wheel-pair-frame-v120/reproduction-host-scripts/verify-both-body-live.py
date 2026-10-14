from pathlib import Path
import json,gzip
r=Path.cwd();checks=[];zeros=[]
def equal(a,b,field):
 assert len(a)==len(b)
 for i,(x,y) in enumerate(zip(a,b)):
  if x!=y:
   assert x&0x7fffffff==y&0x7fffffff==0,(field,i,x,y)
   zeros.append({'field':field,'axis':i,'Native':x,'Source':y})
for tick in [31,32]:
 for rep in [0,1]:
  n=list(map(json.loads,gzip.open(r/f'outputs/pair-pole-v120/native-kernel/kernel-pair-stock-pole-1-{tick}-{rep}-private.jsonl.gz','rt')));s=list(map(json.loads,gzip.open(r/f'outputs/pair-pole-v120/both-body-trace/run{rep}-private.jsonl.gz','rt')))
  entries={x['ecx']:x for x in n if x.get('function')=='spring-row-entry'};wheels=[x for x in n if x.get('function')=='wheel-spring-prepare'];slots={x['wheel_tail']['words'][69]&0x3ffff for x in wheels};rows={tuple(x['constraint_row']['words'][12:15]):x for x in n if x.get('function')=='spring-row-ready' and entries[x['esi']]['spring_object']['words'][7] in slots};first=next(iter(rows.values()));pose=entries[first['esi']]['body0']['words'];key=pose[28:32]+pose[25:28];inputs=[x for x in n if x.get('function')=='spring-solve-input'];selected=[(i,x) for i,x in enumerate(inputs) if tuple(x['solve_row']['words'][12:15]) in rows];assert len(selected)==20
  si=[x['words'] for x in s if x['phase']=='input' and x['words'][:7]==key];so=[x['words'] for x in s if x['phase']=='result' and x['words'][:7]==key];assert len(si)==len(so)==20,(tick,rep,'capture selection',len(si),len(so))
  for local,((index,x),a,b) in enumerate(zip(selected,si,so)):
   row=x['solve_row']['words'];equal(row[12:15]+row[16:19],a[14:20],'both-Jac');equal(row[4:7]+row[8:11],a[20:26],'both-axes');equal([row[28],row[11]],a[38:40],'lambda/mass');equal(x['solve_body0']['words'][:3]+x['solve_body0']['words'][4:7],a[26:32],'body0-input');equal(x['solve_body1']['words'][:3]+x['solve_body1']['words'][4:7],a[32:38],'body1-input')
   nxt=inputs[index+1];assert nxt['solve_row']['words'][1:3]==row[1:3],'Next-row body roles changed';equal(nxt['solve_body0']['words'][:3]+nxt['solve_body0']['words'][4:7],b[26:32],'body0-output-next-row-bound');equal(nxt['solve_body1']['words'][:3]+nxt['solve_body1']['words'][4:7],b[32:38],'body1-output-next-row-bound')
  checks.append({'tick':tick,'repeat':rep,'actual_both_body_incoming_states_axes_Jacobians_mass_lambda_exact':20,'actual_both_body_output_states_bound_by_next_native_row_exact':20})
(r/'work/pair-pole-v120/both-body-live-proof.json').write_text(json.dumps({'scope':'Actual live both-body states before/after every spring row; original output is independently bound by the next generic-row entry with the same actual body pointers. Original rows are selected by spring manager slot, actual Source tick by raw incoming pose and center; all20 rows at each tick, two repeats. No reference states enter runtime.','captures':checks,'private_signed_zero_differences':zeros,'raw_ABI_equality_claimed':not zeros,'goal_complete':False},indent=2)+'\n');print('All80 actual spring rows: both-body inputs and next-row-bound outputs exact')
