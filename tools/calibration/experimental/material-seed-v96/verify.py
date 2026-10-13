"""Portable arithmetic proof and optional fresh process trajectory checkpoint."""
from pathlib import Path
import argparse,json,subprocess,os,struct,gzip,tempfile,hashlib
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args()
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
base=json.loads((root/'base.json').read_text());inc=root/'after/adapter/src/Rock3dEngine/header/physics'
for f in base['files']:
 assert hashlib.sha256((root/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
fixtures=json.loads((root/'evidence/native-seed-fixtures.json').read_text());stage=json.loads((root/'evidence/material-stage-same-input.json').read_text());env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(root/'profile.cfg')
report={'native_seed_fixtures':len(fixtures),'arithmetic':[],'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v96-') as tmp:
 tmp=Path(tmp)
 for opt in ['O0','O2']:
  for name in ['seed','stage']:
   exe=tmp/(name+'-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(inc),'-I'+str(root/'context'),str(root/(name+'-probe.cpp')),'-o',str(exe)],check=True)
   if name=='seed':
    data='\n'.join(' '.join(format(x,'.9g') for x in f['actor']+f['material']) for f in fixtures)+'\n';got=[list(map(float,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode(),env=env).decode().splitlines()];assert len(got)==16;assert all(list(map(bits,a))==list(map(bits,f['expected_seed'])) for a,f in zip(got,fixtures))
   else:
    m=stage['mac_input'];data=' '.join(format(x,'.9g') for x in m['physical_actor']+m['api_actor']+m['callback_plane'])+'\n';got=list(map(float,subprocess.check_output([str(exe)],input=data.encode(),env=env).split()));keys=['raw_local','raw_world','material_world','seed','callback_local0','callback_local1','solver_quaternion'];expected=sum((stage['runs']['O2'][k] for k in keys),[])+stage['runs']['O2']['normal'];assert list(map(bits,got))==list(map(bits,expected));control=dict(env);control.pop('RRR3D_EXPERIMENT_NATIVE_MATERIAL_SEED_V96');old=list(map(float,subprocess.check_output([str(exe)],input=data.encode(),env=control).split()));assert list(map(bits,old[12:16]))==list(map(bits,m['seed']));assert list(map(bits,old[24:28]))==list(map(bits,m['solver_quaternion']));assert bits(old[12])!=bits(got[12])
  report['arithmetic'].append({'optimization':opt,'native_seed64_words_exact':True,'all31_composed_stage_words_exact':True,'default_off_original_actual_seed_solver_exact':True})
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
  assert actual==expected;report['fresh_probe_checkpoint']={'states':sum(map(len,actual.values())),'cases':len(actual),'all_observed_float_words_exact_saved_v96':True,'probe_sha256':hashlib.sha256(args.probe.read_bytes()).hexdigest()}
print(json.dumps(report,indent=2))
