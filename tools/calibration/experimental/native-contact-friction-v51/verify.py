"""Verify actual captures and the exact default-off patch; never synthesizes Native histories."""
from pathlib import Path
import argparse,json,hashlib,importlib.util,sys,tempfile,subprocess,struct
p=argparse.ArgumentParser();p.add_argument('--package',type=Path,required=True);p.add_argument('--out',type=Path,required=True);a=p.parse_args();root=a.package.resolve();here=Path(__file__).resolve().parent;sys.dont_write_bytecode=True;sys.path.insert(0,str(here.parents[1]));import compare
spec=importlib.util.spec_from_file_location('words',here.parent/'native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
manifest=json.loads((root/'manifest.json').read_text())
def sha(path):
 h=hashlib.sha256()
 with path.open('rb') as f:
  for part in iter(lambda:f.read(1024*1024),b''):h.update(part)
 return h.hexdigest()
for name,digest in manifest['files'].items():assert sha(root/name)==digest,name
m=root/'measurements';meta=json.loads((m/'gates/capture-manifest.json').read_text());assert hashlib.sha256((root/'bin/Mac/SpringProbe').read_bytes()).hexdigest()==meta['probe_sha256'];reports={};total=0
for scope in ['native24','long72','old6','isolation00','isolation01','isolation02','isolation03']:
 for mode in ['control','candidate']:
  paths=[m/'gates'/f'{scope}-{mode}-run{i}-mac.jsonl' for i in range(2)];d=[compare.load(x,'mac-jolt-metal') for x in paths];assert d[0]==d[1]
  if mode=='control':assert d[0]==compare.load(m/'v50-controls'/f'{scope}-mac.jsonl','mac-jolt-metal')
  report=words.audit(m/'references'/f'{scope}-windows.jsonl',paths[0]);reports[scope+'-'+mode]=report;assert report['cases_remaining']==(['body-slide-x'] if mode=='control' and scope=='native24' else []),(scope,mode)
  if mode=='candidate':total+=len(report['cases'])
assert total==291
base=json.loads((here/'base.json').read_text())
with tempfile.TemporaryDirectory() as td:
 dest=Path(td)
 for name,h in base['files'].items():
  for label,key in [('before/','before_sha256'),('','after_sha256')]:assert hashlib.sha256((here/(label+name)).read_bytes()).hexdigest()==h[key]
  q=dest/name;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes((here/'before'/name).read_bytes())
 subprocess.run(['git','apply','--check','--unsafe-paths',str(here.parent/'native-contact-friction-v51.patch')],cwd=dest,check=True);subprocess.run(['git','apply','--unsafe-paths',str(here.parent/'native-contact-friction-v51.patch')],cwd=dest,check=True)
 for name,h in base['files'].items():assert hashlib.sha256((dest/name).read_bytes()).hexdigest()==h['after_sha256']
ref=compare.load(m/'references/native24-windows.jsonl','windows-physx-d3d9')['body-slide-x'];candidate=compare.load(m/'gates/native24-candidate-run0-mac.jsonl','mac-jolt-metal')['body-slide-x'];u=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];count=0
for tick in (10,11,22,30):
 payloads=[]
 for rep in range(2):
  n=list(map(json.loads,(m/f'slide-native/{tick}/private-run{rep}.jsonl').read_text().splitlines()));mp=m/f'slide-mac/step{tick}-run{rep}.jsonl';ms=list(map(json.loads,mp.read_text().splitlines()));assert compare.load(m/f'slide-native/{tick}/physical-run{rep}.jsonl','windows-physx-d3d9')['body-slide-x']==ref;assert compare.load(mp,'mac-jolt-metal')['body-slide-x']==candidate
  ns=[x for x in n if x.get('function')=='scalar-solver-row' and x['solve_tick']==tick];ni=[x for x in n if x.get('function')=='normal-solve-exit' and x['solve_tick']==tick];mac=[x['sample'] for x in ms if x['event']=='physics.native-contact-row'];assert len(ns)==len(ni)==len(mac)==40
  for nr,init,mr in zip(ns,ni,mac):
   row=nr['native_solver_row']['words'];pr=init['native_solver_row']['words'];b=nr['native_body0']['words']
   for field,expected in [('lever_arm',pr[4:7]),('velocity',b[:3]),('angular',b[4:7]),('inverse_inertia',b[15:24]),('effective_mass',[pr[20]]),('scaled_bias',[pr[24]]),('lambda',[row[27]])]:
    v=mr[field] if type(mr[field]) is list else [mr[field]];assert list(map(u,v))==expected,(tick,rep,field)
   count+=1
  payloads.append([(x['event'],x.get('detail'),x.get('sample')) for x in ms if x['event'].startswith('physics.')])
 assert payloads[0]==payloads[1]
assert count==320
assert '100% tests passed, 0 tests failed out of 53' in (root/'evidence/ctest.log').read_text()
transforms=json.loads((root/'evidence/transforms.json').read_text());assert transforms['records']==1640 and set(transforms['results'])=={'O0','O2'}
pose=json.loads((root/'evidence/pose-matrix.json').read_text());assert all(x['matrix_and_five_rotator_bits_preserved'] and x['nan_and_large_nonunit_still_rejected'] for x in pose['results'].values())
with tempfile.TemporaryDirectory() as td:
 broad_out=Path(td)/'broad.json';subprocess.run([sys.executable,str(here/'audit-broad.py'),'--package',str(root),'--out',str(broad_out)],check=True);broad=json.loads(broad_out.read_text());assert broad==json.loads((root/'evidence/broad-audit.json').read_text());assert len(broad['exact'])==65 and len(broad['remaining'])==543 and broad['samples']==255864 and broad['two_repeats_exact']
report={'broad_exact':65,'broad_remaining':543,'broad_samples':255864,'scope':'291 actual fixtures, two repeats, original Windows float32 words; default-off v50 control; 160 private contact rows twice; exact patch.','candidate_exact':291,'total':291,'results':reports,'ctest_passed':53,'contact_rows_compared':count,'transforms':transforms,'pose_validation':pose,'runtime_promoted':False,'goal_complete':False};a.out.write_text(json.dumps(report,indent=2)+'\n');print('Verified 291/291 current fixtures, 53 CTests, 320 actual contact rows; broad suite verified 65/608 exact, 543 remaining; contact-report APIs remain open')
