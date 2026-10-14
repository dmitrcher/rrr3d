"""Original Native static-query math, broadphase eligibility and fresh observable checkpoint."""
from pathlib import Path
import argparse,json,gzip,os,subprocess,tempfile,hashlib,struct
r=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args();bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
for f in json.loads((r/'base.json').read_text())['files']:assert hashlib.sha256((r/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
fixtures=json.loads((r/'box-query-fixtures.json').read_text())['fixtures'];inputs=str(len(fixtures))+'\n'+'\n'.join(' '.join(map(str,x['input_words'])) for x in fixtures)+'\n';expected=[x['expected_words'] for x in fixtures];bounds=[[48,0,0,0,0],[49,1,1,0,0]];report={'arithmetic':[],'goal_complete':False}
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
with tempfile.TemporaryDirectory(prefix='motorrock-v107-') as tmp:
 tmp=Path(tmp)
 for opt in ['O0','O2']:
  for name,data,desired in [('verify-box-query',inputs,expected),('candidate-bounds-proof',(r/'candidate-bounds-input.txt').read_text(),bounds)]:
   exe=tmp/(name+'-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(r/'shadow'),'-I'+str(r/'after/adapter/src/Rock3dEngine/header/physics'),str(r/(name+'.cpp')),'-o',str(exe)],check=True);actual=[list(map(int,line.split())) for line in subprocess.check_output([str(exe)],input=data,text=True).splitlines()];assert actual==desired
  report['arithmetic'].append({'optimization':opt,'actual_original_static_query_fixtures':len(fixtures),'Native_query_words_exact':len(fixtures)*17,'actual_original_query_candidate_masks_exact':bounds,'shadow_poses_never_used_in_runtime':True})
 if args.probe:
  assert b'RRR3D_EXPERIMENT_NATIVE_STATIC_WHEEL_BOX_QUERY_V107' in args.probe.read_bytes();env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(r/'profile.cfg')
  reference=samples(r/'evidence/windows-checkpoint.jsonl.gz');path=tmp/'physical.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'list:'+','.join(reference)],env=env,capture_output=True,check=True,timeout=180);actual=samples(path);assert all(len(rows)==600 for rows in actual.values());assert all(actual[n][:len(rows)]==rows for n,rows in reference.items());report['actual_downloaded_original_checkpoint']={n:len(rows) for n,rows in reference.items()};report['full_Native_equal_states']=2400;report['Native_equal_curb_prefix_states']=224;report['full_curb_Native_equality_claimed']=False
  reference=samples(r/'evidence/retained-roll-contact-v106.jsonl.gz');path=tmp/'retained-roll.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'body-wheel-contact-roll'],env=env,capture_output=True,check=True,timeout=180);assert samples(path)==reference;report['retained_roll_contact_states']=180;report['retained_roll_matches_windows_claimed']=False;report['probe_sha256']=hashlib.sha256(args.probe.read_bytes()).hexdigest()
print(json.dumps(report,indent=2))
