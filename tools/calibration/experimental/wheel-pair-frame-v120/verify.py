from pathlib import Path
import json,struct,gzip,hashlib,argparse,subprocess,tempfile,os
r=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args()
for row in json.loads((r/'base.json').read_text())['files']:
 for kind in ['before','after']:assert hashlib.sha256((r/kind/row['location']/row['path']).read_bytes()).hexdigest()==row[kind+'_sha256']
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def physical(row):
 row=dict(row)
 for k in ['position','velocity','rotation','angular_momentum']:row[k]=list(map(bits,row[k]))
 fields={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'};row['wheels']=[{k:bits(v) if k in fields else v for k,v in x.items()} for x in row['wheels']];return row
def samples(path,canonical):
 result={};opener=gzip.open(path,'rt') if str(path).endswith('.gz') else path.open()
 with opener as f:
  for x in map(json.loads,f):
   if x['event']=='calibration.sample':result.setdefault(x['detail'],[]).append(physical(x['sample']) if canonical else x['sample'])
 return result
report={'arithmetic':[],'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v120-') as tmp:
 tmp=Path(tmp);fixtures=json.loads((r/'evidence/both-frame-proof.json').read_text())['fixtures']
 for opt in ['O0','O2']:
  exe=tmp/opt;subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(r/'after/sdk/Jolt/Physics/Constraints/ConstraintPart'),'-I'+str(r/'evidence/dependencies'),str(r/'verify-both-frame.cpp'),'-o',str(exe)],check=True);data='\n'.join(' '.join(map(str,x['input'])) for x in fixtures)+'\n';actual=[list(map(int,l.split())) for l in subprocess.check_output([str(exe)],input=data,text=True).splitlines()];assert actual==[x['expected'] for x in fixtures];report['arithmetic'].append({'kind':'both-body-frame','optimization':opt,'original_records_exact':len(actual)})
 if args.probe:
  probe=args.probe.resolve();assert b'RRR3D_EXPERIMENT_NATIVE_WHEEL_PAIR_FRAME_V120' in probe.read_bytes();env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(r/'profile.cfg');ref=samples(r/'evidence/windows-checkpoint.jsonl.gz',True);path=tmp/'physical.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(ref)],env=env,capture_output=True,check=True,timeout=240);actual=samples(path,True);assert actual==ref;report['fresh_original_Native_equal_states']={n:len(rows) for n,rows in actual.items()};report['full_Native_equal_states']=sum(map(len,actual.values()))
  ref=samples(r/'evidence/v119-control.jsonl.gz',False);env.pop('RRR3D_EXPERIMENT_NATIVE_WHEEL_PAIR_FRAME_V120');path=tmp/'control.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(ref)],env=env,capture_output=True,check=True,timeout=120);assert samples(path,False)==ref;report['flag_off_preserves_v119_states']=sum(map(len,ref.values()));report['probe_sha256']=hashlib.sha256(probe.read_bytes()).hexdigest()
print(json.dumps(report,indent=2))
