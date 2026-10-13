from pathlib import Path
import gzip,json,struct
f=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
fs=lambda x:[fword(y) for y in x]
fword=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
add=lambda x,y:f(x+y)
mul=lambda x,y:f(x*y)
sub=lambda x,y:f(x-y)
eps=f(1e-6)
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--captures-root',type=Path,default=Path(__file__).resolve().parent/'fixtures');parser.add_argument('--out',type=Path,default=Path('native-sat-proof-v72.json'));args=parser.parse_args()
history=[]
for p in args.captures_root.glob('*private.jsonl.gz'):
 data=list(map(json.loads,gzip.open(p,'rt')))
 for row in data:
  if row.get('function')!='box-overlap-before-cache':continue
  a=fs(row['box_matrix0']['words']);b=fs(row['box_matrix1']['words']);ha=fs(row['box_half0']['words']);hb=fs(row['box_half1']['words'])
  C=[]
  for i in range(3):
   cr=[]
   for j in range(3):
    products=[mul(a[3*i+k],b[3*j+k]) for k in range(3)]
    order=[0,1,2] if i==0 else [2,1,0]
    cr.append(add(add(products[order[0]],products[order[1]]),products[order[2]]))
   C.append(cr)
  absolute=[[f(abs(c)+eps) for c in cr] for cr in C]
  delta=[sub(b[9+k],a[9+k]) for k in range(3)]
  # Native A-axis projected distances use f32 SSE in Z,X,Y order.
  da=[add(add(mul(a[3*i+2],delta[2]),mul(a[3*i],delta[0])),mul(a[3*i+1],delta[1])) for i in range(3)]
  # B projections retain intermediate x87 precision then store f32.
  db=[f(b[3*j]*delta[0]+b[3*j+1]*delta[1]+b[3*j+2]*delta[2]) for j in range(3)]
  native_distances=fs(row['box_distances']['words'])
  computed=[]
  for i in range(3):
   # Last |C_i2|+eps stays in x87 after FSTS; memory copy rounds to f32.
   d=f((hb[0]*absolute[i][0]+hb[1]*absolute[i][1])+hb[2]*(abs(C[i][2])+eps)+ha[i])
   computed.append(sub(d,abs(da[i])))
  for j in range(3):
   d=f((ha[0]*absolute[0][j]+ha[1]*absolute[1][j])+ha[2]*absolute[2][j]+hb[j])
   computed.append(sub(d,abs(db[j])))
  expected=fs(row['box_overlaps']['words']);ab_wanted=row['box_abs_rotation']['words']
  history.append({'file':p.name,'tick':row['sample_tick_v71'],'overlaps_exact':list(map(bits,computed))==list(map(bits,expected)),'computed':computed,'native':expected,'distances_exact':list(map(bits,da+db))==list(map(bits,native_distances)),'computed_distances':da+db,'native_distances':native_distances,'absolute_rotation_exact':[bits(c) for cr in absolute for c in cr]==ab_wanted})
r={'scope':'Native box SAT six face-axis overlaps rebuilt from exact saved matrix/extents, not fitted axis choice. Original x87 FSTS preserves unrounded last projection+epsilon for A-axis radius; B axes reload rounded projection coefficients.','captures':len(history),'exact_overlaps':sum(x['overlaps_exact'] for x in history),'history':history,'goal_complete':False}
args.out.write_text(json.dumps(r,indent=2)+'\n')
assert history and all(x['overlaps_exact'] and x['distances_exact'] and x['absolute_rotation_exact'] for x in history)
print(r['exact_overlaps'],'/',r['captures'],'exact six-overlap batches')
for x in history:
 if not x['overlaps_exact'] or not x['distances_exact'] or not x['absolute_rotation_exact']:print(x)
