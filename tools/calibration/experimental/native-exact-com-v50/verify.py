"""Audit saved actual runs and byte-exact patch; does not manufacture Windows results."""
from pathlib import Path
import argparse,json,hashlib,importlib.util,sys,subprocess,tempfile
p=argparse.ArgumentParser();p.add_argument('--package',type=Path,required=True);p.add_argument('--out',type=Path,required=True);a=p.parse_args();root=a.package.resolve();here=Path(__file__).resolve().parent;sys.dont_write_bytecode=True;sys.path.insert(0,str(here.parents[1]));import compare
spec=importlib.util.spec_from_file_location('words',here.parent/'native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
manifest=json.loads((root/'manifest.json').read_text());
for name,digest in manifest['files'].items():assert hashlib.sha256((root/name).read_bytes()).hexdigest()==digest,name
m=root/'measurements';meta=json.loads((m/'gates/capture-manifest.json').read_text());assert hashlib.sha256((root/'bin/Mac/SpringProbe').read_bytes()).hexdigest()==meta['probe_sha256'];reports={}
for scope in ['native24','long72','old6','isolation00','isolation01','isolation02','isolation03']:
 for mode in ['control','candidate']:
  paths=[m/'gates'/f'{scope}-{mode}-run{i}-mac.jsonl' for i in range(2)];d=[compare.load(x,'mac-jolt-metal') for x in paths];assert d[0]==d[1]
  if mode=='control':assert d[0]==compare.load(m/'v49-controls'/f'{scope}-mac.jsonl','mac-jolt-metal')
  report=words.audit(m/'references'/f'{scope}-windows.jsonl',paths[0]);reports[scope+'-'+mode]=report
  remaining=['body-slide-x','dirtdevil/spin-mixed'] if mode=='control' else ['body-slide-x'];assert report['cases_remaining']==(remaining if scope=='native24' else []),(scope,mode)
base=json.loads((here/'base.json').read_text())
with tempfile.TemporaryDirectory() as td:
 dest=Path(td)
 for name,h in base['files'].items():
  for label,key in [('before/','before_sha256'),('','after_sha256')]:assert hashlib.sha256((here/(label+name)).read_bytes()).hexdigest()==h[key]
  q=dest/name;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes((here/'before'/name).read_bytes())
 subprocess.run(['git','apply','--check','--unsafe-paths',str(here.parent/'native-exact-com-v50.patch')],cwd=dest,check=True);subprocess.run(['git','apply','--unsafe-paths',str(here.parent/'native-exact-com-v50.patch')],cwd=dest,check=True)
 for name,h in base['files'].items():assert hashlib.sha256((dest/name).read_bytes()).hexdigest()==h['after_sha256']
ref=compare.load(m/'references/native24-windows.jsonl','windows-physx-d3d9')['body-slide-x']
project=[]
for repeat in range(2):
 assert compare.load(m/'slide-native'/f'physical-run{repeat}.jsonl','windows-physx-d3d9')['body-slide-x']==ref
 records=[]
 for x in map(json.loads,(m/'slide-native'/f'private-run{repeat}.jsonl').read_text().splitlines()):
  if x.get('function') not in ['row-prepared','normal-solve-exit']:continue
  r={'function':x['function'],'hit':x['hit'],'solve_tick':x['solve_tick']}
  field='prepared_row' if x['function']=='row-prepared' else 'native_solver_row';ix=[*range(4,7),*range(16,20)]
  if field=='native_solver_row':ix+=list(range(20,29))
  r[field]=[x[field]['words'][i] for i in ix];body='solver_body0' if field=='prepared_row' else 'native_body0';r[body]=[x[body]['words'][i] for i in [0,1,2,4,5,6,11,*range(15,24)]];records.append(r)
 assert len(records)==48;project.append(records)
assert project[0]==project[1]
for step in (9,10):
 payloads=[]
 for rep in range(2):
  path=m/'slide-mac'/f'step{step}-run{rep}.jsonl';assert compare.load(path,'mac-jolt-metal')['body-slide-x']==compare.load(m/'gates/native24-candidate-run0-mac.jsonl','mac-jolt-metal')['body-slide-x'];payloads.append([(r['event'],r.get('detail'),r.get('sample')) for r in map(json.loads,path.read_text().splitlines()) if r['event'].startswith('physics.')])
 assert payloads[0]==payloads[1]
assert '100% tests passed, 0 tests failed out of 53' in (root/'evidence/ctest.log').read_text()
serialization=json.loads((root/'evidence/serialization.json').read_text());assert serialization['legacy_bytes_preserved'] and serialization['exact_center_and_geometry_survive_roundtrip'] and serialization['shared_child_and_userdata_survive'] and serialization['adjacent_stream_record_preserved']
a.out.write_text(json.dumps({'scope':'Actual two-repeat 291 fixtures, byte-exact float32 parity, v49 default-off control, contact capture preservation and patch; serialization compiled separately in local build.','results':reports,'patch_exact':True,'native_slide_defined_rows_repeat_exact':48,'native_slide_baseline_preserved':True,'mac_contact_telemetry_preserves_physics':True,'serialization':serialization,'runtime_promoted':False,'goal_complete':False},indent=2)+'\n');print('Verified 290/291 candidate fixtures; body-slide-x remains; 53 CTests passed; default-off and patch exact')
