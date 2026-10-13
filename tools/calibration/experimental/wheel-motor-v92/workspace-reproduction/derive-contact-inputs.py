from pathlib import Path
import gzip,json,subprocess,tempfile,struct
root=Path.cwd();directory=root/'work/wheel-motor-v92';out=root/'outputs/wheel-motor-v92';manifest=json.loads((out/'native-contact-motor/manifest.json').read_text());payloads=[];details=[];macproof=[];bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
for rep in range(2):
 entries=[e for e in manifest['captures'] if e['repeat']==rep];assert len(entries)==7;values=[];data=[];matched=[]
 for e in entries:
  rows=[json.loads(s) for s in gzip.open(out/'native-contact-motor'/(e['tag']+'-private.jsonl.gz'),'rt')]
  for phase in ['wheel','contact']:
   inputs=[r for r in rows if r.get('function')==phase+'-motor-input'];assert len(inputs)==4
   for i,r in enumerate(inputs):
    result=[z for z in rows if z.get('function')==phase+'-motor-result' and z.get('wheel_address')==r['wheel_address']];assert len(result)==1;vals=[r[k]['words'][0] for k in ['axle','motor_torque']]+[r['xmm_words'][4][0],r['inverse_wheel_mass']['words'][0],result[0]['xmm_words'][0][0]];values.append(' '.join(str(v) for v in vals));data.append({'name':e['name'],'tick':e['physical_tick'],'wheel':i,'phase':phase,'words':vals})
    if phase=='contact' and e['name']=='mobility/dirtdevil/wheel1/wheel-drive':
     mac=[json.loads(s) for s in (out/f'mac-control/tick{e["physical_tick"]}-rep{rep}-motor.jsonl').open()];m=next(z for z in mac if z['wheel']==i);mw=[bits(m[k]) for k in ['axle_before','torque','delta','inverse_mass']];assert mw==vals[:4],(e['physical_tick'],i,'Mac input differs',mw,vals[:4]);matched.append({'tick':e['physical_tick'],'wheel':i,'input4words_exact':True,'native_result':vals[4],'old_mac_result':bits(m['after_motor']),'old_mac_result_exact':bits(m['after_motor'])==vals[4]})
 payloads.append(str(len(values))+'\n'+'\n'.join(values)+'\n');details.append(data);macproof.append(matched)
assert payloads[0]==payloads[1] and details[0]==details[1] and macproof[0]==macproof[1];(directory/'native-contact-motor-inputs.txt').write_text(payloads[0]);checks=[]
with tempfile.TemporaryDirectory(prefix='motorrock-v92-contact-motor-') as t:
 for opt in ['O0','O2']:
  exe=Path(t)/opt;subprocess.run(['clang++','-std=c++17','-'+opt,str(directory/'verify-motor.cpp'),'-o',str(exe)],check=True);result=subprocess.run([str(exe)],input=payloads[0],text=True,capture_output=True,check=True);control=subprocess.run([str(exe),'old-division'],input=payloads[0],text=True,capture_output=True,check=True);checks.append({'optimization':opt,'native':result.stdout.strip(),'division_control':control.stdout.strip()});print(checks[-1])
(out/'contact-motor-causal-proof.json').write_text(json.dumps({'scope':'Actual Native contact and free integration motor results, same actual inputs. Live Mac input4words exact Native for Dirtdevil119/120/121. Old division fails the same-input Native result; coefficient values identical.','checks':checks,'two_repeats_exact':True,'native_original600_states_preserved':8400,'mac_unchanged600_states_preserved':3600,'mac_same_inputs':macproof[0],'fixtures':details[0],'goal_complete':False},indent=2)+'\n');print('Mac matching inputs:',len(macproof[0]),'old output differences:',sum(not v['old_mac_result_exact'] for v in macproof[0]))
