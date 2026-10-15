from pathlib import Path
import json,sys,importlib.util,hashlib,itertools
root=Path.cwd();sys.path.insert(0,str(root/'work/sync-rrr3d/tools/calibration'));import compare
spec=importlib.util.spec_from_file_location('words',root/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
def cases(path,platform):
 seen=set();current=None
 with compare.open_capture(path) as stream:
  for l in stream:
   x=json.loads(l);compare.finite(x);assert x['platform']==platform
   e=x['event'];name=x['detail']
   if e=='calibration.begin':
    assert current is None and name not in seen;seen.add(name);m=x['sample'];assert m['schema']==1 and m['sample_phase']=='after_solve' and m['dt']>0 and type(m['steps']) is int;current={'name':name,'meta':m,'rows':[],'descriptions':{}}
   elif e=='calibration.sample':
    assert current is not None and current['name']==name;s=x['sample'];assert s['tick']==len(current['rows']);assert abs(s['time']-(s['tick']+1)*current['meta']['dt'])<1e-6
    for field,size in [('position',3),('velocity',3),('angular_momentum',3),('rotation',4)]:assert len(s[field])==size
    assert len(s['wheels'])==4 and abs(sum(v*v for v in s['rotation'])-1)<.01
    assert all(type(w['contact']) is bool for w in s['wheels']);current['rows'].append(s)
   elif e in ('calibration.engine-description','calibration.peer-description'):
    current['descriptions'][e]=x['sample']
   elif e=='calibration.end':
    assert current is not None and current['name']==name and len(current['rows'])==current['meta']['steps'];yield current;current=None
 assert current is None and len(seen)==608,(path,len(seen))
def digest(p):
 h=hashlib.sha256()
 with p.open('rb') as stream:
  for chunk in iter(lambda:stream.read(1024*1024),b''):h.update(chunk)
 return h.hexdigest()
ref=root/'outputs/calibration-v14-source-windows.jsonl';assert digest(ref)=='4bcc66c8f4f28ccb77a07759175573cc56d72035636642617fae4e75addecc1f';out=root/'outputs/controller-steering-v177/final-broad';report={'comparison':'Same exact float32 observable contract as v49-v51 gates; full 608 actual cases; streaming validation.','cases':{},'decimal_only_samples':0,'samples':0}
paths=[out/f'candidate-run{i}.jsonl.gz' for i in range(2)]
for a,b,c in itertools.zip_longest(cases(ref,'windows-physx-d3d9'),cases(paths[0],'mac-jolt-metal'),cases(paths[1],'mac-jolt-metal')):
 assert a is not None and b is not None and c is not None and a['name']==b['name']==c['name'];assert b==c,('repeat',b['name']);assert len(a['rows'])==len(b['rows']);assert a['meta']['dt']==b['meta']['dt'];dif=[];first=None
 for x,y in zip(a['rows'],b['rows']):
  px,py=words.physical(x),words.physical(y)
  if px!=py:
   dif.append(x['tick'])
   if first is None:first={'tick':x['tick'],'fields':[k for k in px if px[k]!=py[k]],'windows':x,'mac':y}
  elif x!=y:report['decimal_only_samples']+=1
 report['samples']+=len(a['rows']);report['cases'][a['name']]={'samples':len(a['rows']),'difference_ticks':dif,'first_difference':first,'exact':not dif}
 if len(report['cases'])%50==0:print('compared',len(report['cases']),flush=True)
assert len(report['cases'])==608;report['exact']=[n for n,r in report['cases'].items() if r['exact']];report['remaining']=[n for n,r in report['cases'].items() if not r['exact']];report.update({'two_repeats_exact':True,'windows_reference_sha256':digest(ref),'goal_complete':False});(root/'outputs/controller-steering-v177/full-broad-comparison.json').write_text(json.dumps(report,indent=2)+'\n');print('exact',len(report['exact']),'/',608,'remaining',len(report['remaining']),'samples',report['samples'],flush=True)
