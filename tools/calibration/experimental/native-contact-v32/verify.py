from pathlib import Path
import json,struct,sys,hashlib
import argparse
p=argparse.ArgumentParser();p.add_argument('--measurements',type=Path,required=True);p.add_argument('--contracts',type=Path,required=True);a=p.parse_args();out=a.measurements
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
f=lambda x:struct.unpack('f',struct.pack('f',x))[0]
def words(r,k):return list(struct.unpack('32f',struct.pack('32I',*r[k]['words'])))
def events(path):return [(r['event'],r['detail'],r.get('sample')) for r in map(json.loads,path.read_text().splitlines())]
report=[];baseline=compare.load(out/'reference-row-iterations4-run0-v30-windows.jsonl','windows-physx-d3d9')
for mode,mask in [('ordinary',0),('errors',31),('ordinary',31),('points',31)]:
 paths=[out/f'calibration-native-arithmetic-{mode}-mask{mask}-run{repeat}-v32-mac.jsonl' for repeat in range(2)]
 assert events(paths[0])==events(paths[1]),(mode,'Semantic repeats differ')
 got=compare.load(paths[0],'mac-jolt-metal');cases=[]
 for name,case in got.items():
  tick=next(c['first_tick'] for c in json.loads(a.contracts.read_text()) if c['case']==name)
  entry={'case':name,'first_tick':tick,'first_velocity_exact':case['rows'][tick]['velocity']==baseline[name]['rows'][tick]['velocity'],'first_position_exact':case['rows'][tick]['position']==baseline[name]['rows'][tick]['position'],'initial_pre_contact_exact':case['rows'][:tick]==baseline[name]['rows'][:tick]}
  if mode=='errors':
   native=list(map(json.loads,(out/f'reference-native-kernel-{name}-geometry-run0-v31-windows.jsonl').read_text().splitlines()));it=[r for r in native if r.get('function')=='solver-iteration-entry'][:40];prepared=[words(r,'native_solver_row') for r in native if r.get('function')=='solver-initialize-exit'][:8]
   rows=[];current=None
   for r in map(json.loads,paths[0].read_text().splitlines()):
    if r['event']=='calibration.begin':current=r['detail']
    if current==name and r['event']=='physics.native-contact-row':rows.append(r['sample'])
   assert len(rows)==len(it)==40,(name,len(rows));states=0;constants=0;issues=[]
   for j,(row,n) in enumerate(zip(rows,it)):
    b=words(n,'native_body0');nr=words(n,'native_solver_row');init=prepared[j%8]
    state=[list(map(f,row['velocity'])),list(map(f,row['angular'])),f(row['lambda'])];expected=[b[:3],b[4:7],nr[27]]
    const=[f(row['effective_mass']),f(row['scaled_bias'])];expectedconst=[init[20],init[24] if j<32 else nr[24]]
    states+=state==expected;constants+=const==expectedconst
    if state!=expected or const!=expectedconst:issues.append({'entry':j,'state':state,'expected':expected,'constants':const,'expected_constants':expectedconst})
    assert row['mask']==31 and row['diagnostic_error'] and row['iteration']==j//8 and row['index']==j%8
   entry.update(native_forward_states_exact=states,native_constants_exact=constants,issues=issues);assert states==constants==40,(name,issues[:1])
  cases.append(entry)
 report.append({'mode':mode,'mask':mask,'semantic_repeats_exact':True,'cases':cases,'files':[{'name':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in paths]})
 print(mode,mask,'first velocity exact',sum(x['first_velocity_exact'] for x in cases),'position exact',sum(x['first_position_exact'] for x in cases),flush=True)
result={'scope':'Actual ARM64 Mac contact prototype. Fullmask31 under recorded Native positions/separation and explicit Native constraint errors reproduces actual Native first-contact40-entry v/angular/lambda sequence. Diagnostic intervention is not standalone parity.','goal_complete':False,'standalone_physical_fix_accepted':False,'native_forward_states_exact':sum(c['native_forward_states_exact'] for r in report if r['mode']=='errors' for c in r['cases']),'reports':report}
(out/'physics-v32-native-arithmetic-audit.json').write_text(json.dumps(result,indent=2)+'\n')
for mode in ['ordinary','points','errors']:
 p=out/f'calibration-native-arithmetic-{mode}-mask31-run0-v32-mac.jsonl';r=compare.compare(out/'reference-row-iterations4-run0-v30-windows.jsonl',p);(out/f'physics-v32-native-arithmetic-{mode}-comparison.json').write_text(json.dumps(r,indent=2)+'\n');print(mode,'full120-step remaining',r['exact_parity']['cases_remaining'],flush=True)
