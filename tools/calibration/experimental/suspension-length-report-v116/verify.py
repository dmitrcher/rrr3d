from pathlib import Path
import json,struct,gzip,hashlib,argparse,subprocess,tempfile,os
r=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--probe',type=Path,required=True);args=p.parse_args()
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
probe=args.probe.resolve();assert b'RRR3D_EXPERIMENT_NATIVE_SUSPENSION_LENGTH_REPORT_V116' in probe.read_bytes()
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((r/'probe-environment.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(r/'profile.cfg')
fixtures={(x['case'],x['tick'],x['wheel']):x for x in map(json.loads,gzip.open(r/'evidence/native-api-fixtures.jsonl.gz','rt'))};report={'goal_complete':False}
with tempfile.TemporaryDirectory(prefix='motorrock-v116-') as tmp:
 tmp=Path(tmp);ref=samples(r/'evidence/windows-checkpoint.jsonl.gz',True);path=tmp/'physical.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(ref)],env=env,capture_output=True,check=True,timeout=180)
 actual=samples(path,True);assert actual==ref;report['fresh_original_Native_equal_states']={n:len(rows) for n,rows in actual.items()};report['full_Native_equal_states']=sum(map(len,actual.values()))
 control=samples(r/'evidence/v115-length-control.jsonl.gz',False);off=dict(env);off.pop('RRR3D_EXPERIMENT_NATIVE_SUSPENSION_LENGTH_REPORT_V116');path=tmp/'control.jsonl';subprocess.run([str(probe),str(path),'list:'+','.join(control)],env=off,capture_output=True,check=True,timeout=180);assert samples(path,False)==control;report['flag_off_preserves_v115_states']=sum(map(len,control.values()))
 checked=0
 for name in control:
  trace=tmp/(name+'-inputs.jsonl');path=tmp/(name+'.jsonl');current=dict(env);current['RRR3D_WHEEL_LENGTH_TRACE_FILE_V116']=str(trace);subprocess.run([str(probe),str(path),'list:'+name],env=current,capture_output=True,check=True,timeout=180)
  rows={}
  for x in map(json.loads,trace.open()):
   k=(x['physical_tick'],x['wheel'])
   if k in rows:assert rows[k]==x
   rows[k]=x
  assert set(rows)=={(t,w) for t in range(600) for w in range(4)}
  for (tick,wheel),x in rows.items():
   n=fixtures[(name,tick,wheel)];assert x['has_contact']==n['has_contact'] and x['radius_word']==n['radius_word']
   if x['has_contact']:assert x['distance_word']==n['contact_position_word']
   assert x['reported_length']==n['derived_double'];checked+=1
 report['actual_Native_API_input_and_double_result_tuples']=checked;report['probe_sha256']=hashlib.sha256(probe.read_bytes()).hexdigest()
print(json.dumps(report,indent=2))
