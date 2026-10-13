from pathlib import Path
import gzip,json,subprocess,tempfile
root=Path.cwd();directory=root/'work/wheel-motor-v92';out=root/'outputs/wheel-motor-v92';manifest=json.loads((out/'native-motor/manifest.json').read_text());payloads=[];details=[]
for rep in range(2):
 entries=[e for e in manifest['captures'] if e['repeat']==rep];assert len(entries)==7;values=[];data=[]
 for e in entries:
  rows=[json.loads(s) for s in gzip.open(out/'native-motor'/(e['tag']+'-private.jsonl.gz'),'rt')];inputs=[r for r in rows if r.get('function')=='wheel-motor-input'];assert len(inputs)==4
  for i,r in enumerate(inputs):
   result=[z for z in rows if z.get('function')=='wheel-motor-result' and z.get('esi')==r['esi']];assert len(result)==1;vals=[r[k]['words'][0] for k in ['axle','motor_torque']]+[r['xmm_words'][4][0],r['inverse_wheel_mass']['words'][0],result[0]['xmm_words'][0][0]];values.append(' '.join(str(v) for v in vals));data.append({'name':e['name'],'tick':e['physical_tick'],'wheel':i,'words':vals})
 payloads.append(str(len(values))+'\n'+'\n'.join(values)+'\n');details.append(data)
assert payloads[0]==payloads[1] and details[0]==details[1];(directory/'native-motor-inputs.txt').write_text(payloads[0]);checks=[]
with tempfile.TemporaryDirectory(prefix='motorrock-v92-motor-') as t:
 for opt in ['O0','O2']:
  exe=Path(t)/opt;subprocess.run(['clang++','-std=c++17','-'+opt,str(directory/'verify-motor.cpp'),'-o',str(exe)],check=True);result=subprocess.run([str(exe)],input=payloads[0],text=True,capture_output=True,check=True);control=subprocess.run([str(exe),'old-division'],input=payloads[0],text=True,capture_output=True,check=True);checks.append({'optimization':opt,'native':result.stdout.strip(),'division_control':control.stdout.strip()});print(checks[-1])
(out/'native-same-input-proof.json').write_text(json.dumps({'scope':'Same actual Native motor inputs; does not yet certify Mac input phase or public axle API calculation.','checks':checks,'two_repeats_exact':True,'native_original600_states_preserved':8400,'fixtures':details[0],'goal_complete':False},indent=2)+'\n')
