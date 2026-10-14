from pathlib import Path
import json,gzip,struct
r=Path.cwd();bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];checks=[];zeros=[]
def equal(a,b,field):
 assert len(a)==len(b)
 for axis,(x,y) in enumerate(zip(a,b)):
  if x!=y:
   assert x&0x7fffffff==y&0x7fffffff==0,(field,axis,x,y)
   zeros.append({'field':field,'axis':axis,'Native':x,'Source':y})
for tick in [31,32]:
 for rep in [0,1]:
  n=list(map(json.loads,gzip.open(r/f'outputs/pair-pole-v120/native-kernel/kernel-pair-stock-pole-1-{tick}-{rep}-private.jsonl.gz','rt')));p=list(map(json.loads,gzip.open(r/f'outputs/pair-pole-v120/mac-candidate/pair-stock-pole-1-{tick}-{rep}-prep.jsonl.gz','rt')));s=list(map(json.loads,gzip.open(r/f'outputs/pair-pole-v120/mac-candidate/pair-stock-pole-1-{tick}-{rep}-solver.jsonl.gz','rt')))
  by_point={tuple(x['wheel_backend']['words'][116:119]):x for x in n if x.get('function')=='wheel-spring-prepare'};entry={x['ecx']:x['spring_object']['words'][7] for x in n if x.get('function')=='spring-row-entry'};prepared={entry[x['esi']]:x['constraint_row']['words'] for x in n if x.get('function')=='spring-row-ready'};by_jac={}
  for x in p:
   wheel=by_point[tuple(map(bits,x['world_point']))];slot=wheel['wheel_tail']['words'][69]&0x3ffff;row=prepared[slot];equal(row[12:15],list(map(bits,x['spring']['jacobian'])),'owner-Jac');equal(row[16:19],list(map(bits,x['contact_spring_jacobian_v120'])),'contact-Jac');equal(row[20:23],list(map(bits,x['spring']['inverse_jacobian'])),'owner-inverse-Jac');equal(row[24:27],list(map(bits,x['contact_spring_inverse_jacobian_v120'])),'contact-inverse-Jac');equal([row[11]],[bits(x['spring']['mass'])],'mass');by_jac[tuple(row[12:15])]=x['wheel']
  phasecounts={}
  for np,sp in [('spring-solve-input','spring-before'),('spring-solve-applied','spring-after')]:
   nr=[x for x in n if x.get('function')==np and tuple(x['solve_row']['words'][12:15]) in by_jac];sr=[x for x in s if x.get('phase')==sp];assert len(nr)==len(sr)==20
   for i,(a,b) in enumerate(zip(nr,sr)):
    row=a['solve_row']['words'];wheel=by_jac[tuple(row[12:15])];assert b['wheel']==wheel,(tick,rep,i,'solveorder',wheel,b['wheel']);equal([row[28]],[bits(b['lambda'])],'lambda');body=a['solve_body0']['words'];equal(body[:3]+body[4:7],list(map(bits,b['velocity']+b['angular'])),'owner-incoming/result-body')
   phasecounts[sp]=len(nr)
  checks.append({'tick':tick,'repeat':rep,'both_body_prepared_Jacobians_and_inverse_Jacobians_and_mass_exact':4,'owner_before_after_body_and_impulse_rows_exact':phasecounts,'actual_spring_row_order_exact':True})
(r/'work/pair-pole-v120/live-spring-proof.json').write_text(json.dumps({'scope':'Actual live candidate vs actual immutable original, actual contact world point and manager handle slot binding. Both prepared Jacobians/inverse Jacobians/mass and every owner before/after state/impulse match original order. Contact-body before/after velocity not yet independently traced; no raw ABI equality claim.','captures':checks,'private_signed_zero_differences':zeros,'goal_complete':False},indent=2)+'\n');print('Bothbody prepared4rows and40 owner before/after checks per capture exact x4captures')
