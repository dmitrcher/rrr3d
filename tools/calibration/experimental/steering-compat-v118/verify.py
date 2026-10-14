from pathlib import Path
import json,struct,gzip,hashlib,argparse,subprocess,tempfile,os
r=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args()
for f in json.loads((r/'base.json').read_text())['files']:assert hashlib.sha256((r/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def physical(row):
 row=dict(row)
 for k in ['position','velocity','rotation','angular_momentum']:row[k]=list(map(bits,row[k]))
 fields={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'}
 row['wheels']=[{k:bits(v) if k in fields else v for k,v in x.items()} for x in row['wheels']];return row
def samples(path,canonical):
 result={};opener=gzip.open(path,'rt') if str(path).endswith('.gz') else path.open()
 with opener as f:
  for x in map(json.loads,f):
   if x['event']=='calibration.sample':result.setdefault(x['detail'],[]).append(physical(x['sample']) if canonical else x['sample'])
 return result
report={'arithmetic':[],'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v118-') as tmp:
 tmp=Path(tmp)
 for kind,fixturefile,expected in [('frame','frame-proof.json','expected_native_jacobian'),('basis','basis-proof.json','expected')]:
  fixtures=json.loads((r/'evidence'/fixturefile).read_text())['fixtures']
  for opt in ['O0','O2']:
   exe=tmp/(kind+'-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-I'+str(r/'after/adapter/src/Rock3dEngine/header/physics'),'-I'+str(r/'evidence/dependencies'),str(r/('verify-'+kind+'.cpp')),'-o',str(exe)],check=True)
   text='\n'.join(' '.join(map(str,x['input'])) for x in fixtures)+'\n';actual=[list(map(int,line.split())) for line in subprocess.check_output([str(exe)],input=text,text=True).splitlines()];assert actual==[x[expected] for x in fixtures];report['arithmetic'].append({'kind':kind,'optimization':opt,'original_records_exact':len(actual)})
 if args.probe:
  probe=args.probe.resolve();assert b'RRR3D_EXPERIMENT_NATIVE_STEERING_COMPAT_V118' in probe.read_bytes()
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(r/'profile.cfg')
  ref=samples(r/'evidence/windows-checkpoint.jsonl.gz',True);path=tmp/'physical.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(ref)],env=env,capture_output=True,check=True,timeout=180);actual=samples(path,True);assert actual==ref;report['fresh_original_Native_equal_states']={n:len(rows) for n,rows in actual.items()};report['full_Native_equal_states']=sum(map(len,actual.values()))
  ref=samples(r/'evidence/v117-steering-control.jsonl.gz',False);env.pop('RRR3D_EXPERIMENT_NATIVE_STEERING_COMPAT_V118');path=tmp/'control.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(ref)],env=env,capture_output=True,check=True,timeout=180);assert samples(path,False)==ref;report['flag_off_preserves_v117_states']=sum(map(len,ref.values()));report['probe_sha256']=hashlib.sha256(probe.read_bytes()).hexdigest()
print(json.dumps(report,indent=2))
