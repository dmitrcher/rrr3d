from pathlib import Path
import json,struct
root=Path.cwd();names=['mobility/dirtdevil/wheel1/wheel-drive','mobility/marauder/wheel1/wheel-drive','mobility/marauder/wheel2/wheel-drive','mobility/marauder/wheel1/brake-moving','mobility/manticora/wheel3/lateral'];bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def load(p):
 result={n:[] for n in names}
 with p.open() as f:
  for line in f:
   if not any(n in line for n in names):continue
   r=json.loads(line)
   if r['event']=='calibration.sample' and r['detail'] in result:result[r['detail']].append(r['sample'])
 return result
def physical(r):
 r=dict(r)
 for k in ['position','velocity','rotation','angular_momentum']:r[k]=list(map(bits,r[k]))
 fields=['angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'];r['wheels']=[{k:bits(v) if k in fields else v for k,v in w.items()} for w in r['wheels']];return r
native=load(root/'outputs/calibration-v14-source-windows.jsonl');mac=load(root/'outputs/wheel-motor-v92/broad/candidate-run0.jsonl');report={}
for name in names:
 a,b=native[name],mac[name];assert len(a)==len(b)==600
 first=next((i for i,(x,y) in enumerate(zip(a,b)) if physical(x)!=physical(y)),None)
 if first is not None:
  px,py=physical(a[first]),physical(b[first]);report[name]={'first_difference':first,'fields':[k for k in px if px[k]!=py[k]],'windows':a[first],'mac':b[first]};print(name,first,report[name]['fields'])
(root/'outputs/wheel-motor-v92/next-priority.json').write_text(json.dumps({'scope':'Read-only first-divergence localization from completed repeat0; final repeat validation separate.','cases':report,'goal_complete':False},indent=2)+'\n')
