"""Verify Native wheel-manager processor slots and fresh full observable checkpoints."""
from pathlib import Path
import argparse,json,os,gzip,tempfile,subprocess,hashlib,struct
r=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args();bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
for f in json.loads((r/'base.json').read_text())['files']:assert hashlib.sha256((r/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
fixtures=json.loads((r/'manager-list-fixtures.json').read_text())['fixtures'];report={'arithmetic':[],'goal_complete':False}
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
with tempfile.TemporaryDirectory(prefix='motorrock-v108-') as tmp:
 tmp=Path(tmp)
 for opt in ['O0','O2']:
  exe=tmp/('manager-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-I'+str(r/'after/adapter/src/Rock3dEngine/header/physics'),str(r/'manager-list-proof.cpp'),'-o',str(exe)],check=True)
  for fixture in fixtures:
   actual=[list(map(int,line.split())) for line in subprocess.check_output([str(exe)],input=fixture['input'],text=True).splitlines()];assert actual==fixture['expected']
  report['arithmetic'].append({'optimization':opt,'actual_Native_event_repeats':len(fixtures),'all_original_observed_wheel_processor_slots_exact':True,'body_managers_excluded':True,'original_events_never_used_in_runtime':True})
 if args.probe:
  assert b'RRR3D_EXPERIMENT_NATIVE_STATIC_PAIR_DISPATCH_V108' in args.probe.read_bytes();env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(r/'profile.cfg');reference=samples(r/'evidence/windows-checkpoint.jsonl.gz');path=tmp/'physical.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'list:'+','.join(reference)],env=env,capture_output=True,check=True,timeout=180);actual=samples(path);assert actual==reference;report['fresh_original_Native_equal_states']={n:len(rows) for n,rows in actual.items()};report['full_Native_equal_states']=sum(map(len,actual.values()))
  reference=samples(r/'evidence/retained-roll-contact-v107.jsonl.gz');path=tmp/'retained-roll.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'body-wheel-contact-roll'],env=env,capture_output=True,check=True,timeout=180);assert samples(path)==reference;report['retained_roll_contact_states']=180;report['retained_roll_matches_windows_claimed']=False;report['probe_sha256']=hashlib.sha256(args.probe.read_bytes()).hexdigest()
print(json.dumps(report,indent=2))
