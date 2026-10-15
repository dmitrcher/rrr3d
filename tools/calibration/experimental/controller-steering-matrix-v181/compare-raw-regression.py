from pathlib import Path
import json,gzip,struct,hashlib
root=Path.cwd();out=root/'outputs/game-control-wake-v133';native=out/'native';mac=root/'outputs/controller-steering-matrix-v181/raw-regression'
def rows(p,event):
 f=gzip.open(p,'rt') if str(p).endswith('.gz') else p.open();return [r.get('sample',r) for r in map(json.loads,f) if r['event']==event]
def diff(a,b,path=''):
 if isinstance(a,dict):return sum((diff(a[k],b[k],path+'.'+k) for k in a),[])
 if isinstance(a,list):
  assert len(a)==len(b),(path,len(a),len(b));return sum((diff(x,y,path+f'[{i}]') for i,(x,y) in enumerate(zip(a,b))),[])
 if isinstance(a,bool):return [] if a==b else [dict(field=path,native=a,mac=b)]
 return [] if struct.pack('<f',a)==struct.pack('<f',b) else [dict(field=path,native=a,mac=b)]
report={};fields=['position','velocity','angular_momentum','rotation','wheels','body_awake']
for track in [0,16,48,64]:
 runs=[]
 for rep in [0,1]:
  n=rows(native/f'track{track}-rep{rep}.jsonl.gz','world.sample');m=rows(mac/f'track{track}-on{rep}.jsonl','solo.sample');assert len(n)==len(m)==900
  ds=[dict(tick=i,fields=diff({k:a[k] for k in fields},{k:b[k] for k in fields})) for i,(a,b) in enumerate(zip(n,m))];bad=[x for x in ds if x['fields']];runs.append(dict(repeat=rep,exact_states=900-len(bad),first_difference=bad[0] if bad else None))
  fixture=root/f'outputs/real-map-v124/repeats/track{track}-run0.words';assert fixture.read_bytes()==(mac/f'track{track}-on{rep}.words').read_bytes()
 report[str(track)]=dict(runs=runs);print(track,[(r['exact_states'],None if r['first_difference'] is None else r['first_difference']['tick']) for r in runs])
(root/'outputs/controller-steering-matrix-v181/raw-regression-comparison.json').write_text(json.dumps(dict(scope='Verified original parent-free startup path; original SDK-only map replay one car/noAI. GameCar driver/rendered original race spawn still unproved.',tracks=report,identical_fixtures=True,goal_complete=False),indent=2)+'\n')

assert all(r['exact_states']==900 for t in report.values() for r in t['runs']), 'Raw map regression detected'
