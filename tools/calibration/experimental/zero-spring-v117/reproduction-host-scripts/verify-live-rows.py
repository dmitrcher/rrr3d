from pathlib import Path
import json,gzip,struct
r=Path.cwd();out=r/'outputs/zero-spring-v117';bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];value=lambda u:struct.unpack('<f',struct.pack('<I',u))[0]
def load(p):return list(map(json.loads,gzip.open(p,'rt')))
reports=[]
for rep in range(2):
 native=load(out/f'native/kernel-body-wheel-contact-x-0-{rep}-private.jsonl.gz');prep=load(out/f'mac-candidate/body-wheel-contact-x-0-{rep}-prep.jsonl.gz');solver=load(out/f'mac-candidate/body-wheel-contact-x-0-{rep}-solver.jsonl.gz')
 creation=[x for x in native if x.get('function')=='wheel-spring-created'];assert len(creation)==4
 # Original manager handles use an 18-bit object index, as in the getter resolver.
 owner_by_slot={x['spring_handle']&0x3ffff:i for i,x in enumerate(creation)};assert len(owner_by_slot)==4
 objects={x['ecx']:x['spring_object']['words'][7] for x in native if x.get('function')=='spring-row-entry'}
 prepared=[];preparation_zero_differences=[]
 for x in native:
  if x.get('function')!='spring-row-ready' or objects[x['esi']] not in owner_by_slot:continue
  row=x['constraint_row']['words'];wheel=owner_by_slot[objects[x['esi']]];source=next(y for y in prep if y['wheel']==wheel)['spring'];assert source['active']
  assert row[11]==bits(source['mass'])
  for field,original,current in [('jacobian',row[12:15],list(map(bits,source['jacobian']))),('inverse_jacobian',row[20:23],list(map(bits,source['inverse_jacobian'])))]:
   for axis,(a,b) in enumerate(zip(original,current)):
    if a!=b:
     assert value(a)==value(b)==0.0,(wheel,field,axis,'Nonzero preparation word differs')
     preparation_zero_differences.append({'wheel':wheel,'field':field,'axis':axis,'native':a,'source':b})
  prepared.append({'wheel':wheel,'joint_slot':objects[x['esi']],'mass':row[11],'jacobian':row[12:15],'inverse_jacobian':row[20:23]})
 assert len(prepared)==4
 owner_by_jacobian={tuple(x['jacobian']):x['wheel'] for x in prepared};assert len(owner_by_jacobian)==4
 before=[x for x in native if x.get('function')=='spring-solve-input' and tuple(x['solve_row']['words'][12:15]) in owner_by_jacobian]
 after=[x for x in native if x.get('function')=='spring-solve-applied' and tuple(x['solve_row']['words'][12:15]) in owner_by_jacobian];assert len(before)==len(after)==20
 zero_differences=[]
 for phase,rows in [('spring-before',before),('spring-after',after)]:
  occurrence={}
  for x in rows:
   row=x['solve_row']['words'];wheel=owner_by_jacobian[tuple(row[12:15])];iteration=occurrence.get(wheel,0)+1;occurrence[wheel]=iteration
   source=next(y for y in solver if y.get('phase')==phase and y['wheel']==wheel and y['iteration']==iteration)
   assert row[11]==bits(source['mass']) and row[28]==bits(source['lambda']),(phase,wheel,iteration,'coefficient/impulse differs')
   body=x['solve_body0']['words'];native_velocity=body[0:3];native_angular=body[4:7]
   for field,original,current in [('velocity',native_velocity,list(map(bits,source['velocity']))),('angular',native_angular,list(map(bits,source['angular'])) )]:
    for axis,(a,b) in enumerate(zip(original,current)):
     if a!=b:
      assert value(a)==value(b)==0.0,(phase,wheel,iteration,field,axis,'body state differs')
      zero_differences.append({'phase':phase,'wheel':wheel,'iteration':iteration,'field':field,'axis':axis,'native':a,'source':b})
 reports.append({'repeat':rep,'actual_handle_slot_binding':True,'prepared_rows_nonzero_words_exact':4,'prepared_mass_words_exact':4,'preparation_private_signed_zero_differences':preparation_zero_differences,'solve_before_after_input_and_impulse_checks':40,'all_nonzero_body_words_exact':True,'private_signed_zero_differences':zero_differences})
assert reports[0]['private_signed_zero_differences']==reports[1]['private_signed_zero_differences']
report={'scope':'Original object handle slots bind wheel identity directly; prepared Jacobians bind copied solver rows. Actual prepared mass and before/after impulse words match Source; every nonzero Jacobian, inverse-Jacobian, incoming and outgoing body velocity word matches. Private signed-zero differences are explicit. No arbitrary wheel permutation or reference injection.','captures':reports,'raw_ABI_equality_claimed':False,'goal_complete':False}
(r/'work/zero-spring-v117/live-row-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Actual4row preparations and40before/after solves per repeat verified; private zero differences:',len(reports[0]['private_signed_zero_differences']))
