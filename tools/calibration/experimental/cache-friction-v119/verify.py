from pathlib import Path
import json,struct,gzip,hashlib,argparse,subprocess,tempfile,os
r=Path(__file__).resolve().parent;parser=argparse.ArgumentParser();parser.add_argument('--probe',type=Path);args=parser.parse_args()
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
with tempfile.TemporaryDirectory(prefix='motorrock-v119-') as tmp:
 tmp=Path(tmp);fixtures=json.loads((r/'evidence/cache-proof.json').read_text())['fixtures'];massfixtures=[]
 for n in ['mass-proof.json','mass-next-proof.json']:massfixtures+=json.loads((r/'evidence'/n).read_text())['fixtures']
 for kind,rows in [('cache',fixtures),('mass',massfixtures)]:
  for opt in ['O0','O2']:
   exe=tmp/(kind+'-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(r/'after/adapter/src/Rock3dEngine/header/physics'),'-I'+str(r/'evidence/dependencies'),str(r/('verify-'+kind+'.cpp')),'-o',str(exe)],check=True);data='\n'.join(' '.join(map(str,x['input'])) for x in rows)+'\n';env=dict(os.environ)
   if kind=='cache':env['RRR3D_EXPERIMENT_NATIVE_CACHE_WORLD_POINT_V119']='1'
   out=[list(map(int,line.split())) for line in subprocess.check_output([str(exe)],input=data,text=True,env=env).splitlines()];actual=out if kind=='cache' else [x[1::2] for x in out];assert actual==[x['expected'] for x in rows];entry={'kind':kind,'optimization':opt,'original_records_exact':len(actual)}
   if kind=='cache':
    env.pop('RRR3D_EXPERIMENT_NATIVE_CACHE_WORLD_POINT_V119');off=[list(map(int,line.split())) for line in subprocess.check_output([str(exe)],input=data,text=True,env=env).splitlines()];assert off==[x['flag_off_Source_expected'] for x in rows];entry['flag_off_Source_records_preserved']=len(off)
   else:assert [x[::2] for x in out]==[x['XYZ_mass'] for x in rows]
   report['arithmetic'].append(entry)
 if args.probe:
  probe=args.probe.resolve();assert all(k.encode() in probe.read_bytes() for k in json.loads((r/'base.json').read_text())['flags']);env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(r/'profile.cfg')
  ref=samples(r/'evidence/windows-checkpoint.jsonl.gz',True);path=tmp/'physical.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(ref)],env=env,capture_output=True,check=True,timeout=240);actual=samples(path,True);assert actual==ref;report['fresh_original_Native_equal_states']={n:len(rows) for n,rows in actual.items()};report['full_Native_equal_states']=sum(map(len,actual.values()))
  for mode,name,disable in [('off','v118-control.jsonl.gz',json.loads((r/'base.json').read_text())['flags']),('cache-only','cache-only-control.jsonl.gz',['RRR3D_EXPERIMENT_NATIVE_FRICTION_MASS_ORDER_V119'])]:
   controlenv=dict(env)
   for flag in disable:controlenv.pop(flag)
   ref=samples(r/'evidence'/name,False);path=tmp/(mode+'.jsonl');subprocess.run([str(probe),str(path),'list:'+','.join(ref)],env=controlenv,capture_output=True,check=True,timeout=120);assert samples(path,False)==ref;report[mode+'_preserved_decimal_states']=sum(map(len,ref.values()))
  report['probe_sha256']=hashlib.sha256(probe.read_bytes()).hexdigest()
print(json.dumps(report,indent=2))
