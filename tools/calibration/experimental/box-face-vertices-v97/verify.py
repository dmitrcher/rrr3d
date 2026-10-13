"""Actual exported Native box face indices and optional fresh-process checkpoint."""
from pathlib import Path
import argparse,json,subprocess,os,struct,gzip,tempfile,hashlib
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args()
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
base=json.loads((root/'base.json').read_text());inc=root/'after/adapter/src/Rock3dEngine/header/physics'
for f in base['files']:assert hashlib.sha256((root/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
fixtures=json.loads((root/'evidence/native-face-vertex-fixtures.json').read_text())['fixtures'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'probe-environment.json').read_text())['environment']);env['RRR3D_CALIBRATION_PROFILE']=str(root/'profile.cfg')
report={'arithmetic':[],'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v97-') as tmp:
 tmp=Path(tmp)
 data='\n'.join(' '.join(map(str,r['half']+[r['axis'],r['sign']])) for r in fixtures)+'\n'
 for opt in ['O0','O2']:
  exe=tmp/('faces-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(inc),str(root/'face-probe.cpp'),'-o',str(exe)],check=True)
  got=[list(map(float,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode(),env=env).decode().splitlines()];assert len(got)==len(fixtures)==18;assert all(list(map(bits,a))==list(map(bits,sum(r['selected'],[]))) for a,r in zip(got,fixtures))
  control=dict(env);control.pop('RRR3D_EXPERIMENT_NATIVE_BOX_FACE_VERTICES_V97');assert subprocess.check_output([str(exe),'--flag'],env=control).strip()==b'0';assert subprocess.check_output([str(exe),'--flag'],env=env).strip()==b'1'
  report['arithmetic'].append({'optimization':opt,'actual_native_corner_words_exact':216,'fixtures':18,'all_six_static_table_rows_bound':True,'default_off_flag_verified':True})
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
  assert actual==expected;report['fresh_probe_checkpoint']={'states':sum(map(len,actual.values())),'cases':len(actual),'all_observed_float_words_exact_saved_v97':True,'probe_sha256':hashlib.sha256(args.probe.read_bytes()).hexdigest()}
print(json.dumps(report,indent=2))
