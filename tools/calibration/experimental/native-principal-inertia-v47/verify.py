"""Audit the v47 investigation package. A passing audit does not accept the candidate."""
import argparse,gzip,hashlib,json,struct,subprocess,sys,tempfile
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--package',type=Path,required=True);a=p.parse_args();root=a.package.resolve();here=Path(__file__).resolve().parent
sys.dont_write_bytecode=True
sys.path.insert(0,str(here.parent.parent));import compare
f32=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
floats=lambda w:[struct.unpack('<f',struct.pack('<I',x))[0] for x in w]
double=lambda w:struct.unpack('<d',struct.pack('<II',*w))[0]
def events(path):return [json.loads(s) for s in path.read_text().splitlines()]
def protocol(path,platform):
 if path.suffix!='.gz':return compare.load(path,platform)
 with tempfile.TemporaryDirectory() as d:
  p=Path(d)/'run.jsonl';p.write_bytes(gzip.decompress(path.read_bytes()));return compare.load(p,platform)
manifest=json.loads((root/'manifest.json').read_text())
for name,sha in manifest['files'].items():assert hashlib.sha256((root/name).read_bytes()).hexdigest()==sha,name
m=root/'measurements';native=[];rows=[];thresholds=0
for r in range(2):
 ev=events(m/'native24'/f'mass-run{r}.jsonl');before=[o for o in ev if 'dense_inertia' in o];after=[o for o in ev if 'principal_inertia' in o];assert len(before)==len(after)==24
 rows.append([(floats(b['dense_inertia']['words']),floats(c['principal_inertia']['words'])+floats(c['principal_axes']['words'])) for b,c in zip(before,after)])
 current=None
 for o in ev:
  if 'dense_inertia' in o:current=floats(o['dense_inertia']['words'])
  if o.get('function')=='jacobi-sweep-entry':
   expected=(.5*sum(current[4*i]**2 for i in range(3))+sum(current[3*i+j]**2 for i in range(3) for j in range(i+1,3)))*9.999999949504854e-13
   assert double(o['jacobi_threshold']['words'])==expected;thresholds+=1
 original=protocol(m/'native24'/f'untraced-run{r}.jsonl','windows-physx-d3d9');assert protocol(m/'native24'/f'physical-run{r}.jsonl','windows-physx-d3d9')==original
 assert protocol(m/'shapes'/f'untraced-run{r}.jsonl','windows-physx-d3d9')==original;assert protocol(m/'shapes'/f'physical-run{r}.jsonl','windows-physx-d3d9')==original;native.append(original)
assert native[0]==native[1];assert rows[0]==rows[1];assert thresholds==100
with tempfile.TemporaryDirectory() as d:
 binary=Path(d)/'principal-probe';header=here/'src/Rock3dEngine/header/physics'
 subprocess.run(['xcrun','clang++','-std=c++20','-O2','-I'+str(header),str(here/'principal-probe.cpp'),'-o',str(binary)],check=True)
 text='\n'.join(' '.join(str(v) for v in matrix) for matrix,expected in rows[0])+'\n';out=subprocess.check_output([str(binary)],input=text.encode()).decode().splitlines();assert len(out)==24
 for line,(matrix,expected) in zip(out,rows[0]):assert [f32(float(v)) for v in line.split()[1:]]==expected
summary={'native_matrices_and_outputs_exact':24,'native_fields_exact':288,'native_thresholds_exact':thresholds,'scopes':{},'goal_complete':False,'runtime_correction_accepted':False}
for scope,reference,expected in [('mac24',native[0],6),('long72',protocol(m/'reference-long72.jsonl','windows-physx-d3d9'),72),('old6',protocol(m/'reference-old6.jsonl','windows-physx-d3d9'),6),('focused50',protocol(m/'reference-focused50.jsonl','windows-physx-d3d9'),1)]:
 loaded={}
 for mode in ['baseline','control','ordinary']:
  runs=[protocol(m/scope/f'calibration-principal-{mode}-run{r}-mac.jsonl','mac-jolt-metal') for r in range(2)];assert runs[0]==runs[1];loaded[mode]=runs[0]
 assert loaded['baseline']==loaded['control'];assert set(loaded['ordinary'])==set(reference)
 exact=[n for n in reference if reference[n]['rows']==loaded['ordinary'][n]['rows']];assert len(exact)==expected
 summary['scopes'][scope]={'exact':len(exact),'total':len(reference),'remaining':[n for n in reference if n not in exact]}
 if scope=='mac24':
  for i in range(6):assert f'pose-v45-inverted-{i}' in exact
  n='monstertruckBoss/spin-mixed';before=max(compare.norm(x['position'],y['position']) for x,y in zip(reference[n]['rows'],loaded['control'][n]['rows']));after=max(compare.norm(x['position'],y['position']) for x,y in zip(reference[n]['rows'],loaded['ordinary'][n]['rows']));assert after>before;summary['broader_position_regression']={'case':n,'control':before,'candidate':after}
for stage in range(4):
 for repeat in range(2):
  native=protocol(m/'isolation-reference'/f'isolation-stage{stage:02d}-run{repeat}-windows.jsonl','windows-physx-d3d9');mac=protocol(m/'isolation'/f'isolation-stage{stage:02d}-run{repeat}-mac.jsonl','mac-jolt-metal');assert set(native)==set(mac);assert all(native[n]['rows']==mac[n]['rows'] for n in native)
with tempfile.TemporaryDirectory() as d:
 tmp=Path(d)
 for r in range(2):
  (tmp/f'calibration-depth-ordinary-run{r}-v39-mac.jsonl').symlink_to(m/'focused50'/f'calibration-principal-ordinary-run{r}-mac.jsonl')
 subprocess.run([sys.executable,str(here/'compare-solver.py'),'--measurements',str(tmp),'--reference-dir',str(m/'private-step50'),'--step','50'],check=True)
 solver=json.loads((tmp/'solver-comparison.json').read_text());assert solver['repeats']['0']==solver['repeats']['1'];case=solver['repeats']['0']['pose-v45-inverted-5'];assert case['rows']==20 and not case['differences'];summary['actual_solver_rows_exact']=20
with tempfile.TemporaryDirectory() as d:
 tmp=Path(d);descriptor=json.loads((here.parent/'native-principal-inertia-v47-base.json').read_text())
 for rel,sha in descriptor['files'].items():
  if sha['before'] is not None:
   src=here/'before'/rel;assert hashlib.sha256(src.read_bytes()).hexdigest()==sha['before'];dst=tmp/rel;dst.parent.mkdir(parents=True,exist_ok=True);dst.write_bytes(src.read_bytes())
 subprocess.run(['git','init','-q',str(tmp)],check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
 subprocess.run(['git','-C',str(tmp),'apply','--whitespace=nowarn',str(here.parent/'native-principal-inertia-v47.patch')],check=True)
 for rel,sha in descriptor['files'].items():assert (tmp/rel).read_bytes()==(here/rel).read_bytes();assert hashlib.sha256((tmp/rel).read_bytes()).hexdigest()==sha['after']
 summary['patch_sources_byte_exact']=True
summary['force_isolation_exact']=189;print(json.dumps(summary,indent=2))
