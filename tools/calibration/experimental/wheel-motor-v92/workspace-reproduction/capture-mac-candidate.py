from pathlib import Path
import json,os,subprocess,gzip,struct,hashlib
root=Path.cwd();out=root/'outputs/wheel-motor-v92/mac-candidate';out.mkdir(parents=True,exist_ok=False);probe=root/'work/frozen-v92/rrr3d_physics_calibration';name='mobility/dirtdevil/wheel1/wheel-drive';expected=[]
with (root/'outputs/wheel-motor-v92/focused/motor-0.jsonl').open() as f:
 for r in map(json.loads,f):
  if r['event']=='calibration.sample' and r['detail']==name:expected.append(r['sample'])
assert len(expected)==600;captures=[];bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
for caseindex,tick in enumerate([119,120,121]):
 traces=[]
 for rep in range(2):
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'work/frozen-v92/manifest.json').read_text())['environment']);tag=f'tick{tick}-rep{rep}';p=out/(tag+'-physical.jsonl');trace=out/(tag+'-motor.jsonl');env['RRR3D_WHEEL_MOTOR_TRACE_FILE_V92']=str(trace);env['RRR3D_WHEEL_MOTOR_TRACE_STEP_V92']=str(tick)
  with (out/(tag+'.log')).open('wb') as log:subprocess.run([str(probe),str(p),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=180)
  actual=[r['sample'] for r in map(json.loads,p.open()) if r['event']=='calibration.sample'];assert actual==expected,'Read-only trace changed V92 history';rows=[json.loads(s) for s in trace.open()];assert len(rows)==4 and all(r['physical_tick']==tick for r in rows);traces.append(rows)
  native=[json.loads(s) for s in gzip.open(root/f'outputs/wheel-motor-v92/native-contact-motor/case{caseindex}-tick{tick}-rep{rep}-private.jsonl.gz','rt')];inputs=[r for r in native if r.get('function')=='contact-motor-input']
  for i,n in enumerate(inputs):
   m=next(r for r in rows if r['wheel']==i);result=next(r for r in native if r.get('function')=='contact-motor-result' and r.get('wheel_address')==n['wheel_address']);final=next(r for r in native if r.get('function')=='contact-motor-brake-final' and r.get('wheel_address')==n['wheel_address']);nw=[n[k]['words'][0] for k in ['axle','motor_torque']]+[n['xmm_words'][4][0],n['inverse_wheel_mass']['words'][0]];assert [bits(m[k]) for k in ['axle_before','torque','delta','inverse_mass']]==nw;assert bits(m['after_motor'])==result['xmm_words'][0][0];assert bits(m['after_brake'])==final['axle']['words'][0]
  captures.append({'tick':tick,'repeat':rep,'physical600_states_exact_v92':True,'actual_native4motors_input_output_exact':True});print(tag,'actual Native4motor inputs/results exact',flush=True)
 assert traces[0]==traces[1]
(out/'manifest.json').write_text(json.dumps({'scope':'Read-only actual Mac candidate motor inputs/output. Physical history exact V92 and actual Native inputs/results match all12motors twice.','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'captures':captures,'goal_complete':False},indent=2)+'\n')
