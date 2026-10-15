from pathlib import Path
import json,gzip,collections
p=Path('work/cold-manager-v162');checks=[]
for track,last in [(16,420),(64,720)]:
 src=Path(f'outputs/cold-manager-v162/source-track{track}');source=[x['sample'] for x in map(json.loads,open(src/f'track{track}-physical.jsonl')) if x['event']=='solo.sample'];plain=[x['sample'] for x in map(json.loads,open(f'outputs/sleep-inputs-v160/moving-maps/track{track}-on0.jsonl')) if x['event']=='solo.sample'];assert source==plain and len(source)==900
 managers={x['step']:x for x in map(json.loads,open(src/'managers.jsonl'))};dispatch={x['sdk_counter']-1:x for x in map(json.loads,open(src/'dispatch.jsonl'))}
 for rep in [0,1]:
  r=list(map(json.loads,gzip.open(f'outputs/cold-manager-v162/native/track{track}-run{rep}-private.jsonl.gz','rt')));nq=collections.defaultdict(list);nm=collections.defaultdict(list);backends={}
  for x in r:
   if 'queried_wheel_backend' in x:backends[x['queried_wheel_backend']['address']]=(x['queried_wheel_backend']['words'][73]&65535)-9
   if x.get('function')=='wheel-enqueue-v162':nq[x['sample_tick_v71']].append((x['enqueue_wheel_v162']['words'][73]&65535)-9)
   if x.get('function')=='manager-pass-v162':
    a=x['manager_shape0_v162'];b=x['manager_shape1_v162'];w=(a['words'][73]&65535)-9
    nm[x['sample_tick_v71']].append(dict(wheel=w,slot=x['manager_v162']['words'][5],manager=x['manager_v162']['address'],other=b['address'],flags=x['manager_v162']['words'][22]))
  differences=[]
  for tick in range(last+1):
   actual=nq[tick];wanted=[w for w in dispatch.get(tick,{}).get('creation_visits',[]) if source[tick]['wheels'][w]['contact']]
   if actual!=wanted:differences.append(dict(tick=tick,native_enqueue=actual,source_creation=wanted,native_managers=nm[tick],source_managers=managers[tick]))
  transitions=[];previous=None
  for tick in range(last+1):
   order=nq[tick]
   if order!=previous:transitions.append(dict(tick=tick,enqueue=order,source=dispatch.get(tick,{}).get('creation_visits'),managers=nm[tick]));previous=order
  item=dict(track=track,repeat=rep,first_order_difference=differences[0] if differences else None,difference_count=len(differences),last_order_difference=differences[-1] if differences else None,transitions=transitions);checks.append(item)
  print(track,rep,'first',None if not differences else {k:v for k,v in differences[0].items() if k in ['tick','native_enqueue','source_creation']},'different',len(differences),'last transitions',[(x['tick'],x['enqueue']) for x in transitions[-8:]])
(p/'history-proof.json').write_text(json.dumps(dict(scope='Actual six-hook original manager/query/enqueue history; all900 original states preserved in4captures. Source fresh diagnostics preserve all900 states; Native enqueue vs Source actual creation visitation, excluding non-contact wheels.',checks=checks,goal_complete=False),indent=2)+'\n')
