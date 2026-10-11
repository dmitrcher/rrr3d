from pathlib import Path
import argparse
p=argparse.ArgumentParser();p.add_argument('--measurements',type=Path,required=True);p.add_argument('--frame-probe',type=Path,required=True);a=p.parse_args()
import json,struct,subprocess,sys,hashlib
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
f=lambda x:struct.unpack('f',struct.pack('f',x))[0]
def floats(row,key):
 w=row[key]['words'];return list(struct.unpack(f'{len(w)}f',struct.pack(f'{len(w)}I',*w)))
results=[];out=a.measurements;baseline=compare.load(out/'reference-row-iterations4-run0-v30-windows.jsonl','windows-physx-d3d9')
for m in range(3):
 for h in range(2):
  for v in range(2):
   name=f'isolate-03-contact-m{m}-h{h}-v{v}-f1';reports=[]
   for repeat in range(2):
    path=out/f'reference-native-preparation-{name}-geometry-run{repeat}-v33-windows.jsonl';rows=list(map(json.loads,path.open()))
    groups={k:[r for r in rows if r.get('function')==k][:8] for k in ['material-inputs-ready','material-global-error','material-quaternion-adjusted','material-error-ready','set-error-entry','converted-error-after-copy']};assert all(len(g)==8 for g in groups.values())
    values=[floats(x,'material_input_error')[0] for x in groups['material-inputs-ready']]
    model=[json.loads(x) for x in subprocess.check_output([str(a.frame_probe.resolve())],input='\n'.join(format(e,'.9g') for e in values)+'\n',text=True).splitlines()];assert len(model)==8
    checks=[]
    for i,r in enumerate(model):
     r={k:list(map(f,x)) for k,x in r.items()};old=floats(groups['material-inputs-ready'][i],'material_stack')[43:47];new=floats(groups['material-quaternion-adjusted'][i],'material_stack')[9:13];world=floats(groups['material-global-error'][i],'material_global_error');error=floats(groups['material-error-ready'][i],'material_output_error');setter=floats(groups['set-error-entry'][i],'setter_input');raw=floats(groups['converted-error-after-copy'][i],'converter_current_output')[14]
     assert r['raw']==old and r['material']==new and r['global']==world and r['error']==error and [r['error'][0],0.,0.]==setter and raw==values[i],(name,i,r,old,new,world,error)
     assert groups['set-error-entry'][i]['stack_return_rva']==0x2042ef
     assert groups['material-error-ready'][i]['material_stack']['words'][0xe0//4]&0x20
     checks.append({'point':i,'raw_scalar':values[i],'raw_quaternion':old,'generated_material_quaternion':new,'generated_global_error':r['global'],'generated_error_before_tangent_mask':r['error'],'generated_normal_constraint_error':r['error'][0],'actual_setter_error_after_tangent_mask':setter,'exact':True})
    phys=out/f'reference-native-preparation-{name}-physics-run{repeat}-v33-windows.jsonl';assert compare.load(phys,'windows-physx-d3d9')[name]==baseline[name];reports.append(checks)
   assert reports[0]==reports[1];results.append({'case':name,'semantic_numeric_repeats_exact':True,'full120_native_physical_states_unchanged':True,'points':reports[0]})
report={'scope':'Actual ARM64 C++ helper generates raw and material quaternions from identity actor and +Z fixture normal, then transforms recorded raw scalar errors forward/inverse. No recorded quaternions or expected outputs are passed to the helper. Raw geometry distance remains a recorded Native input; not independent contact-generation acceptance. All12 cases captured twice in real Windows, all120 states unchanged from v30.','cases':12,'repeats':2,'points_per_case':8,'generated_raw_quaternions_exact':96,'generated_material_quaternions_exact':96,'generated_global_errors_exact':96,'generated_constraint_errors_exact':96,'physical_reference_unchanged':True,'goal_complete':False,'standalone_contact_accepted':False,'helper_sha256':hashlib.sha256(a.frame_probe.read_bytes()).hexdigest(),'results':results}
(out/'physics-v33-derived-error-audit.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated quaternions and errors:96/96 exact. Both repeats match; all120 Native states unchanged.')
