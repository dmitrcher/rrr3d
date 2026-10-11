from pathlib import Path
import json,hashlib,struct,sys
import argparse
p=argparse.ArgumentParser();p.add_argument('--measurements',type=Path,required=True);p.add_argument('--contracts',type=Path,required=True);a=p.parse_args();out=a.measurements
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
f=lambda x:struct.unpack('f',struct.pack('f',x))[0]
def words(r,k):return list(struct.unpack('32f',struct.pack('32I',*r[k]['words'])))
contracts=json.loads(a.contracts.read_text());ticks={c['case']:c['first_tick'] for c in contracts};native={};prepared={};baseline=compare.load(out/'reference-row-iterations4-run0-v30-windows.jsonl','windows-physx-d3d9')
for c in contracts:
 r=list(map(json.loads,(out/f'reference-native-kernel-{c["case"]}-geometry-run0-v31-windows.jsonl').read_text().splitlines()));it=[x for x in r if x.get('function')=='solver-iteration-entry'][:40]
 prepared[c['case']]=[words(x,'native_solver_row') for x in r if x.get('function')=='solver-initialize-exit'][:8]
 native[c['case']]=[(words(x,'native_body0')[:3],words(x,'native_body0')[4:7],words(x,'native_solver_row')[27]) for x in it]
def extract(path):
 digest=hashlib.sha256();rows={};state={};legacy={};current=None
 for r in map(json.loads,path.read_text().splitlines()):
  digest.update(json.dumps([r['event'],r['detail'],r.get('sample')],sort_keys=True,separators=(',',':')).encode());digest.update(b'\n')
  if r['event']=='calibration.begin':current=r['detail']
  if r['event']=='physics.native-contact-row':rows.setdefault(current,[]).append(r['sample'])
  if r['event']=='physics.source-normal-row' and r['sample']['step']==ticks[current] and r['sample']['phase']=='biased-final':legacy.setdefault(current,[]).append(r['sample'])
  if r['event']=='calibration.sample':state.setdefault(current,[]).append(r['sample'])
 return digest.hexdigest(),rows,state,legacy
result=[]
for mask in range(32):
 paths=[out/f'calibration-native-arithmetic-errors-mask{mask}-run{r}-v32-mac.jsonl' for r in range(2)];captures=[extract(p) for p in paths];assert captures[0]==captures[1],('Semantic repeat differs',mask)
 _,rows,state,legacy=captures[0];cases=[]
 for c in contracts:
  name=c['case'];expected=native[name];matches=0
  if mask:
   assert len(rows[name])==40
   for j,(r,n) in enumerate(zip(rows[name],expected)):
    assert r['step']==c['first_tick'] and r['index']==j%8 and r['iteration']==j//8 and r['mask']==mask and r['diagnostic_error']
    got=(list(map(f,r['velocity'])),list(map(f,r['angular'])),f(r['lambda']));matches+=got==n
  else:assert not rows
  sources=rows[name][:8] if mask else legacy[name][:8];assert len(sources)==8
  masses=sum(f(x['effective_mass'])==y[20] for x,y in zip(sources,prepared[name]));biases=sum(f(x['scaled_bias'])==y[24] for x,y in zip(sources,prepared[name])) if mask&2 else None
  if mask&1:assert masses==8
  if mask&3==3:assert biases==8
  tick=c['first_tick'];cases.append({'case':name,'native_entry_states_exact':matches if mask else None,'effective_masses_exact':masses,'applied_native_scaled_biases_exact':biases,'first_velocity_exact':state[name][tick]['velocity']==baseline[name]['rows'][tick]['velocity'],'first_position_exact':state[name][tick]['position']==baseline[name]['rows'][tick]['position'],'pre_contact_states_exact':state[name][:tick]==baseline[name]['rows'][:tick]})
 row={'mask':mask,'effective_masses_exact':sum(x['effective_masses_exact'] for x in cases),'applied_native_scaled_biases_exact':sum(x['applied_native_scaled_biases_exact'] or 0 for x in cases) if mask&2 else None,'semantic_repeat_exact':True,'native_entry_states_exact':sum(x['native_entry_states_exact'] or 0 for x in cases) if mask else None,'first_velocities_exact':sum(x['first_velocity_exact'] for x in cases),'first_positions_exact':sum(x['first_position_exact'] for x in cases),'cases':cases,'files':[{'name':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in paths]}
 result.append(row);print(mask,row['native_entry_states_exact'],row['first_velocities_exact'],row['first_positions_exact'],flush=True)
assert result[-1]['native_entry_states_exact']==480 and result[-1]['first_velocities_exact']==result[-1]['first_positions_exact']==12
report={'scope':'Actual Mac arithmetic factorial with recorded Native geometry and explicitly recorded Native constraint errors.32 masks,12 cases,2 repeats,120 physical steps each. Source scalar error only influences native bias when bit2 is enabled. The scaled_bias trace field is a Native candidate, not the applied bias when bit2 is off. Mask0 control follows the original solver and has no private iteration trace. Six first contacts are speculative and six penetrating. No standalone parity acceptance.','mask_bits':{'mass':1,'bias':2,'point_velocity':4,'original_delta':8,'force_first':16},'cases_per_mask':12,'masks':32,'repeats':2,'runs':768,'observed_private_states_per_repeat':31*480,'all480_exact_masks':[r['mask'] for r in result if r['native_entry_states_exact']==480],'all12_first_velocity_and_position_exact_masks':[r['mask'] for r in result if r['first_velocities_exact']==r['first_positions_exact']==12],'goal_complete':False,'standalone_physical_fix_accepted':False,'results':result}
(out/'physics-v32-native-factorial-audit.json').write_text(json.dumps(report,indent=2)+'\n')
