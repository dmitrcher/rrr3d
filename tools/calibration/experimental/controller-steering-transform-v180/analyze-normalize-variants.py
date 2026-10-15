import runpy,json,gzip,struct,math,itertools
from pathlib import Path
m=runpy.run_path('work/controller-steering-transform-v180/analyze-transform.py');f=m['f'];value=m['value'];word=m['word'];mul=m['mul'];add=m['add'];matrix=m['matrix'];mm=m['mm'];translation=m['translation'];quat=m['quaternion'];div=m['div']
rows=[x for x in map(json.loads,gzip.open('outputs/controller-steering-transform-v180-tilt/accepted-native0.jsonl.gz','rt')) if x['event']=='steering.pose'];report={}
for variant in ['double','xyzw','wxyz','paired','zyxw']:
 bad=[]
 for n in rows:
  q=list(map(value,n['rotation_before']))
  if n['angle_hook_hit']:
   r=[[value(n['matrix_before'][i*3+j]) for j in range(3)] for i in range(3)];half=mul(value(n['angle_word']),.5);r=mm(r,matrix([0,0,f(math.sin(half)),f(math.cos(half))]));q=quat(r)
   if variant=='double':sq=f(sum(x*x for x in q))
   elif variant=='paired':sq=add(add(mul(q[0],q[0]),mul(q[1],q[1])),add(mul(q[2],q[2]),mul(q[3],q[3])))
   else:
    order={'xyzw':[0,1,2,3],'wxyz':[3,0,1,2],'zyxw':[2,1,0,3]}[variant];sq=mul(q[order[0]],q[order[0]])
    for i in order[1:]:sq=add(sq,mul(q[i],q[i]))
   inv=div(1,f(math.sqrt(sq)));q=[mul(x,inv) for x in q]
  if list(map(word,q))!=[0 if x==0x80000000 else x for x in n['rotation_call']]:bad.append(n['case'])
 report[variant]=bad
print('tilted normalization variants', {k:len(v) for k,v in report.items()});Path('outputs/controller-steering-transform-v180-tilt/normalization-variants.json').write_text(json.dumps(report,indent=2)+'\n')
