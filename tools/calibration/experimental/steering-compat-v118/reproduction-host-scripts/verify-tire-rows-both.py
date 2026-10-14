from pathlib import Path
import json,gzip,struct
r=Path.cwd();w=r/'work/steer-transition-v118';bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];value=lambda u:struct.unpack('<f',struct.pack('<I',u))[0]
def load(p):return list(map(json.loads,gzip.open(p,'rt')))
reports=[]
for name,folder,source_folder in [('steer-left','native','mac-final-live'),('steer-right','native-right','mac-final-live-right')]:
 for rep in [0,1]:
  n=load(r/f'outputs/steer-transition-v118/{folder}/kernel-{name}-240-{rep}-private.jsonl.gz');s=load(r/f'outputs/steer-transition-v118/{source_folder}/{name}-240-{rep}-tire.jsonl.gz');prep=load(r/f'outputs/steer-transition-v118/{source_folder}/{name}-240-{rep}-prep.jsonl.gz')
  wheels=[x for x in n if x.get('function')=='wheel-spring-prepare'];slots={}
  for wheel,x in enumerate(wheels):
   slots[x['wheel_tail']['words'][71]&0x3ffff]=(wheel,'longitudinal');slots[x['wheel_tail']['words'][72]&0x3ffff]=(wheel,'lateral')
  obj={x['ecx']:x['spring_object']['words'][7] for x in n if x.get('function')=='spring-row-entry'};by_jac={};prepared=0
  for x in n:
   if x.get('function')!='spring-row-ready' or obj[x['esi']] not in slots:continue
   wheel,kind=slots[obj[x['esi']]];row=x['constraint_row']['words'];z=prep[wheel][kind]
   assert row[11]==bits(z['mass']) and row[12:15]==list(map(bits,z['jacobian'])) and row[20:23]==list(map(bits,z['inverse_jacobian']))
   by_jac[tuple(row[12:15])]=(wheel,kind);prepared+=1
  assert prepared==8 and len(by_jac)==8
  checked=0;zero=[]
  for event,suffix in [('spring-solve-input','input'),('spring-solve-applied','result')]:
   native=[x for x in n if x.get('function')==event and tuple(x['solve_row']['words'][12:15]) in by_jac];source=[x for x in s if x['event'].endswith('-'+suffix)];assert len(native)==len(source)==40
   for index,(x,z) in enumerate(zip(native,source)):
    row=x['solve_row']['words'];wheel,kind=by_jac[tuple(row[12:15])];assert z['event']==kind+'-'+suffix,(index,wheel,kind,z['event'],'row order differs')
    assert row[11]==bits(z['mass'])
    if row[28]!=bits(z['lambda']):
     assert value(row[28])==z['lambda']==0.0,(suffix,index,wheel,kind,'Nonzero impulse differs')
     zero.append({'field':'impulse','native':row[28],'source':bits(z['lambda']),'index':index,'phase':suffix})
    # Original stores upper/lower bounds in words28/29 only before limit setup;
    # body and lambda checks bind application order without guessing bound layout.
    body=x['solve_body0']['words'];linear=[z['linear'][0],z['linear'][2],z['linear'][1]];angular=[-z['angular'][0],-z['angular'][2],-z['angular'][1]]
    for field,a,b in [('linear',body[:3],list(map(bits,linear))),('angular',body[4:7],list(map(bits,angular)))]:
     for axis,(aa,bb) in enumerate(zip(a,b)):
      if aa!=bb:
       assert value(aa)==value(bb)==0.0,(index,wheel,kind,suffix,field,axis,'nonzero body word differs')
       zero.append({'field':field,'axis':axis,'native':aa,'source':bb,'index':index,'phase':suffix})
    checked+=1
  reports.append({'case':name,'repeat':rep,'actual_tire_handle_slot_binding':True,'prepared_rows_raw_word_exact':prepared,'tire_before_after_body_and_impulse_checks_exact':checked,'actual_original_row_order_exact':True,'private_signed_zero_differences':zero})
assert reports[0]['private_signed_zero_differences']==reports[1]['private_signed_zero_differences']
assert reports[2]['private_signed_zero_differences']==reports[3]['private_signed_zero_differences']
(w/'tire-row-proof.json').write_text(json.dumps({'scope':'Eight actual original tire rows mapped by original manager handles and copied prepared Jacobians. Actor-frame mass/Jacobian/inverse-Jacobian, actual lateral-before-longitudinal order, every nonzero before/after impulse and body word match throughout five iterations. Original bound loads ECX+40/+44 independently bind actual upper/lower limits.','captures':reports,'raw_ABI_equality_claimed':False,'goal_complete':False},indent=2)+'\n');print('Actual8tire preparations and80before/after solves per repeat verified')
