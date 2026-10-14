from pathlib import Path
import json,gzip,os,subprocess,struct,hashlib,collections
r=Path.cwd();out=r/'outputs/wheel-length-v116/mac';out.mkdir(parents=True,exist_ok=False)
frozen=r/'work/frozen-v116-candidate';manifest=json.loads((frozen/'manifest.json').read_text());probe=frozen/'rrr3d_physics_calibration';assert hashlib.sha256(probe.read_bytes()).hexdigest()==manifest['hashes'][probe.name]
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def physical(x):
 x=dict(x)
 for k in ['position','velocity','rotation','angular_momentum']:x[k]=list(map(bits,x[k]))
 fields={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'}
 x['wheels']=[{k:bits(v) if k in fields else v for k,v in y.items()} for y in x['wheels']];return x
names=['travel-long','radius-small'];old={n:[] for n in names};native={n:[] for n in names}
for path,store in [(r/'outputs/spring-force-v115/broad/candidate-run0.jsonl',old),(r/'outputs/calibration-v14-source-windows.jsonl',native)]:
 with path.open() as f:
  for x in map(json.loads,f):
   if x.get('event')=='calibration.sample' and x['detail'] in store:store[x['detail']].append(x['sample'])
fixtures={}
for x in map(json.loads,gzip.open(r/'work/wheel-length-v116/native-api-fixtures.jsonl.gz','rt')):fixtures[(x['case'],x['tick'],x['wheel'])]=x
reports={};captures=[]
for name in names:
 reports[name]={}
 for mode in ['control','candidate']:
  runs=[];traces=[]
  for rep in range(2):
   tag=f'{name}-{mode}-{rep}';path=out/(tag+'.jsonl');trace=out/(tag+'-inputs.jsonl');env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(manifest['environment'])
   if mode=='control':env.pop('RRR3D_EXPERIMENT_NATIVE_SUSPENSION_LENGTH_REPORT_V116')
   env['RRR3D_WHEEL_LENGTH_TRACE_FILE_V116']=str(trace)
   with (out/(tag+'.log')).open('wb') as log:subprocess.run([str(probe),str(path),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=180)
   actual=[x['sample'] for x in map(json.loads,path.open()) if x.get('event')=='calibration.sample'];assert len(actual)==600
   if mode=='control':assert actual==old[name],'V115 history changed with new flag off'
   else:assert list(map(physical,actual))==list(map(physical,native[name])),'Candidate observable history differs from Original'
   inputs=list(map(json.loads,trace.open()));by={}
   for x in inputs:
    k=(x['physical_tick'],x['wheel'])
    if k in by:assert by[k]==x,'Inconsistent repeated snapshot input'
    by[k]=x
   assert set(by)=={(t,w) for t in range(600) for w in range(4)}
   for (tick,wheel),x in by.items():
    n=fixtures[(name,tick,wheel)];assert x['has_contact']==n['has_contact'] and x['radius_word']==n['radius_word']
    if x['has_contact']:assert x['distance_word']==n['contact_position_word'],(name,tick,wheel,'Source raw API distance differs from Native')
    if mode=='candidate':assert x['reported_length']==n['derived_double'],(name,tick,wheel,'double report differs')
   runs.append(actual);traces.append(by)
   for p in [path,trace]:
    with gzip.open(str(p)+'.gz','wb') as f:f.write(p.read_bytes())
    p.unlink()
   captures.append({'case':name,'mode':mode,'repeat':rep,'states':600,'input_rows':len(inputs),'unique_actual_input_tuples':len(by),'native_distance_and_radius_words_exact':True,'full_Native_history_exact':mode=='candidate','V115_history_preserved':mode=='control'});print(tag,'600 states and2400same-input tuples verified',flush=True)
  assert runs[0]==runs[1] and traces[0]==traces[1]
  reports[name][mode]={'two_repeats_exact':True,'actual_input_tuples':2400,'states':600}
 (out/'manifest.json').write_text(json.dumps({'scope':'Actual Source API distance/radius words match Native in all600 states; candidate changes only derived diagnostic precision. Full control history preserves V115, full candidate history matches immutable Windows.','probe_sha256':manifest['hashes'][probe.name],'captures':captures,'cases':reports,'goal_complete':False},indent=2)+'\n')
print('Source same-input diagnostic proof complete',flush=True)
