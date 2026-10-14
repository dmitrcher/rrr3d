from pathlib import Path
import json,gzip,struct,re
root=Path.cwd(); out=root/'work/model-drop-v122'
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
value=lambda u:struct.unpack('<f',struct.pack('<I',u))[0]
def load(p):return [json.loads(re.sub(r'\bnan\b','NaN',s)) for s in gzip.open(p,'rt')]
def differences(a,b):
 assert len(a)==len(b)
 signed_zeros.extend({'axis':i,'native_word':x,'source_word':y} for i,(x,y) in enumerate(zip(a,b)) if x!=y and not (x&0x7fffffff or y&0x7fffffff))
 return [{'axis':i,'native_word':x,'source_word':y,'native':value(x),'source':value(y)} for i,(x,y) in enumerate(zip(a,b)) if x!=y and (x&0x7fffffff or y&0x7fffffff)]
reports=[];signed_zeros=[]
for tick in [145,146]:
 for rep in [0,1]:
  n=load(root/f'outputs/model-drop-v122/native-kernel/kernel-marauder-drop-{tick}-{rep}-private.jsonl.gz')
  p=load(root/f'outputs/model-drop-v122/mac-candidate/marauder-drop-{tick}-{rep}-prep.jsonl.gz')
  s=load(root/f'outputs/model-drop-v122/mac-candidate/marauder-drop-{tick}-{rep}-solver.jsonl.gz')
  tire=load(root/f'outputs/model-drop-v122/mac-candidate/marauder-drop-{tick}-{rep}-tire.jsonl.gz')
  bypoint={tuple(x['wheel_backend']['words'][116:119]):x for x in n if x.get('function')=='wheel-spring-prepare'}
  entry={x['ecx']:x for x in n if x.get('function')=='spring-row-entry'}
  ready={entry[x['esi']]['spring_object']['words'][7]:x for x in n if x.get('function')=='spring-row-ready'}
  preparation=[];slotmap={};jacmap={}
  for z in p:
   key=tuple(map(bits,z['world_point'])); matches=[(k,x) for k,x in bypoint.items() if k==key or (k[0]==key[0] and k[2]==key[2])]
   assert len(matches)==1,(tick,rep,z['wheel'],'ambiguous point binding')
   nativekey,w=matches[0]; preparation.append({'wheel':z['wheel'],'kind':'world-point','differences':differences(list(nativekey),list(key))}); body=None
   for field,idx in [('spring',69),('longitudinal',71),('lateral',72)]:
    slot=w['wheel_tail']['words'][idx]&0x3ffff;slotmap[slot]=(z['wheel'],field)
    x=ready[slot];row=x['constraint_row']['words'];jacmap[tuple(row[12:15])]=(z['wheel'],field)
    diffs={f:d for f,a,b in [('mass',[row[11]],[bits(z[field]['mass'])]),('jacobian',row[12:15],list(map(bits,z[field]['jacobian']))),('inverse_jacobian',row[20:23],list(map(bits,z[field]['inverse_jacobian'])))] if (d:=differences(a,b))}
    body=entry[x['esi']]['body0']['words']
    preparation.append({'wheel':z['wheel'],'kind':field,'differences':diffs})
   preparation.append({'wheel':z['wheel'],'kind':'body-pose','differences':differences(body[28:32]+body[25:28],list(map(bits,z['quaternion']+z['center'])))})
  solve=[]
  for fn,phase in [('spring-solve-input','spring-before'),('spring-solve-applied','spring-after')]:
   nr=[x for x in n if x.get('function')==fn and jacmap.get(tuple(x['solve_row']['words'][12:15]),(None,None))[1]=='spring'];sr=[x for x in s if x.get('phase')==phase];assert len(nr)==len(sr)==20
   for i,(x,z) in enumerate(zip(nr,sr)):
    assert jacmap[tuple(x['solve_row']['words'][12:15])][0]==z['wheel']
    row=x['solve_row']['words'];body=x['solve_body0']['words']
    d=differences(body[:3]+body[4:7]+[row[28]],list(map(bits,z['velocity']+z['angular']+[z['lambda']])))
    if d:solve.append({'phase':phase,'index':i,'wheel':z['wheel'],'differences':d})
  ts=[]
  for fn,suffix in [('spring-solve-input','input'),('spring-solve-applied','result')]:
   nr=[x for x in n if x.get('function')==fn and jacmap.get(tuple(x['solve_row']['words'][12:15]),(None,None))[1] in ['longitudinal','lateral']];sr=[x for x in tire if x['event'].endswith('-'+suffix)];assert len(nr)==len(sr)==40
   for i,(x,z) in enumerate(zip(nr,sr)):
    row=x['solve_row']['words'];wheel,kind=jacmap[tuple(row[12:15])];assert z['event']==kind+'-'+suffix
    body=x['solve_body0']['words'];a=body[:3]+body[4:7]+[row[28]];b=list(map(bits,[z['linear'][0],z['linear'][2],z['linear'][1],-z['angular'][0],-z['angular'][2],-z['angular'][1],z['lambda']]))
    d=differences(a,b)
    if d:ts.append({'phase':suffix,'index':i,'wheel':wheel,'kind':kind,'differences':d})
  reports.append({'tick':tick,'repeat':rep,'prepared_rows':preparation,'spring_differences':solve,'tire_differences':ts})
(out/'candidate-row-proof.json').write_text(json.dumps({'scope':'Actual Native/Source static identity mesh point and every prepared mass/Jacobian/inverse-Jacobian plus40 spring and80 tire incoming/result row checks per capture,145/146 two repeats. Signed zeros explicitly separate; no raw ABI equality claim.','captures':reports,'private_signed_zero_differences':signed_zeros,'raw_ABI_equality_claimed':False,'goal_complete':False},indent=2)+'\n')
assert all(not p['differences'] for r in reports for p in r['prepared_rows']);assert all(not r['spring_differences'] and not r['tire_differences'] for r in reports)
for r in reports:
 print('tick',r['tick'],'repeat',r['repeat'],'prep differences',[p for p in r['prepared_rows'] if p['differences']]);print('spring',len(r['spring_differences']),r['spring_differences'][:1]);print('tire',len(r['tire_differences']),r['tire_differences'][:1])
