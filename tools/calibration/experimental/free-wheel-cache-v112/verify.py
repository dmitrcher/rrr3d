"""Check original cached ray arithmetic and fresh original observable histories."""
from pathlib import Path
import json,struct,gzip,hashlib,argparse,subprocess,tempfile,os
r=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args()
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
for f in json.loads((r/'base.json').read_text())['files']:
 assert hashlib.sha256((r/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
def physical(row):
 row=dict(row)
 for k in ['position','velocity','rotation','angular_momentum']:row[k]=list(map(bits,row[k]))
 fields={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'}
 row['wheels']=[{k:bits(v) if k in fields else v for k,v in x.items()} for x in row['wheels']];return row
def samples(path):
 result={}
 with gzip.open(path,'rt') as f:
  for x in map(json.loads,f):
   if x['event']=='calibration.sample':result.setdefault(x['detail'],[]).append(physical(x['sample']))
 return result
fixtures=json.loads((r/'cache-fixtures.json').read_text());report={'arithmetic':[],'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v112-') as tmp:
 tmp=Path(tmp)
 for opt in ['O0','O2']:
  exe=tmp/opt;subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(r/'after/adapter/src/Rock3dEngine/header/physics'),str(r/'verify-cache.cpp'),'-o',str(exe)],check=True)
  result=list(map(int,subprocess.check_output([str(exe)],input='\n'.join(' '.join(map(str,x['input'])) for x in fixtures)+'\n',text=True).split()));assert result==[x['expected'] for x in fixtures]
  report['arithmetic'].append({'optimization':opt,'original_branch_results_exact':len(result)})
 if args.probe:
  probe=args.probe.resolve();assert b'RRR3D_EXPERIMENT_NATIVE_FREE_WHEEL_CACHE_V112' in probe.read_bytes()
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(r/'profile.cfg')
  ref=samples(r/'evidence/windows-checkpoint.jsonl.gz');path=tmp/'physical.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(ref)],env=env,capture_output=True,check=True,timeout=180)
  with gzip.open(str(path)+'.gz','wb') as dst:dst.write(path.read_bytes())
  actual=samples(Path(str(path)+'.gz'));assert actual==ref;report['fresh_original_Native_equal_states']={n:len(rows) for n,rows in actual.items()};report['full_Native_equal_states']=sum(map(len,actual.values()))
  ref=samples(r/'evidence/v111-podushka.jsonl.gz');env.pop('RRR3D_EXPERIMENT_NATIVE_FREE_WHEEL_CACHE_V112');path=tmp/'control.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(ref)],env=env,capture_output=True,check=True,timeout=180)
  with gzip.open(str(path)+'.gz','wb') as dst:dst.write(path.read_bytes())
  assert samples(Path(str(path)+'.gz'))==ref;report['flag_off_preserves_v111_podushka_states']=600;report['probe_sha256']=hashlib.sha256(probe.read_bytes()).hexdigest()
print(json.dumps(report,indent=2))
