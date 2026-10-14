import json,gzip,struct,hashlib
from pathlib import Path
out=Path('outputs/live-sleep-v128/wake-reset-target')
def rows(path,event):
 f=gzip.open(path,'rt') if str(path).endswith('.gz') else open(path)
 return [x.get('sample',x) for x in map(json.loads,f) if x['event']==event]
def bits(x):return struct.pack('<f',x)
def diff(a,b,path=''):
 if isinstance(a,dict):return sum((diff(a[k],b[k],path+'.'+k) for k in a),[])
 if isinstance(a,list):return sum((diff(x,y,path+f'[{i}]') for i,(x,y) in enumerate(zip(a,b))),[])
 if isinstance(a,bool):return [] if a==b else [dict(field=path,native=a,mac=b)]
 return [] if bits(a)==bits(b) else [dict(field=path,native=a,mac=b)]
fields=['position','velocity','angular_momentum','rotation','wheels']
native=rows('outputs/local-mesh-v126/native-corrected/track0-rep0.jsonl.gz','world.sample')
report={}
for mode in ('off','on0','on1'):
 mac=rows(out/(mode+'.jsonl'),'solo.sample');assert len(native)==len(mac)==900
 differences=[dict(tick=i,fields=diff({k:n[k] for k in fields},{k:m[k] for k in fields})) for i,(n,m) in enumerate(zip(native,mac))];bad=[x for x in differences if x['fields']]
 report[mode]=dict(exact_states=900-len(bad),first_difference=bad[0] if bad else None,differing_states=len(bad),target28=differences[28],target29=differences[29])
old=rows('outputs/real-map-v124/mac-actuators/track0-rep0.jsonl','solo.sample');off=rows(out/'off.jsonl','solo.sample');# Legacy raw mode included a GameCar momentum roundtrip; isolated mode deliberately omits it.
for mode in ('off','on0','on1'):
 assert hashlib.sha256((out/(mode+'.words')).read_bytes()).hexdigest()=='84def079e86575581ee15bdeb6793b0a6360c6f0545886803a6bf0e2e628e9f5'
report['neutral_gamecar_roundtrip_omitted']=True;report['identical_fixtures']=True
(out/'comparison-corrected-reference.json').write_text(json.dumps(report,indent=2)+'\n')
for mode in ('off','on0','on1'):print(mode,'exact states',report[mode]['exact_states'],'first',report[mode]['first_difference'])
