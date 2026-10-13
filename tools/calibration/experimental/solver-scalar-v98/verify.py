"""Actual Native captured solver quaternion inputs, production header, fresh Windows checkpoint."""
from pathlib import Path
import argparse,json,subprocess,os,struct,gzip,tempfile,hashlib
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args()
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
flt=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
base=json.loads((root/'base.json').read_text());inc=root/'after/adapter/src/Rock3dEngine/header/physics'
for f in base['files']:assert hashlib.sha256((root/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
fixtures=json.loads((root/'evidence/same-input-arithmetic-proof.json').read_text())['fixtures'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(root/'profile.cfg')
report={'arithmetic':[],'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v98-') as tmp:
 tmp=Path(tmp)
 data='\n'.join(' '.join(format(flt(v),'.9g') for v in r['actual_actor_words']+r['actual_local_words']) for r in fixtures)+'\n'
 for opt in ['O0','O2']:
  exe=tmp/('compose-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(inc),'-I'+str(root/'context'),str(root/'compose-probe.cpp'),'-o',str(exe)],check=True)
  got=[list(map(float,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode(),env=env).decode().splitlines()];assert len(got)==len(fixtures)==4;assert all(list(map(bits,a))==r['native_solver_words'] for a,r in zip(got,fixtures))
  control=dict(env);control.pop('RRR3D_EXPERIMENT_NATIVE_SOLVER_SCALAR_V98');old=[list(map(float,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode(),env=control).decode().splitlines()];assert all(list(map(bits,a[:3]))==r['native_solver_words'][:3] and bits(a[3])==bits(r['old_scalar']) and bits(a[3])!=r['native_solver_words'][3] for a,r in zip(old,fixtures))
  report['arithmetic'].append({'optimization':opt,'actual_native_quaternion_words_exact':16,'actual_input_words_bound':32,'fixtures':4,'default_off_preserves_old_four_scalar_mismatches':True,'unmeasured_input_domain_certified':False})
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
