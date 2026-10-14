"""Verify original static-box preparation, material frame, coupled solve and fresh observable checkpoints."""
from pathlib import Path
import argparse,json,os,gzip,tempfile,subprocess,hashlib,struct
r=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args();bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
for f in json.loads((r/'base.json').read_text())['files']:assert hashlib.sha256((r/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
preparation=json.loads((r/'preparation-fixtures.json').read_text());frames=json.loads((r/'frame-fixtures.json').read_text());kernels=json.loads((r/'static-kernel-fixtures.json').read_text());report={'arithmetic':[],'goal_complete':False}
def physical(row):
 row=dict(row)
 for k in ['position','velocity','rotation','angular_momentum']:row[k]=list(map(bits,row[k]))
 f={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'};row['wheels']=[{k:bits(v) if k in f else v for k,v in wheel.items()} for wheel in row['wheels']];return row
def samples(path):
 result={}
 with (gzip.open(path,'rt') if str(path).endswith('.gz') else path.open()) as src:
  for row in map(json.loads,src):
   if row.get('event')=='calibration.sample':result.setdefault(row['detail'],[]).append(physical(row['sample']))
 return result
with tempfile.TemporaryDirectory(prefix='motorrock-v111-') as tmp:
 tmp=Path(tmp)
 env=dict(os.environ);env.update(json.loads((r/'probe-environment.json').read_text()))
 def run(name,opt,data):
  exe=tmp/(name+'-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(r/'shadow'),'-I'+str(r/'after/sdk/Jolt/Physics/Constraints/ConstraintPart'),str(r/(name+'.cpp')),'-o',str(exe)],check=True)
  output=subprocess.check_output([str(exe)],input='\n'.join(' '.join(map(str,x)) for x in data)+'\n',text=True,env=env);return [list(map(int,line.split())) for line in output.splitlines()]
 for opt in ['O0','O2']:
  for fixture in preparation:
   l=run('verify-lever',opt,[x[:10] for x in fixture['input']]);m=run('verify-mass',opt,fixture['input'])
   for i,e in enumerate(fixture['expected']):assert l[i][:9]==e['physical_matrix'] and l[i][9:12]==e['local_point'] and l[i][12:15]==e['solver_lever'] and m[i][:9]==e['tensor'] and m[i][18]==e['mass']
  for fixture in frames:
   actual=run('verify-frame',opt,[fixture['input']])[1][:4];assert all(a==b or a&0x7fffffff==0 and b&0x7fffffff==0 for a,b in zip(actual,fixture['Native_frame_words']))
  for fixture in kernels:
   out=run('verify-static-kernel',opt,fixture['input'])
   for c in fixture['body_checks']:assert out[c['row']][:6]==c['expected']
   for c in fixture['lambda_checks']:assert out[c['row']][6:10]==c['expected']
  report['arithmetic'].append({'optimization':opt,'preparation_Native_repeats':2,'frame_Native_captures':len(frames),'kernel_Native_captures':len(kernels),'within_iteration_body_transitions_exact':sum(len(x['body_checks']) for x in kernels),'retained_lambda_and_friction_vectors_exact':sum(len(x['lambda_checks']) for x in kernels)})
 if args.probe:
  assert b'RRR3D_EXPERIMENT_NATIVE_STATIC_BOX_SOLVER_V111' in args.probe.read_bytes();env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(r/'profile.cfg');reference=samples(r/'evidence/windows-checkpoint.jsonl.gz');path=tmp/'physical.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'list:'+','.join(reference)],env=env,capture_output=True,check=True,timeout=180);actual=samples(path);assert actual==reference;report['fresh_original_Native_equal_states']={n:len(rows) for n,rows in actual.items()};report['full_Native_equal_states']=sum(map(len,actual.values()))
  reference=samples(r/'evidence/retained-roll-contact-v110.jsonl.gz');path=tmp/'retained-roll.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'body-wheel-contact-roll'],env=env,capture_output=True,check=True,timeout=180);assert samples(path)==reference;report['retained_roll_contact_states']=180;report['retained_roll_matches_windows_claimed']=False;report['probe_sha256']=hashlib.sha256(args.probe.read_bytes()).hexdigest()
print(json.dumps(report,indent=2))
