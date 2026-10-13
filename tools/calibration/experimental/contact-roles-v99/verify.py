"""Actual original Windows cache inputs through the exported production header."""
from pathlib import Path
import argparse,json,subprocess,os,struct,gzip,tempfile,hashlib
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args()
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
flt=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
base=json.loads((root/'base.json').read_text());inc=root/'after/adapter/src/Rock3dEngine/header/physics'
for f in base['files']:assert hashlib.sha256((root/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
fixtures=json.loads((root/'evidence/creation-order-v99-cache-arithmetic-proof.json').read_text())['fixtures'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(root/'profile.cfg')
report={'arithmetic':[],'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v99-') as tmp:
 tmp=Path(tmp)
 data='\n'.join(' '.join(format(flt(w),'.9g') for w in f['actual_native_input_words']) for f in fixtures)+'\n'
 for opt in ['O0','O2']:
  exe=tmp/('cache-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(inc),'-I'+str(root/'context'),str(root/'cache-probe.cpp'),'-o',str(exe)],check=True)
  got=[list(map(bits,map(float,l.split()))) for l in subprocess.check_output([str(exe)],input=data.encode(),env=env).decode().splitlines()];assert got==[f['actual_native_output_words'] for f in fixtures]
  old=[list(map(bits,map(float,l.split()))) for l in subprocess.check_output([str(exe),'old'],input=data.encode(),env=env).decode().splitlines()];mismatches=sum(a!=b for row,f in zip(old,fixtures) for a,b in zip(row,f['actual_native_output_words']));assert mismatches==8
  report['arithmetic'].append({'optimization':opt,'actual_native_cache_and_lever_words_exact':72,'old_dynamic_body0_mismatching_words':8,'unmeasured_input_domain_certified':False})
 if args.probe:
  def physical(r):
   r=dict(r)
   for k in ['position','velocity','rotation','angular_momentum']:r[k]=list(map(bits,r[k]))
   fields={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'};r['wheels']=[{k:bits(v) if k in fields else v for k,v in w.items()} for w in r['wheels']];return r
  expected={}
  for r in map(json.loads,gzip.open(root/'evidence/checkpoint-expected.jsonl.gz','rt')):expected.setdefault(r['detail'],[]).append(physical(r['sample']))
  path=tmp/'checkpoint.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'list:'+','.join(expected)],env=env,check=True,capture_output=True,timeout=180);actual={n:[] for n in expected}
  for r in map(json.loads,path.open()):
   if r.get('event')=='calibration.sample':actual[r['detail']].append(physical(r['sample']))
  assert actual==expected;report['fresh_probe_checkpoint']={'states':sum(map(len,actual.values())),'cases':len(actual),'all_observed_float_words_exact_original_windows':True,'probe_sha256':hashlib.sha256(args.probe.read_bytes()).hexdigest()}
print(json.dumps(report,indent=2))
