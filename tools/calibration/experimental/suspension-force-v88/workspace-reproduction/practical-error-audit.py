"""Measure complete trajectories; prioritize magnitude independently of bit parity."""
from pathlib import Path
import hashlib, itertools, json, math, zipfile, io, gzip
root=Path.cwd()
def cases(stream):
 current=None
 for line in stream:
  row=json.loads(line);event=row['event']
  if event=='calibration.begin':
   assert current is None
   current={'name':row['detail'],'meta':row['sample'],'rows':[]}
  elif event=='calibration.sample':
   assert current is not None and row['detail']==current['name'] and row['sample']['tick']==len(current['rows'])
   current['rows'].append(row['sample'])
  elif event=='calibration.end':
   assert current is not None and len(current['rows'])==current['meta']['steps']
   yield current;current=None
 assert current is None
def distance(a,b):return math.sqrt(sum((x-y)**2 for x,y in zip(a,b)))
def angle(a,b):
 ax,ay,az,aw=a;bx,by,bz,bw=b
 v=(aw*bx-bw*ax-ay*bz+az*by,aw*by-bw*ay-az*bx+ax*bz,aw*bz-bw*az-ax*by+ay*bx)
 return 2*math.atan2(math.sqrt(sum(x*x for x in v)),abs(sum(x*y for x,y in zip(a,b))))*180/math.pi
def metrics(reference,candidate):
 p=[distance(a['position'],b['position']) for a,b in zip(reference,candidate)]
 v=[distance(a['velocity'],b['velocity']) for a,b in zip(reference,candidate)]
 s=[abs(distance(a['velocity'],[0,0,0])-distance(b['velocity'],[0,0,0])) for a,b in zip(reference,candidate)]
 r=[angle(a['rotation'],b['rotation']) for a,b in zip(reference,candidate)]
 return {'angular_momentum_max':max(distance(a['angular_momentum'],b['angular_momentum']) for a,b in zip(reference,candidate)), 'wheel_contact_mismatches':sum(x['contact']!=y['contact'] for a,b in zip(reference,candidate) for x,y in zip(a['wheels'],b['wheels'])), 'wheel_suspension_max':max(abs(x['suspension_length']-y['suspension_length']) for a,b in zip(reference,candidate) for x,y in zip(a['wheels'],b['wheels'])), 'wheel_normal_force_max':max(abs(x['engine_normal_force']-y['engine_normal_force']) for a,b in zip(reference,candidate) for x,y in zip(a['wheels'],b['wheels'])), 'position_max':max(p),'position_rms':math.sqrt(sum(x*x for x in p)/len(p)),'position_final':p[-1],'velocity_max':max(v),'speed_max':max(s),'speed_final':s[-1],'orientation_max_degrees':max(r),'orientation_final_degrees':r[-1]}
def capture(path):return path.open() if path.exists() else gzip.open(str(path)+'.gz','rt')
previous={r['name']:r for r in json.load(open(root/'outputs/physics-v87-final-practical-error-audit.json'))['cases']}
rows=[]
with capture(root/'outputs/calibration-v14-source-windows.jsonl') as native, capture(root/'outputs/suspension-force-v88/broad/candidate-run0.jsonl') as current:
 for a,b in itertools.zip_longest(cases(native),cases(current)):
  assert a is not None and b is not None and a['name']==b['name']
  assert len(a['rows'])==len(b['rows'])
  rows.append({'name':a['name'],'samples':len(a['rows']),'seconds':len(a['rows'])/60,'pair':bool(a['meta'].get('pair')),'current':metrics(a['rows'],b['rows']),'previous_v73':previous[a['name']]['previous_v73'],'previous_v87':previous[a['name']]['current']})
  if len(rows)%100==0:print('Measured',len(rows),flush=True)
assert len(rows)==608
selected=['settle','spring-soft','suspension-drop','wheel-drive','coast','brake','steer-left','steer-right','lateral','slope-longitudinal','slope-lateral','wall-hit','bump-crossing']
report={'units':'Position and speed use original source-world coordinates and seconds; no claim that one source unit is one metre. Orientation is degrees.','scope':'Full retained Windows and current Mac V88 histories; previously verified V73/V87 magnitude reports retained as comparison baseline; magnitude metrics are separate from strict float32 equivalence. These are isolated engine fixtures, not full-game race verification.','cases':rows,'selected':{r['name']:r for r in rows if r['name'] in selected},'largest_position_errors':sorted(rows,key=lambda r:r['current']['position_max'],reverse=True)[:20],'largest_orientation_errors':sorted(rows,key=lambda r:r['current']['orientation_max_degrees'],reverse=True)[:20],'goal_complete':False}
(root/'outputs/physics-v88-final-practical-error-audit.json').write_text(json.dumps(report,indent=2)+'\n')
for name,r in report['selected'].items():print(name,'now',r['current'],'before',r['previous_v73'],flush=True)
