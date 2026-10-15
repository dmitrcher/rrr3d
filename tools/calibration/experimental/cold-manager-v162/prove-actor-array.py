from pathlib import Path
import json,gzip
p=Path('work/cold-manager-v162');checks=[]
for tr,last in [(16,420),(64,720)]:
 source={r['step']:r for r in map(json.loads,open(f'outputs/cold-manager-v162/source-track{tr}/managers.jsonl'))};states={};array=[];pending=[];types={}
 for t,r in sorted(source.items()):
  base=r['body_shapes'][0];owned=lambda key:any(base<=a<=base+4 for a in key)
  for key in pending:
   if key in array:i=array.index(key);array[i]=array[-1];array.pop();types.pop(key)
  allowed={tuple(sorted(k)) for k in r['active_wheel_pairs']}
  for key in map(lambda x:tuple(sorted(x)),r['created']):
   if not owned(key) or key in array:continue
   w=next((a-base-1 for a in key if base<a<=base+4),None);types[key]=(0 if key in allowed else 5,w) if w is not None else (1,None);array.append(key)
  pending=[tuple(sorted(k)) for k in r['removed'] if owned(k)];states[t]=[types[key] for key in array]
 for rep in [0,1]:
  rows=list(map(json.loads,gzip.open(f'outputs/cold-manager-v162/native-partition/track{tr}-run{rep}-private.jsonl.gz','rt')));mapping={r['queried_wheel_backend']['address']:(r['queried_wheel_backend']['words'][73]&65535)-9 for r in rows if 'queried_wheel_backend' in r};dif=[];snapshots=[]
  for r in rows:
   if 'partition_array_v162b' not in r:continue
   t=r['sample_tick_v71'];native=[]
   for i,ptr in enumerate(r['partition_array_v162b']['words']):
    words=r[f'partition_minus8_{i}']['words'];typ=words[7];wheel=mapping.get(words[9]-4,mapping.get(words[10]-4));native.append((typ,wheel))
   actual=states[t];entry=dict(tick=t,native=native,source=actual)
   if native!=actual:dif.append(entry)
   if t in [last,109,110,412,413,657,658]:snapshots.append(entry)
  check=dict(track=tr,repeat=rep,first_difference=dif[0] if dif else None,differences=len(dif),snapshots=snapshots);checks.append(check);print(tr,rep,'diff',len(dif),'first',None if not dif else dif[0]['tick'],'final',snapshots[-1])
(p/'actor-array-proof.json').write_text(json.dumps(dict(scope='Pure Source all-pair dense actor-list model, pending removals nextstep. Native type labels and wheel IDs decoded from actual actor-owned array; observed orders never enter model.',checks=checks,goal_complete=False),indent=2)+'\n')
