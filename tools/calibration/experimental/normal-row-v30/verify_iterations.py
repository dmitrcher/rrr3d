from pathlib import Path
import json,sys,hashlib,copy,argparse
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
parser=argparse.ArgumentParser(description='Verify saved v30 flat-contact diagnostic captures.')
parser.add_argument('--measurements',type=Path,required=True)
parser.add_argument('--contracts',type=Path,required=True)
args=parser.parse_args()
out=args.measurements;contracts={c['case']:c for c in json.loads(args.contracts.read_text())}
def read(p):
 result=[];n=None
 for line in p.open():
  r=json.loads(line);compare.finite(r)
  if r['event']=='calibration.begin':n=r['detail']
  result.append({'case':n,'event':r['event'],'detail':r['detail'],'sample':r.get('sample')})
 return result
def repeat(prefix):
 ps=[out/(prefix.format(r=r)+'.jsonl') for r in range(2)];a,b=[read(p) for p in ps];assert a==b
 return a,{'files':[p.name for p in ps],'records':len(a),'semantic_sha256':hashlib.sha256(json.dumps(a,sort_keys=True,separators=(',',':')).encode()).hexdigest(),'repeat_identical':True}
baseline=compare.load(out/'reference-inverse-mass-flat-run0-v30-windows.jsonl','windows-physx-d3d9')
base_records=read(out/'reference-inverse-mass-flat-run0-v30-windows.jsonl')
native_captures={};mac_captures={};geometry={}
for count in [1,2,4,8]:
 rs,a=repeat(f'reference-row-iterations{count}-run{{r}}-v30-windows');states=compare.load(out/a['files'][0],'windows-physx-d3d9');assert set(states)==set(contracts)
 actual={r['case']:r['sample']['actual_iterations'] for r in rs if r['event']=='calibration.solver-iterations'};assert actual=={n:count for n in contracts};a['actual_sdk_iterations_verified']=True
 inputs=[]
 for r in rs:
  n=r['case'];s=r['sample']
  if n in contracts and r['event'] in ['calibration.local-points','calibration.body-points','calibration.point-force'] and s['tick']==contracts[n]['first_tick'] and r['event']!='calibration.point-force':inputs.append(r)
 if count==1:geometry=inputs
 else:assert inputs==geometry
 for n,c in states.items():
  t=contracts[n]['first_tick'];assert c['rows'][:t]==baseline[n]['rows'][:t]
  if count==4:assert c==baseline[n]
 a['pre_contact_states_exact_all12']=True;a['first_raw_local_points_and_callback_geometry_identical_across_counts']=True
 native_captures[count]=a
 for mask in [0,2]:
  prefix=f'calibration-row-arithmetic-both-mask{mask}-run{{r}}-v30-mac' if count==4 else f'calibration-row-iterations{count}-both-mask{mask}-run{{r}}-v30-mac'
  rr,b=repeat(prefix);ms=compare.load(out/b['files'][0],'mac-jolt-metal');assert set(ms)==set(contracts)
  replays={}
  for r in rr:
   if r['event']=='physics.diagnostic-contact-replay':
    n=r['case'];assert n not in replays;s=r['sample'];assert s['step']==contracts[n]['first_tick'] and s['points']==8 and s['positions_replayed'] and s['separations_replayed'];replays[n]=True
  assert set(replays)==set(contracts)
  cases={}
  for n,c in ms.items():
   t=contracts[n]['first_tick'];assert c['rows'][:t]==states[n]['rows'][:t]
   target=states[n]['rows'][t];got=c['rows'][t]
   cases[n]={'pre_contact_states_exact':True,'first_contact_state_exact':target==got,'first_position_error':compare.norm(target['position'],got['position']),'first_velocity_error':compare.norm(target['velocity'],got['velocity'])}
  b['cases']=cases;b['scope']='One-step recorded geometry intervention; not standalone parity.';mac_captures[str(count)+'-mask'+str(mask)]=b
report={'schema':1,'native':native_captures,'mac_interventions':mac_captures,'actual_native_solver_iterations_verified':True,'all_semantic_repeats_exact':True,'goal_complete':False}
(out/'physics-v30-row-iterations-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print('All iteration captures complete and semantically repeatable; actual Native counts1/2/4/8 verified; first geometry and pre-contact states exact across counts.')
