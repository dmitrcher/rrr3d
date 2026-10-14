from pathlib import Path
import json,struct,sys
r=Path.cwd();names={'curb-side':[],'bump-crossing':[]}
for row in map(json.loads,open(sys.argv[1])):
 if row.get('event')=='calibration.sample':names[row['detail']].append(row['sample'])
reference={n:[] for n in names}
for row in map(json.loads,open(r/'outputs/calibration-v14-source-windows.jsonl')):
 if row.get('event')=='calibration.sample' and row['detail'] in names:reference[row['detail']].append(row['sample'])
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def physical(row):
 row=dict(row)
 for k in ['position','velocity','rotation','angular_momentum']:row[k]=list(map(bits,row[k]))
 f={'angular_speed','height','engine_longitudinal_slip','engine_lateral_slip','suspension_length','engine_normal_force','longitudinal_impulse','lateral_impulse'};row['wheels']=[{k:bits(v) if k in f else v for k,v in w.items()} for w in row['wheels']];return row
for n,rows in names.items():
 assert len(rows)==len(reference[n])==600
 dif=[a['tick'] for a,b in zip(rows,reference[n]) if physical(a)!=physical(b)];print(n,len(rows),'first',dif[0] if dif else None,'differing ticks',len(dif))
