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
 return {'wheel_angular_speed_max':max(abs(x['angular_speed']-y['angular_speed']) for a,b in zip(reference,candidate) for x,y in zip(a['wheels'],b['wheels'])), 'angular_momentum_max':max(distance(a['angular_momentum'],b['angular_momentum']) for a,b in zip(reference,candidate)), 'wheel_contact_mismatches':sum(x['contact']!=y['contact'] for a,b in zip(reference,candidate) for x,y in zip(a['wheels'],b['wheels'])), 'wheel_suspension_max':max(abs(x['suspension_length']-y['suspension_length']) for a,b in zip(reference,candidate) for x,y in zip(a['wheels'],b['wheels'])), 'wheel_normal_force_max':max(abs(x['engine_normal_force']-y['engine_normal_force']) for a,b in zip(reference,candidate) for x,y in zip(a['wheels'],b['wheels'])), 'position_max':max(p),'position_rms':math.sqrt(sum(x*x for x in p)/len(p)),'position_final':p[-1],'velocity_max':max(v),'speed_max':max(s),'speed_final':s[-1],'orientation_max_degrees':max(r),'orientation_final_degrees':r[-1]}
def capture(path):return path.open() if path.exists() else gzip.open(str(path)+'.gz','rt')
import struct
def canonical(row):
 row=dict(row)
 f32=lambda v:struct.unpack('<f',struct.pack('<f',v))[0]
 for k in ['position','velocity','rotation','angular_momentum']:row[k]=[f32(v) for v in row[k]]
 row['wheels']=[{k:f32(v) if isinstance(v,float) else v for k,v in wheel.items()} for wheel in row['wheels']]
 return row
rows=[]
with capture(root/'outputs/calibration-v14-source-windows.jsonl') as native, capture(root/'outputs/static-box-lever-v111/broad-final/candidate-run0.jsonl') as previous, capture(root/'outputs/free-wheel-v112/broad-final/candidate-run0.jsonl') as current:
 for a,b,c in itertools.zip_longest(cases(native),cases(previous),cases(current)):
  assert a and b and c and a['name']==b['name']==c['name']
  na,pa,ca=map(lambda case:list(map(canonical,case['rows'])),[a,b,c]);rows.append({'name':a['name'],'previous_v111':metrics(na,pa),'current':metrics(na,ca)})
assert len(rows)==608
fields=['wheel_angular_speed_max','position_max','velocity_max','orientation_max_degrees','angular_momentum_max','wheel_contact_mismatches','wheel_suspension_max','wheel_normal_force_max'];summary={}
for f in fields:
 change=[x for x in rows if x['previous_v111'][f]!=x['current'][f]];summary[f]={'improved':[x['name'] for x in change if x['current'][f]<x['previous_v111'][f]],'worsened':[x['name'] for x in change if x['current'][f]>x['previous_v111'][f]],'same':608-len(change)}
report={'scope':'Full run numerical magnitudes from actual float32 observables. JSON decimal serialization is normalized to its underlying F32 value before geometric/norm metrics. Raw-decimal historical report retained separately, not overwritten.','cases':rows,'maxima':summary,'goal_complete':False};(root/'outputs/physics-v112-validated-canonical-error-audit.json').write_text(json.dumps(report,indent=2)+'\n');print({f:{'improved':len(v['improved']),'worsened':v['worsened']} for f,v in summary.items()})
