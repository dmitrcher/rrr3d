import json,struct,gzip
F=lambda x:struct.unpack('<f',struct.pack('<f',x))[0];add=lambda a,b:F(F(a)+F(b));sub=lambda a,b:F(F(a)-F(b));mul=lambda a,b:F(F(a)*F(b));div=lambda a,b:F(F(a)/F(b));cross=lambda a,b:[sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))]
dot=lambda a,b:add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));wf=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
ms=[json.loads(l) for l in open('outputs/material-friction-trace-v62/62-0.jsonl')]
frames={tuple(r['sample']['callback_plane']):r['sample'] for r in ms if r['event']=='physics.native-material-stages'}
rows=[r['sample'] for r in ms if r['event']=='physics.native-contact-row' and r['sample']['iteration']==0]
ns=[json.loads(l) for l in gzip.open('outputs/triangle-native-followup-v60/native-border-grounded-62-0-private.jsonl.gz','rt')]
native=[r for r in ns if r.get('function')=='solver-initialize-exit' and r.get('simulation_step')==61][:4]
report=[]
for i,(r,n) in enumerate(zip(rows,native)):
 q=frames[(0,0,1) if i<2 else (1,0,0)]['solver_quaternion'];x,y,z,w=map(F,q);x2=mul(x,2);y2=mul(y,2);z2=mul(z,2)
 ts=[[sub(mul(y,x2),mul(w,z2)),sub(1,add(mul(z,z2),mul(x,x2))),add(mul(z,y2),mul(w,x2))],[add(mul(z,x2),mul(w,y2)),sub(mul(z,y2),mul(w,x2)),sub(1,add(mul(y,y2),mul(x,x2)))]]
 masses=[]
 for t in ts:
  a=cross(r['lever_arm'],t);I=r['inverse_inertia'];response=[add(add(mul(I[j],a[0]),mul(I[j+3],a[1])),mul(I[j+6],a[2])) for j in range(3)];masses.append(div(1,add(r['inverse_mass'],dot(t,cross(response,r['lever_arm'])))))
 words=[wf(x) for x in n['native_solver_row']['words']]
 item={'index':i,'native_friction_mass':words[21:23],'mac_friction_mass':masses,'tangents':ts};report.append(item);print(item)
from pathlib import Path
Path('outputs/physics-v62-friction-mass-audit.json').write_text(json.dumps(report,indent=2)+'\n')
