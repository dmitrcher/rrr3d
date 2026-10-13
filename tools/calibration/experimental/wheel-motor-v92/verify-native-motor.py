"""Derive motor fixtures from immutable Native observations and check actual helper."""
from pathlib import Path
import gzip,json,struct,subprocess,tempfile
base=Path(__file__).resolve().parent;bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def records(p):
 with gzip.open(p,'rt') as f:return [json.loads(s) for s in f]
ref={}
for r in records(base/'captures/windows-reference.jsonl.gz'):
 if r['event']=='calibration.sample':ref.setdefault(r['detail'],[]).append(r['sample'])
manifest=json.loads((base/'captures/native-contact-motor/manifest.json').read_text());payloads=[];live=[]
for rep in range(2):
 rowsout=[];matching=[];entries=[e for e in manifest['captures'] if e['repeat']==rep];assert len(entries)==7
 for e in entries:
  directory=base/'captures/native-contact-motor';raw=records(directory/(e['tag']+'-private.jsonl.gz'));actual=[r['sample'] for r in records(directory/(e['tag']+'-physical.jsonl.gz')) if r['event']=='calibration.sample'];assert actual==ref[e['name']] and len(actual)==600
  for phase in ['wheel','contact']:
   inputs=[r for r in raw if r.get('function')==phase+'-motor-input'];assert len(inputs)==4
   for i,r in enumerate(inputs):
    result=[z for z in raw if z.get('function')==phase+'-motor-result' and z.get('wheel_address')==r['wheel_address']];assert len(result)==1;values=[r[k]['words'][0] for k in ['axle','motor_torque']]+[r['xmm_words'][4][0],r['inverse_wheel_mass']['words'][0],result[0]['xmm_words'][0][0]];rowsout.append(' '.join(str(v) for v in values))
    if phase=='contact' and e['name']=='mobility/dirtdevil/wheel1/wheel-drive':
     old=records(base/f'captures/mac-control/tick{e["physical_tick"]}-rep{rep}-motor.jsonl.gz');m=next(v for v in old if v['wheel']==i);assert list(bits(m[k]) for k in ['axle_before','torque','delta','inverse_mass'])==values[:4];matching.append((e['physical_tick'],i,values,bits(m['after_motor'])))
     candidate=records(base/f'captures/mac-candidate/tick{e["physical_tick"]}-rep{rep}-motor.jsonl.gz');c=next(v for v in candidate if v['wheel']==i);assert list(bits(c[k]) for k in ['axle_before','torque','delta','inverse_mass'])==values[:4];assert bits(c['after_motor'])==values[4];final=next(z for z in raw if z.get('function')=='contact-motor-brake-final' and z.get('wheel_address')==r['wheel_address']);assert bits(c['after_brake'])==final['axle']['words'][0]
 payloads.append(str(len(rowsout))+'\n'+'\n'.join(rowsout)+'\n');live.append(matching)
assert payloads[0]==payloads[1]==(base/'proofs/native-contact-motor-inputs.txt').read_text();assert live[0]==live[1] and len(live[0])==12 and sum(words[4]!=old for tick,wheel,words,old in live[0])==4
with tempfile.TemporaryDirectory(prefix='motorrock-v92-motor-') as t:
 for opt in ['O0','O2']:
  exe=Path(t)/opt;subprocess.run(['clang++','-std=c++17','-'+opt,str(base/'proofs/verify-motor.cpp'),'-o',str(exe)],check=True)
  native=subprocess.run([str(exe)],input=payloads[0],text=True,capture_output=True,check=True);control=subprocess.run([str(exe),'old-division'],input=payloads[0],text=True,capture_output=True,check=True);assert native.stdout.strip()=='56 Native motor results exact';assert control.stdout.strip()=='8 old-division word differences';print(opt+': '+native.stdout.strip()+'; '+control.stdout.strip())
print('8400 Original physical states preserved; all48 live Mac input words exact Native; old outputs differ4words, candidate all12inputs/motor/brake outputs exactNative. No coefficient fitting.')
