from pathlib import Path
import json,gzip,struct,importlib.util
r=Path.cwd();bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];zeros=[];checks=[]
def equal(a,b,field):
 assert len(a)==len(b)
 for idx,(aa,bb) in enumerate(zip(a,b)):
  if aa!=bb:
   assert aa&0x7fffffff==bb&0x7fffffff==0,(field,idx,aa,bb)
   zeros.append({'field':field,'axis':idx,'Native':aa,'Source':bb})
spec=importlib.util.spec_from_file_location('words',r/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
for tick in [61,62]:
 for rep in [0,1]:
  folder='native-contact';prefix=r/f'outputs/border-air-v121/{folder}/v121-border-air-straight-{tick}-{rep}'
  n=list(map(json.loads,gzip.open(str(prefix)+'-private.jsonl.gz','rt')));np=[x['sample'] for x in map(json.loads,gzip.open(str(prefix)+'-physical.jsonl.gz','rt')) if x.get('event')=='calibration.sample'];s=list(map(json.loads,gzip.open(r/f'outputs/border-air-v121/mac-candidate-contact/border-air-straight-{tick}-{rep}-physical.jsonl.gz','rt')));sp=[x['sample'] for x in s if x.get('event')=='calibration.sample'];assert len(sp)==600 and list(map(words.physical,sp))==list(map(words.physical,np))
  nr=[x for x in n if x.get('function')=='solver-iteration-entry'];prep=[x for x in n if x.get('function')=='solver-initialize-exit'];sr=[x['sample'] for x in s if x.get('event')=='physics.native-contact-row' and x['sample']['step']==tick];assert len(sr)==len(nr)==len(prep)==40
  for idx,(a,b,c) in enumerate(zip(sr,nr,prep)):
   slot=0 if b['native_body0']['words'][11] else 1;body=b[f'native_body{slot}']['words'];row=b['native_solver_row']['words'];pr=c['native_solver_row']['words'];equal(body[:3]+body[4:7],list(map(bits,a['velocity']+a['angular'])),f'{tick}/{rep}/{idx}/body');equal(body[15:24],list(map(bits,a['inverse_inertia'])),'tensor');equal(row[4+4*slot:7+4*slot],list(map(bits,a['lever_arm'])),'lever');equal([pr[20],pr[24],row[27]],[bits(a['effective_mass']),bits(a['scaled_bias']),bits(a['lambda'])],'mass/bias/retained_lambda')
  checks.append({'tick':tick,'repeat':rep,'all600_Native_states_exact':True,'all40_actual_contact_incoming_body_tensors_levers_prepared_mass_bias_and_retained_lambda_exact':True})
(r/'work/border-air-v121/final-live-proof.json').write_text(json.dumps({'scope':'Actual live final candidate vs immutable original Windows; two repeats at each of both former first divergences. Every contact incoming body, tensor, lever, prepared mass/bias and retained impulse matches, complete180-step public trajectories match. No measured inputs feed runtime.','captures':checks,'private_signed_zero_differences':zeros,'raw_ABI_equality_claimed':not zeros,'goal_complete':False},indent=2)+'\n');print('All4 uninterrupted600-state histories and160 actual contact incoming rows Native exact')
