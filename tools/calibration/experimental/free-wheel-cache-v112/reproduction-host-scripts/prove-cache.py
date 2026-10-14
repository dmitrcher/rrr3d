from pathlib import Path
import json,gzip,struct,subprocess,tempfile
r=Path.cwd();w=r/'work/free-wheel-v112';out=r/'outputs/free-wheel-v112'
native=json.loads((out/'native-entry/manifest.json').read_text());fixtures=[]
for rep in [0,1]:
 rows=[]
 for e in native['captures']:
  if e['repeat']!=rep:continue
  data=list(map(json.loads,gzip.open(out/'native-entry'/(e['tag']+'-private.jsonl.gz'),'rt')))
  for x in data:
   if x.get('function')!='free-wheel-entry-v112':continue
   t=x['wheel_tail']['words'];nextrow=next(v for v in data if v.get('function')=='wheel-motor-input' and v['wheel_address']==x['wheel_address'])
   # Original sentinel is FLT_MAX; backend +2c8 is longitudinal
   # tire stiffness, independently identified by the serialized descriptor.
   rows.append({'tick':e['physical_tick'],'wheel':len([v for v in rows if v['tick']==e['physical_tick']]),
    'input':[t[7],t[64],t[40],t[50],int(t[9]!=0x7f7fffff),t[58]],
    'expected':nextrow['axle']['words'][0]})
 assert len(rows)==12;fixtures.append(rows)
assert fixtures[0]==fixtures[1];(w/'cache-fixtures.json').write_text(json.dumps(fixtures[0],indent=2)+'\n')
checks=[]
header=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics')
payload='\n'.join(' '.join(map(str,x['input'])) for x in fixtures[0])+'\n'
with tempfile.TemporaryDirectory(prefix='motorrock-cache-v112-') as tmp:
 for opt in ['O0','O2']:
  exe=Path(tmp)/opt;subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(header),str(w/'verify-cache.cpp'),'-o',str(exe)],check=True)
  actual=list(map(int,subprocess.check_output([str(exe)],input=payload,text=True).split()));assert actual==[x['expected'] for x in fixtures[0]]
  checks.append({'optimization':opt,'actual_original_branch_results_exact':len(actual),'Native_repeats':2})
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
live=[]
for rep in [0,1]:
 for tick in [507,508,509]:
  data=list(map(json.loads,(out/'mac-final'/f'tick{tick}-rep{rep}-motor.jsonl').open()))
  for x in data:
   if x['current_contact']:continue
   native=next(v for v in fixtures[rep] if v['tick']==tick and v['wheel']==x['wheel'])
   actual=[bits(x[k]) for k in ['axle_prior','cached_forward','radius','stiffness']]+[int(x['previous_contact']),x['flags']]
   assert actual==native['input'],(rep,tick,x['wheel'],'incoming',actual,native)
   assert bits(x['axle_before'])==native['expected']
   live.append({'tick':tick,'wheel':x['wheel'],'repeat':rep,'all6_input_fields_exact':True,'production_branch_result_exact':True})
report={'scope':'Actual original free-wheel entry and after-reset checkpoints on identical inputs; Source retained ray state derived independently, no Native values supplied to runtime. +2c8 is serialized longitudinal tire stiffness, not suspension force. FLT_MAX marks invalid cached ray.',
 'checks':checks,'fixtures':fixtures[0],'live_same_input_checks':live,'actual_Native_original600_histories_preserved':3600,
 'live_Source_full600_Native_equivalent_histories':3600,'goal_complete':False}
(out/'same-input-cache-proof.json').write_text(json.dumps(report,indent=2)+'\n');print(checks,'live inputs/results',len(live))
