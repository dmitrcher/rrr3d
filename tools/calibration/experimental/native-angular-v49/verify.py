import argparse,hashlib,json,struct,subprocess,sys,tempfile,importlib.util
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--package',type=Path,required=True);args=p.parse_args();root=args.package.resolve();here=Path(__file__).resolve().parent
sys.dont_write_bytecode=True;sys.path.insert(0,str(here.parents[1]));import compare
spec=importlib.util.spec_from_file_location('words',here/'compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
manifest=json.loads((root/'manifest.json').read_text())
for n,h in manifest['files'].items():assert hashlib.sha256((root/n).read_bytes()).hexdigest()==h,n
m=root/'measurements';w=lambda x:struct.unpack('<f',struct.pack('<I',x))[0];bits=words.bits
read=lambda p:[json.loads(l) for l in p.read_text().splitlines()]
load=lambda p,platform:compare.load(p,platform)
native=[load(m/'native-angular'/f'angular-run{r}-windows.jsonl','windows-physx-d3d9') for r in range(2)];assert native[0]==native[1]
summary={'runtime_promoted':False,'goal_complete':False,'native_physical_preserved':True,'arithmetic':{},'parity':{},'build_tests':53}
with tempfile.TemporaryDirectory() as d:
 tmp=Path(d)
 for opt in ['O0','O2']:
  exe=tmp/('integrator-'+opt);subprocess.run(['xcrun','clang++','-std=c++20','-'+opt,'-ffp-contract=off',str(here/'angular-probe.cpp'),'-o',str(exe)],check=True)
  total=0
  for scope,name,count in [('integrator','marauder/spin-mixed',120),('integrator-coupled','body-slide-x',180)]:
   repeats=[]
   for repeat in range(2):
    assert load(m/scope/f'physical-run{repeat}.jsonl','windows-physx-d3d9')[name]==native[0][name]
    groups={}
    for r in read(m/scope/f'private-run{repeat}.jsonl'):
     if 'function' in r:groups.setdefault(r.get('integration_frame',r['hit']-1),{})[r['function']]=r
    assert len(groups)==count
    inputrows=[];expected=[]
    for tick,rs in sorted(groups.items()):
     e=rs['integrate-entry'];a=rs['quaternion-written'];assert e['x87_control']==0x37f
     inputrows.append(list(map(w,e['body']['words'][28:32]+e['body']['words'][22:25]))+[w(e['simulation']['words'][15])])
     q=list(map(w,a['body']['words'][28:32]));assert list(map(bits,q))==list(map(bits,native[0][name]['rows'][tick]['rotation']))
     if 'sine-factor' in rs:
      f=rs['sine-factor'];ln=rs['angular-length'];extra=[w(ln['stack']['words'][19]),w(f['stack']['words'][4]),w(f['stack']['words'][5]),w(f['stack']['words'][19])]
     else:extra=[0,0,1,0]
     expected.append(q+extra)
    repeats.append((inputrows,expected))
   assert repeats[0]==repeats[1]
   data=''.join(' '.join(map(repr,v))+'\n' for v in inputrows).encode();got=subprocess.check_output([str(exe)],input=data);actual=[[bits(x) for x in l.split()] for l in got.decode().splitlines()];assert actual==[list(map(bits,v)) for v in expected],(scope,opt);total+=count*8
  summary['arithmetic']['integrator-'+opt]={'float_fields_exact':total}
  exe=tmp/('public-'+opt);subprocess.run(['xcrun','clang++','-std=c++20','-'+opt,'-ffp-contract=off',str(here/'public-angular-probe.cpp'),'-o',str(exe)],check=True)
  repeats=[]
  for repeat in range(2):
   rows=read(m/'native-angular'/f'angular-run{repeat}-windows.jsonl');inputrows=[];expected=[]
   for r in rows:
    if r['event']!='calibration.angular-momentum':continue
    name=r['detail'];s=r['sample'];inputrows.append(native[repeat][name]['rows'][s['tick']]['rotation']+s['mass_inertia']+s['angular_velocity']);expected.append([s[n][j][i] for n in ['mass_orientation_columns','world_inertia_columns'] for i in range(3) for j in range(3)]+s['momentum'])
   repeats.append((inputrows,expected))
  assert repeats[0]==repeats[1] and len(expected)==2220
  data=''.join(' '.join(map(repr,v))+'\n' for v in inputrows).encode();actual=[[bits(x) for x in l.split()] for l in subprocess.check_output([str(exe)],input=data).decode().splitlines()];assert actual==[list(map(bits,v)) for v in expected]
  summary['arithmetic']['public-'+opt]={'float_fields_exact':46620}
 # Confirm the published exact patch reconstructs every after file byte for byte.
 base=json.loads((here/'base.json').read_text());subprocess.run(['git','init','-q',str(tmp/'patch')],check=True)
 for name,meta in base['files'].items():
  if meta['before_sha256']:
   src=here/'before'/name;assert hashlib.sha256(src.read_bytes()).hexdigest()==meta['before_sha256'];dest=tmp/'patch'/name;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(src.read_bytes())
 subprocess.run(['git','-C',str(tmp/'patch'),'apply','--whitespace=nowarn',str(here.parent/'native-angular-v49.patch')],check=True)
 for name,meta in base['files'].items():assert (tmp/'patch'/name).read_bytes()==(here/name).read_bytes() and hashlib.sha256((here/name).read_bytes()).hexdigest()==meta['after_sha256']
for mode in ['control','momentum','actor','both']:
 runs=[load(m/'mac-public'/f'{mode}-run{r}-mac.jsonl','mac-jolt-metal') for r in range(2)];assert runs[0]==runs[1]
 report=words.audit(m/'native-angular/angular-run0-windows.jsonl',m/'mac-public'/f'{mode}-run0-mac.jsonl')
 if mode=='both':assert len(report['cases_exact'])==16 and report['cases_remaining']==['body-slide-x','dirtdevil/spin-mixed'] and len(report['decimal_only_differences'])==46;summary['parity']['spin18']=report
probe_hash=hashlib.sha256((root/'bin/Mac/SpringProbe').read_bytes()).hexdigest()
assert json.loads((m/'mac-public/capture-manifest.json').read_text())['probe_sha256']==probe_hash
cases=0
for scope in ['native24','long72','old6','isolation00','isolation01','isolation02','isolation03']:
 ref=m/'references'/f'{scope}-windows.jsonl';old=load(m/'v48-controls'/f'{scope}-mac.jsonl','mac-jolt-metal');candidate=None
 for mode in ['control','candidate']:
  runs=[load(m/'gates'/f'{scope}-{mode}-run{r}-mac.jsonl','mac-jolt-metal') for r in range(2)];assert runs[0]==runs[1]
  if mode=='control':assert runs[0]==old
  else:candidate=runs[0]
  report=words.audit(ref,m/'gates'/f'{scope}-{mode}-run0-mac.jsonl')
  if scope!='native24':assert not report['cases_remaining'];summary['parity'][scope+'-'+mode]={'exact':len(report['cases_exact'])}
  elif mode=='candidate':assert len(report['cases_exact'])==22 and report['cases_remaining']==['body-slide-x','dirtdevil/spin-mixed'];summary['parity']['native24']=report
 cases+=len(candidate)
assert cases==291 and json.loads((m/'gates/capture-manifest.json').read_text())['probe_sha256']==probe_hash
# Independent original and Mac COM observers preserve DirtDevil's full physical history.
for r in range(2):
 assert load(m/'dirt-native'/f'physical-run{r}.jsonl','windows-physx-d3d9')['dirtdevil/spin-mixed']==native[0]['dirtdevil/spin-mixed']
 assert load(m/'dirt-mac'/f'trace-run{r}-mac.jsonl','mac-jolt-metal')['dirtdevil/spin-mixed']==load(m/'mac-public'/f'both-run{r}-mac.jsonl','mac-jolt-metal')['dirtdevil/spin-mixed']
assert '100% tests passed, 0 tests failed out of 53' in (root/'evidence/ctest.log').read_text()
for repeat in range(2):
 assert load(m/'quaternion'/f'physical-run{repeat}.jsonl','windows-physx-d3d9')['marauder/spin-mixed']==native[0]['marauder/spin-mixed']
 quat=[r['returned_quaternion']['words'] for r in read(m/'quaternion'/f'private-run{repeat}.jsonl') if r.get('function')=='backend-quaternion-return']
 assert quat==[list(map(bits,r['rotation'])) for r in native[0]['marauder/spin-mixed']['rows']]
native_center=[w(x) for x in next(r for r in read(m/'dirt-native/private-run0.jsonl') if r.get('function')=='integrate-entry')['body']['words'][25:28]]
mac_center=json.loads((m/'dirt-mac/readings-run0.json').read_text())[0]
assert bits(native_center[0])==0x3dcccccd and bits(mac_center['local_center'][0])==0x3dccccce
summary['dirt_com_lead']={'native':native_center,'mac':mac_center['center_position'],'corrected':False}
(root/'verification-result.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps({'arithmetic':summary['arithmetic'],'expanded_cases_exact':22,'expanded_cases_total':24,'goal_complete':False}))
