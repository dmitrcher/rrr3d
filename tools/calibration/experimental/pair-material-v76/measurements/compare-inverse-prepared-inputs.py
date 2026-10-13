from pathlib import Path
import json,gzip,struct
root=Path.cwd();namespace={};exec((root/'work/pair-contact-v71/verify-pair-contact-transitions.py').read_text().split('history=[]')[0],namespace);axes=namespace['axes'];fs=namespace['fs'];bits=namespace['bits'];canon=lambda w:0 if w==0x80000000 else w
history=[]
groups=[('native-remaining','v75-remaining','pair-stock-oblique-0',[39,40]),('native-remaining','v75-remaining','pair-stock-weak-second-0',[38,39])]
for folder,prefix,name,ticks in groups:
 for tick in ticks:
  data=list(map(json.loads,gzip.open(root/f'outputs/pair-local-cache-v75/{folder}/{prefix}-{name}-tick{tick}-0-private.jsonl.gz','rt')))
  entries=[x for x in data if x.get('function')=='solver-iteration-entry'];prepared=[x for x in data if x.get('function')=='solver-initialize-exit'];assert len(entries)==len(prepared)
  mac=list(map(json.loads,open(root/f'outputs/pair-material-v76/mac-box-inverse/{name}-0-native-pair.jsonl')))
  path=root/f'outputs/pair-material-v76/mac-box-inverse/{name}-0.jsonl'
  stream=path.open() if path.exists() else gzip.open(str(path)+'.gz','rt')
  contact_ticks=[x['sample']['tick'] for x in map(json.loads,stream) if x['event']=='calibration.body-impulse']
  starts=[i for i,r in enumerate(mac) if r['index']==0 and r['iteration']==0]
  offset=starts[contact_ticks.index(tick)]
  count=len(entries)//5; assert len(mac)>=len(entries);details=[]
  for j,x in enumerate(entries):
   p=fs(prepared[j]['native_solver_row']['words']);basis=axes(p[16:20]);m=mac[offset+j];assert m['index']==j%count and m['iteration']==j//count
   expected={'normal':basis[0],'tangent0':basis[1],'tangent1':basis[2],'mass':p[20:23],'bias':p[24:27],'friction':p[12:15],'normal_impulse':p[27],'static_mu':[p[7],p[11]],'dynamic_mu':[p[15],p[23]]}
   for b in range(2):
    body=fs(x[f'native_body{b}']['words']);expected.update({f'body{b}_velocity':body[:3],f'body{b}_angular':body[4:7],f'body{b}_lever':p[4:7] if b==0 else p[8:11],f'body{b}_inverse_mass':body[11],f'body{b}_inverse_inertia':body[15:24]})
   differences={};raw=[]
   for key,v in expected.items():
    a=list(map(bits,m[key])) if isinstance(v,list) else [bits(m[key])];b=list(map(bits,v)) if isinstance(v,list) else [bits(v)]
    if a!=b:raw.append(key)
    if list(map(canon,a))!=list(map(canon,b)):differences[key]={'mac':m[key],'native':v,'mac_words':a,'native_words':b}
   details.append({'index':j%count,'iteration':j//count,'raw_different_fields':raw,'differences':differences,'all_numeric_inputs_exact':not differences})
  history.append({'case':name,'tick':tick,'mac_offset':offset,'rows':len(entries),'numeric_exact_rows':sum(x['all_numeric_inputs_exact'] for x in details),'details':details});print(name,tick,'numeric inputs exact',history[-1]['numeric_exact_rows'],'/',len(entries))
(root/'outputs/physics-v76-inverse-prepared-input-audit.json').write_text(json.dumps({'scope':'Compare actual Mac kernel preparation/state to original saved Windows rows at same tick and contact order across continuous cold/warm history. Match contact-tick index to actual kernel reset boundaries; no fixed row offset assumed. Raw and signed-zero-canonical fields reported separately; no expected data injected.','history':history,'goal_complete':False},indent=2)+'\n')
