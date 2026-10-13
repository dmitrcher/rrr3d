"""Actual captured Native angle arguments and outputs, production header, fresh Windows checkpoint."""
from pathlib import Path
import argparse,json,subprocess,os,struct,gzip,tempfile,hashlib
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args()
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
flt=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
base=json.loads((root/'base.json').read_text());inc=root/'after/adapter/src/Rock3dEngine/header/physics'
for f in base['files']:assert hashlib.sha256((root/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
fixtures=json.loads((root/'evidence/expanded-same-input-proof.json').read_text())['fixtures'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(root/'profile.cfg')
report={'arithmetic':[],'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v100-') as tmp:
 tmp=Path(tmp)
 data='\n'.join(' '.join(format(flt(w),'.9g') for w in f['actual_native_input_words']) for f in fixtures)+'\n'
 for opt in ['O0','O2']:
  exe=tmp/('angle-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(inc),'-I'+str(root/'context'),str(root/'angle-probe.cpp'),'-o',str(exe)],check=True)
  got=[list(map(float,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode(),env=env).decode().splitlines()];assert len(got)==len(fixtures)==52;assert all(bits(a[0])==f['actual_native_angle_word'] and bits(a[1])==f['actual_native_cosine_word'] for a,f in zip(got,fixtures))
  control=dict(env);control.pop('RRR3D_EXPERIMENT_NATIVE_TIRE_ANGLE_V100');old=[list(map(float,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode(),env=control).decode().splitlines()];mismatches=sum(bits(a[0])!=f['actual_native_angle_word'] for a,f in zip(old,fixtures));assert mismatches==6
  report['arithmetic'].append({'optimization':opt,'actual_Native_angles_exact':52,'actual_Native_arguments_exact':52,'default_off_retains_old_six_word_mismatches':True,'unmeasured_input_domain_certified':False})
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
