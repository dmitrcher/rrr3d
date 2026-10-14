"""Verify production object and callback helpers against actual Native observations."""
from pathlib import Path
import argparse,json,subprocess,os,struct,gzip,tempfile,hashlib
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path);args=p.parse_args();bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
base=json.loads((root/'base.json').read_text());inc=root/'after/sdk/Jolt/Physics/Vehicle'
for f in base['files']:assert hashlib.sha256((root/'after'/f['location']/f['path']).read_bytes()).hexdigest()==f['after_sha256']
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(root/'profile.cfg');report={'arithmetic':[],'goal_complete':False}
a=json.loads((root/'evidence/native-object-inputs.json').read_text());queries=a['query_inputs'];configs=a['configuration_inputs'];objects=''.join(f"{x['query_mode']} {int(x['spring_handle_present'])} 0\n" for x in queries)+''.join(f"1 1 {x['original_travel_word']}\n" for x in configs);object_expected=[[int(x['actual_group_reset']),0] for x in queries]+[[0,int(x['actual_Native_positive_travel_spring_branch'])] for x in configs]
fixtures=json.loads((root/'evidence/native-callback-inputs.json').read_text())['fixtures'];callback_data='';callback_expected=[]
for f in fixtures:
 callback_data+=str(len(f['frames']))+'\n'
 for x in f['frames']:callback_data+=' '.join(str(int(v)) for v in x['current_query_wheels'])+'\n';callback_expected.append(x['actual_Native_prepare_order'])
with tempfile.TemporaryDirectory(prefix='motorrock-v105-') as tmp:
 tmp=Path(tmp)
 for opt in ['O0','O2']:
  for name,data,expected in [('object',objects,object_expected),('callback',callback_data,callback_expected)]:
   exe=tmp/(name+'-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-I'+str(inc),str(root/(name+'-probe.cpp')),'-o',str(exe)],check=True);actual=[list(map(int,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode()).decode().splitlines()];assert actual==expected
  report['arithmetic'].append({'optimization':opt,'actual_Native_query_inputs_exact':len(queries),'actual_Native_positive_travel_branch_hits_exact':len(configs),'actual_Native_callback_prepare_frames_exact':len(callback_expected),'zero_travel_and_arbitrary_callback_domains_certified':False})
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
 if args.probe:
  expected=[physical(x['sample']) for x in map(json.loads,gzip.open(root/'evidence/retained-roll-contact-v102.jsonl.gz','rt'))];path=tmp/'retained-roll-contact.jsonl';subprocess.run([str(args.probe.resolve()),str(path),'list:body-wheel-contact-roll'],env=env,check=True,capture_output=True,timeout=180);actual=[physical(x['sample']) for x in map(json.loads,path.open()) if x.get('event')=='calibration.sample'];assert actual==expected and len(actual)==180;report['fresh_rejected_fixture_retained_v102']={'states':180,'all_observable_float_words_exact_accepted_v102':True,'all_words_exact_Windows_claimed':False}
print(json.dumps(report,indent=2))
