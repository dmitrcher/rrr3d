from pathlib import Path
import argparse,json,struct,sys,subprocess
p=argparse.ArgumentParser();p.add_argument('--measurements',type=Path,required=True);p.add_argument('--cache-probe',type=Path,required=True);a=p.parse_args();out=a.measurements
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
from experimental.verify_isolation import verify as verify_isolation
f=lambda v:struct.unpack('f',struct.pack('f',v))[0]
def fs(r,k):
 w=r[k]['words'];return list(struct.unpack(f'{len(w)}f',struct.pack(f'{len(w)}I',*w)))
def events(p):return [(r['event'],r['detail'],r.get('sample')) for r in map(json.loads,p.open())]
baseline=out/'reference-row-iterations4-run0-v30-windows.jsonl';gold=compare.load(baseline,'windows-physx-d3d9');report={}
for mode in ['ordinary','control']:
 paths=[out/f'calibration-native-cache-{mode}-run{r}-v35-mac.jsonl' for r in range(2)];assert events(paths[0])==events(paths[1]);comparison=compare.compare(baseline,paths[0]);assert comparison['exact_parity']['complete']==(mode=='ordinary')
 if mode=='control':assert compare.load(paths[0],'mac-jolt-metal')==compare.load(out/'calibration-native-edge-ordinary-run0-v34-mac.jsonl','mac-jolt-metal')
 report[mode]=comparison['exact_parity']
current=None;mac={}
for r in map(json.loads,(out/'calibration-native-cache-ordinary-run0-v35-mac.jsonl').open()):
 if r['event']=='calibration.begin':current=r['detail']
 if r['event']=='physics.native-contact-row':mac.setdefault(current,[]).append(r['sample'])
 assert r['event']!='physics.diagnostic-contact-replay'
count=0
for hv,first,late in [('h0-v0',25,27),('h0-v1',22,24),('h1-v0',2,7),('h1-v1',1,3)]:
 name='isolate-03-contact-m0-'+hv+'-f1'
 for repeat in range(2):
  path=out/f'reference-native-later-{name}-physics-run{repeat}-v35-windows.jsonl';got=compare.load(path,'windows-physx-d3d9');assert got[name]==gold[name]
 n=list(map(json.loads,(out/f'reference-native-later-{name}-geometry-run0-v35-windows.jsonl').open()));it=[r for r in n if r.get('function')=='solver-iteration-entry'];ini=[r for r in n if r.get('function')=='solver-initialize-exit'];pre=[r for r in n if r.get('function')=='prepared-row-before-response'];offset=(late-first)*40;errOffset=(late-first)*8
 assert len(mac[name])==40
 for j,r in enumerate(mac[name]):
  row=fs(it[offset+j],'native_solver_row');body=fs(it[offset+j],'native_body0');prepared=fs(ini[offset+j],'native_solver_row');err=fs(pre[errOffset+j%8],'input_constraint')[14]
  wanted={'velocity':body[:3],'angular':body[4:7],'lambda':row[27],'effective_mass':prepared[20],'scaled_bias':prepared[24] if j<32 else row[24],'constraint_error':err,'lever_arm':prepared[4:7]}
  for key,v in wanted.items():assert (list(map(f,r[key])) if isinstance(r[key],list) else f(r[key]))==v,(name,j,key)
  assert r['step']==late and r['index']==j%8 and r['iteration']==j//8 and r['mask']==31 and r['derived_error'] and not r['diagnostic_error'];count+=1
report['later_native_rows_exact']=count
for repeat in range(2):
 name='isolate-03-contact-m0-h1-v1-f1';native=out/f'reference-native-cache-watch-{name}-physics-run{repeat}-v35-windows.jsonl';assert compare.load(native,'windows-physx-d3d9')[name]==gold[name]
 n=list(map(json.loads,(out/f'reference-native-cache-watch-{name}-geometry-run{repeat}-v35-windows.jsonl').open()));rows=[r for r in n if r.get('function')=='solver-row-watch' and r['offset']==0x254225];assert len(rows)==60
 inputs=[];expected=[]
 for r in rows:
  raw=fs(r,'watched_raw_point');delta=fs(r,'watched_stack')[8:11];inputs.append(' '.join(map(repr,raw[18:22]+delta)));expected.append(raw[16:18])
 result=subprocess.run([str(a.cache_probe.resolve())],input='\n'.join(inputs)+'\n',capture_output=True,text=True,check=True);got=[list(map(f,map(float,row.split()))) for row in result.stdout.splitlines()];assert got==expected
report['recorded_input_cached_tangents_exact']=120
for stage,cases in [('00',9),('01',108),('02',48),('03',24)]:
 refs=[out/f'reference-isolation-stage{stage}-run{r}-v22-windows.jsonl' for r in range(2)];macs=[out/f'calibration-isolation-stage{stage}-run{r}-v35-mac.jsonl' for r in range(2)];verified=verify_isolation(*refs,*macs);assert verified['exact_parity']['complete'] and verified['exact_parity']['cases_checked']==cases
report['isolation_cases_exact']=189
for suffix,remaining in [('v35',70),('v35-control',72)]:
 paths=[out/f'calibration-contact-long-neon-material-run{r}-{suffix}-mac.jsonl' for r in range(2)];assert compare.load(paths[0],'mac-jolt-metal')==compare.load(paths[1],'mac-jolt-metal');r=compare.compare(out/'reference-long-contact-run0-v25-windows.jsonl',paths[0]);assert len(r['cases'])==72 and len(r['exact_parity']['cases_remaining'])==remaining
 report[suffix+'_tilted_remaining']=remaining
report['goal_complete']=False
print(json.dumps(report,indent=2))
