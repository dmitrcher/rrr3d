"""Verify production deferred pair lifetime against actual Native events and fresh probe states."""
from pathlib import Path
import argparse,json,subprocess,os,struct,gzip,tempfile,hashlib
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args();bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
manifest=json.loads((root/'base.json').read_text())
for f in manifest['files']:assert hashlib.sha256((root/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
proof=json.loads((root/'evidence/native-pair-reset-proof.json').read_text());expected=proof['proof'][0]['actual_all_objects_clear_call_206f41'];assert all(r['actual_all_objects_clear_call_206f41']==expected for r in proof['proof'])
report={'scope':'Actual Native deferred manager events and fresh isolated numerical probe states. Overall game parity incomplete.','arithmetic':[],'goal_complete':False}
def physical(r):
 r=dict(r)
 for k in ['position','velocity','rotation','angular_momentum']:r[k]=list(map(bits,r[k]))
 floats={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'}
 r['wheels']=[{k:bits(v) if k in floats else v for k,v in w.items()} for w in r['wheels']];return r
def samples(path):
 out={}
 with (gzip.open(path,'rt') if str(path).endswith('.gz') else path.open()) as stream:
  for r in map(json.loads,stream):
   if r.get('event')=='calibration.sample':out.setdefault(r['detail'],[]).append(physical(r['sample']))
 return out
with tempfile.TemporaryDirectory(prefix='motorrock-v106-') as temp:
 temp=Path(temp)
 for opt in ['O0','O2']:
  exe=temp/('pair-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(root/'shadow'),'-I'+str(root/'after/adapter/src/Rock3dEngine/header/physics'),str(root/'shadow/pair-geometry-proof.cpp'),'-o',str(exe)],check=True)
  actual=[list(map(int,line.split())) for line in subprocess.check_output([str(exe)],input=(root/'pair-shadow-input.txt').read_bytes()).decode().splitlines()];assert actual==expected
  report['arithmetic'].append({'optimization':opt,'independent_original_Native_events_exact':actual,'shadow_poses_never_used_by_runtime':True})
 if args.probe:
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(root/'profile.cfg')
  assert all(flag.encode() in args.probe.read_bytes() for flag in ['RRR3D_EXPERIMENT_NATIVE_TIRE_OBJECT_LIFETIME_V106','RRR3D_EXPERIMENT_NATIVE_WHEEL_PAIR_RESET_V106'])
  reference=samples(root/'evidence/windows-checkpoint.jsonl.gz');path=temp/'native-checkpoints.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'list:'+','.join(reference)],env=env,capture_output=True,check=True,timeout=180);actual=samples(path);assert actual==reference
  report['fresh_original_windows_float32_states']={n:len(rows) for n,rows in actual.items()};report['total_original_windows_states']=sum(map(len,actual.values()))
  reference=samples(root/'evidence/retained-roll-contact-v105.jsonl.gz');path=temp/'retained-roll.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'list:body-wheel-contact-roll'],env=env,capture_output=True,check=True,timeout=180);actual=samples(path);assert actual==reference
  report['retained_roll_contact_states']=sum(map(len,actual.values()));report['retained_roll_matches_windows_claimed']=False;report['probe_sha256']=hashlib.sha256(args.probe.read_bytes()).hexdigest()
print(json.dumps(report,indent=2))
