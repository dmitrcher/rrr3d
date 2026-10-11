#!/usr/bin/env python3
"""Verify native trace repeats, baseline preservation and forward kernel reconstruction."""
import argparse, hashlib, json, struct, sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[2]))
import compare
from model import reconstruct

def physical(path):return compare.load(path,'windows-physx-d3d9')
def events(path):
 return [(r['event'],r['detail'],r.get('sample')) for r in map(json.loads,path.read_text(encoding='utf-8-sig').splitlines())]
def trace_projection(path):
 result=[]
 for r in map(json.loads,path.read_text().splitlines()):
  if 'function' not in r:continue
  record={'function':r['function'],'hit':r['hit'],'offset':r['offset']}
  if 'contact_handle' in r:record.update(handle=r['contact_handle'],kind=r['handle_kind'],lookup=r['lookup_rva'])
  for field in ['prepared_row','native_solver_row']:
   if field in r:
    w=r[field]['words'];indexes=list(range(4,20))+list(range(23,28))+list(range(29,32))
    if r['function'] in ['solver-initialize-exit','solver-iteration-entry']:indexes+=list(range(20,23))
    record[field]={str(i):w[i] for i in indexes};record[field]['type']=w[0]&0xff
  for field in ['solver_body0','solver_body1','native_body0','native_body1']:
   if field in r:record[field]={str(i):r[field]['words'][i] for i in [0,1,2,4,5,6,11,*range(15,24)]}
  if 'native_bias_scale' in r:record['native_bias_scale']=r['native_bias_scale']['words']
  result.append(record)
 return result

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--measurements',type=Path,required=True);p.add_argument('--out',type=Path,required=True);a=p.parse_args()
 baseline=physical(a.measurements/'reference-row-iterations4-run0-v30-windows.jsonl')
 source={};current=None
 for r in map(json.loads,(a.measurements/'calibration-row-arithmetic-both-mask0-run0-v30-mac.jsonl').open()):
  if r['event']=='calibration.begin':current=r['detail']
  if r['event']=='physics.source-normal-row' and r['sample']['phase']=='biased-final':source.setdefault(current,{}).setdefault(r['sample']['step'],[]).append(r['sample'])
 report=[]
 for mass in range(3):
  for height in range(2):
   for velocity in range(2):
    name=f'isolate-03-contact-m{mass}-h{height}-v{velocity}-f1';prefix='reference-native-kernel-'+name+'-'
    traces=[a.measurements/(prefix+f'geometry-run{i}-v31-windows.jsonl') for i in range(2)]
    states=[a.measurements/(prefix+f'physics-run{i}-v31-windows.jsonl') for i in range(2)]
    assert all(physical(path)=={name:baseline[name]} for path in states),(name,'Baseline changed')
    assert events(states[0])==events(states[1]),(name,'Physics/event repeat differs')
    projected=[trace_projection(path) for path in traces];assert projected[0]==projected[1],(name,'Defined numerical trace repeat differs')
    predicted=[reconstruct(traces[i],states[i],name) for i in range(2)];assert predicted[0]==predicted[1]
    model=predicted[0];assert model['entries_exact']==40 and model['final_velocity_exact'],(name,'Kernel reconstruction failed')
    native=[r for r in projected[0] if r['function']=='solver-initialize-exit'][:8];src=source[name][model['first_tick']][:8]
    encode=lambda x:struct.unpack('I',struct.pack('f',x))[0]
    matches=[r['native_solver_row']['20']==encode(s['effective_mass']) for r,s in zip(native,src)]
    configs=[r for r in map(json.loads,traces[0].read_text().splitlines()) if r.get('event')=='watch-config'];assert configs and all(r['success'] for r in configs)
    report.append({'case':name,'first_tick':model['first_tick'],'native_kernel_inputs_checked':40,'effective_masses_reconstructed_exact':model['effective_masses_reconstructed_exact'],'scaled_biases_reconstructed_exact':model['scaled_biases_reconstructed_exact'],'forward_v_angular_lambda_exact':True,'final_velocity_exact':True,'baseline_all120_states_preserved':True,'physics_and_event_repeats_exact':True,'defined_trace_repeats_exact':True,'mac_replayed_geometry_effective_mass_matches':sum(matches),'effective_masses_checked':len(matches),'watch_configuration_successful':True,'files':[{'name':path.name,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()} for path in traces+states]})
    print(name,'40 exact states; all120 baseline states preserved; K',sum(matches),'/8',flush=True)
 result={'schema':1,'status':'progress','goal_complete':False,'standalone_physical_fix_accepted':False,'scope':'Native PhysX2.8.4 actual initialization and iterative kernel, identity actor orientation, first impact in12 flat isolated ground-on cases, four biased passes and one unbiased pass. Native recorded rows are diagnostic inputs. Raw pointer addresses, heap padding and unspecified fields are excluded from trace repeat claims.','cases':report,'effective_masses_reconstructed_exact':sum(r['effective_masses_reconstructed_exact'] for r in report),'scaled_biases_reconstructed_exact':sum(r['scaled_biases_reconstructed_exact'] for r in report),'kernel_states_exact':sum(r['native_kernel_inputs_checked'] for r in report),'mac_mass_matches':sum(r['mac_replayed_geometry_effective_mass_matches'] for r in report),'core_sha256':'c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb'}
 a.out.write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
