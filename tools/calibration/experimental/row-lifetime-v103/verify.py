"""Bind production query branch to actual Native mode/handle/reset observations."""
from pathlib import Path
import argparse,json,subprocess,os,struct,gzip,tempfile,hashlib
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args();bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
base=json.loads((root/'base.json').read_text());inc=root/'after/sdk/Jolt/Physics/Vehicle'
for f in base['files']:assert hashlib.sha256((root/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
observations=json.loads((root/'evidence/same-input-query-proof.json').read_text())['actual_query_inputs'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(root/'profile.cfg');report={'arithmetic':[],'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v103-') as tmp:
 tmp=Path(tmp);data=''.join(f"{r['query_mode']} {int(r['spring_handle_present'])}\n" for r in observations)
 for opt in ['O0','O2']:
  exe=tmp/('query-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-I'+str(inc),str(root/'query-probe.cpp'),'-o',str(exe)],check=True)
  got=list(map(int,subprocess.check_output([str(exe)],input=data.encode()).decode().splitlines()));assert got==[int(r['actual_group_reset']) for r in observations]
  report['arithmetic'].append({'optimization':opt,'actual_Native_query_inputs_exact':len(got),'actual_actor_wheel_reset_groups':sum(got),'unmeasured_lifecycle_domains_certified':False})
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
