from pathlib import Path
import json,sys,struct,hashlib
import argparse
p=argparse.ArgumentParser();p.add_argument('--measurements',type=Path,required=True);p.add_argument('--contracts',type=Path,required=True);a=p.parse_args();out=a.measurements;sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
f=lambda x:struct.unpack('f',struct.pack('f',x))[0]
def floats(r,k):
 w=r[k]['words'];return list(struct.unpack(f'{len(w)}f',struct.pack(f'{len(w)}I',*w)))
def events(path):return [(r['event'],r['detail'],r.get('sample')) for r in map(json.loads,path.open())]
native=compare.load(out/'reference-row-iterations4-run0-v30-windows.jsonl','windows-physx-d3d9');contracts=json.loads(a.contracts.read_text());reports=[]
for mode in ['points','ordinary','errors','control']:
 paths=[out/f'calibration-derived-error-{mode}-run{r}-v33-mac.jsonl' for r in range(2)];assert events(paths[0])==events(paths[1]);got=compare.load(paths[0],'mac-jolt-metal');allrows={};current=None;replays=[]
 for r in map(json.loads,paths[0].open()):
  if r['event']=='calibration.begin':current=r['detail']
  if r['event']=='physics.native-contact-row':allrows.setdefault(current,[]).append(r['sample'])
  if r['event']=='physics.diagnostic-contact-replay':replays.append(r['sample'])
 if mode in ['ordinary','control']:assert not replays
 if mode=='points':assert all(not r.get('native_errors_replayed',False) for r in replays) and len(replays)==12
 cases=[]
 for c in contracts:
  name=c['case'];tick=c['first_tick'];row={'case':name,'first_tick':tick,'speculative':c['first_native_row_speculative'],'first_position_exact':got[name]['rows'][tick]['position']==native[name]['rows'][tick]['position'],'first_velocity_exact':got[name]['rows'][tick]['velocity']==native[name]['rows'][tick]['velocity']}
  if mode in ['points','errors','ordinary']:
   native_rows=list(map(json.loads,(out/f'reference-native-preparation-{name}-geometry-run0-v33-windows.jsonl').open()));iterations=[r for r in native_rows if r.get('function')=='solver-iteration-entry'][:40];prepared=[floats(r,'native_solver_row') for r in native_rows if r.get('function')=='solver-initialize-exit'][:8];errors=[floats(r,'input_constraint')[14] for r in native_rows if r.get('function')=='prepared-row-before-response'][:8];mac=allrows[name];assert len(mac)==40
   states=masses=biases=error_exact=0
   for j,(r,n) in enumerate(zip(mac,iterations)):
    b=floats(n,'native_body0');nr=floats(n,'native_solver_row');states+=list(map(f,r['velocity']))==b[:3] and list(map(f,r['angular']))==b[4:7] and f(r['lambda'])==nr[27];masses+=f(r['effective_mass'])==prepared[j%8][20];biases+=f(r['scaled_bias'])==(prepared[j%8][24] if j<32 else nr[24]);error_exact+=f(r['constraint_error'])==errors[j%8]
    assert r['step']==tick and r['index']==j%8 and r['iteration']==j//8 and r['mask']==31
    if mode in ['points','ordinary']:assert r['derived_error'] and not r['diagnostic_error']
    else:assert r['diagnostic_error'] and not r.get('derived_error',False)
   row.update(native_states_exact=states,effective_masses_exact=masses,scaled_biases_exact=biases,constraint_errors_exact=error_exact)
   if mode in ['points','errors']:assert states==masses==biases==error_exact==40,(name,row)
   if mode=='ordinary':assert error_exact==40,(name,row)
  cases.append(row)
 result=compare.compare(out/'reference-row-iterations4-run0-v30-windows.jsonl',paths[0]);(out/f'physics-v33-derived-{mode}-comparison.json').write_text(json.dumps(result,indent=2)+'\n')
 reports.append({'mode':mode,'semantic_repeats_exact':True,'first_positions_exact':sum(x['first_position_exact'] for x in cases),'first_velocities_exact':sum(x['first_velocity_exact'] for x in cases),'full120_contacts_remaining':len(result['exact_parity']['cases_remaining']),'cases':cases,'files':[{'name':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in paths]});print(mode,reports[-1]['first_positions_exact'],reports[-1]['first_velocities_exact'],reports[-1]['full120_contacts_remaining'],flush=True)
control=compare.load(out/'calibration-derived-error-control-run0-v33-mac.jsonl','mac-jolt-metal');old=compare.load(out/'calibration-native-arithmetic-ordinary-mask0-run0-v32-mac.jsonl','mac-jolt-metal');assert control==old
points=next(r for r in reports if r['mode']=='points');assert points['first_positions_exact']==points['first_velocities_exact']==12
report={'scope':'Actual Mac derives Native constraint error from its separation/skin and generated quaternions. Points-only one-step Native geometry intervention remains; no Native errors or output states are supplied. All480 observed solver entry states and derived constraint errors exact. Full standalone contacts remain unaccepted.','first_contact_native_states_exact':480,'first_contact_derived_errors_exact':96,'first_contact_prepared_masses_biases_exact':96,'first_positions_velocities_exact':12,'ordinary_first_contact_derived_errors_exact':96,'control_ordinary_v32_unchanged':True,'goal_complete':False,'standalone_physical_fix_accepted':False,'reports':reports};(out/'physics-v33-derived-contact-audit.json').write_text(json.dumps(report,indent=2)+'\n')
