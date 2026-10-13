from pathlib import Path
import json,gzip,struct,hashlib
root=Path.cwd();o=root/'outputs/box-face-v97'
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
flt=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
f32=lambda x:flt(bits(x))
fixtures=[]
for rep in range(2):
 n=list(map(json.loads,gzip.open(o/f'native-solver-frame132/v97-solver-frame132-body-tilt-roll-{rep}-private.jsonl.gz','rt')))
 m=list(map(json.loads,open(o/f'mac-next-frame132/body-tilt-roll-132-{rep}-physical.jsonl')))
 stage=next(x['sample'] for x in m if x['event']=='physics.native-material-stages');rows=[x['sample'] for x in m if x['event']=='physics.native-contact-row'];composes=[x for x in n if x.get('function')=='solver-frame0-compose-input'];prepared=[x for x in n if x.get('function')=='solver-initialize-exit'];assert len(composes)==2 and len(prepared)==10
 for i,c in enumerate(composes):
  actor=c['solver_frame_actor0_quaternion']['words'];local=c['solver_frame_local0_quaternion']['words'];assert actor==list(map(bits,stage['physical_actor']));assert local==list(map(bits,stage['callback_local0']))
  a=list(map(flt,actor));l=list(map(flt,local));prod=[f32(x*y) for x,y in zip(a,l)];old=f32(prod[3]-f32(f32(prod[0]+prod[2])+prod[1]));new=f32(prod[3]-f32(f32(prod[2]+prod[1])+prod[0]));target=prepared[i]['native_solver_row']['words'][19]
  assert bits(new)==target and bits(old)==bits(stage['solver_quaternion'][3]) and bits(old)!=target
  assert prepared[i]['native_solver_row']['words'][16:19]==list(map(bits,stage['solver_quaternion'][:3]))
  if i==0:
   b=prepared[i]['native_body0']['words'];assert b[:3]+b[4:7]==list(map(bits,rows[0]['velocity']+rows[0]['angular']));assert b[15:24]==list(map(bits,rows[0]['inverse_inertia']));assert b[11]==bits(rows[0]['inverse_mass']);assert prepared[i]['native_solver_row']['words'][4:7]==list(map(bits,rows[0]['lever_arm']))
  fixtures.append({'repeat':rep,'contact':i,'actual_actor_words':actor,'actual_local_words':local,'native_solver_words':prepared[i]['native_solver_row']['words'][16:20],'old_scalar':old,'native_scalar':flt(target),'actual_source_scalar':new,'all_eight_input_words_exact_mac':True})
report={'scope':'Read-only actual Native actor0 compose input at RVA74c1d, original180states exact Golden per repeat; two contacts times two repeats. Mac V97 unchanged.', 'fixtures':fixtures,'actual_input_words_bound':32,'scalar_order_native':'(local.z*actor.z + local.y*actor.y) + local.x*actor.x, individual float32 rounding','old_scalar_order':'(local.x*actor.x + local.z*actor.z) + local.y*actor.y','first_incoming_body_tensor_lever_inverse_mass_exact':True,'all_four_native_scalar_words_reproduced':True,'runtime_fix_installed':False,'goal_complete':False}
(o/'next132-solver-compose-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('actual inputs32 words exact; recovered scalar4 exact, old4 different')
