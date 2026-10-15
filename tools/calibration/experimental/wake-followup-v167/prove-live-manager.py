from pathlib import Path
import json,gzip,collections
p=Path(__file__).resolve().parent;checks=[]
for track,last in [(16,496),(64,720)]:
 folder='wake-followup-v167' if track==16 else 'cold-manager-v162'
 src=Path(f'outputs/pair-retirement-v166/source-track{track}')
 managers={r['step']:r for r in map(json.loads,open(src/'managers.jsonl'))}
 physical=[r['sample'] for r in map(json.loads,open(src/f'track{track}-physical.jsonl')) if r['event']=='solo.sample']
 plain=[r['sample'] for r in map(json.loads,open(f'outputs/pair-retirement-v166/moving-maps/track{track}-on0.jsonl')) if r['event']=='solo.sample'];assert physical==plain and len(physical)==900
 dispatch={r['sdk_counter']-1:r for r in map(json.loads,open(src/'dispatch.jsonl'))}
 for rep in [0,1]:
  q=collections.defaultdict(list)
  for r in map(json.loads,gzip.open(f'outputs/{folder}/native-partition/track{track}-run{rep}-private.jsonl.gz','rt')):
   if r.get('function')=='wheel-enqueue-v162':q[r['sample_tick_v71']].append((r['enqueue_wheel_v162']['words'][73]&65535)-9)
  for mode in ['selected','first-any','first-live']:
   bad=[]
   for tick in range(last+1):
    r=managers[tick];removed={tuple(sorted(pair)) for pair in r['removed']};base=r['body_shapes'][0];ranks={}
    for i,pair in enumerate(r['active_wheel_pairs']):
     if mode=='first-live' and tuple(sorted(pair)) in removed:continue
     for shape in pair:
      if base<shape<=base+4:ranks.setdefault(shape-base-1,i+1)
    contacts=[w for w in range(4) if physical[tick]['wheels'][w]['contact']]
    if tick not in dispatch:predicted=[]
    elif mode=='selected':predicted=[w for w in dispatch[tick]['creation_visits'] if w in contacts]
    else:predicted=sorted(contacts,key=lambda w:(ranks.get(w,0),w))
    if predicted!=q[tick]:bad.append(dict(tick=tick,native=q[tick],source=predicted))
   checks.append(dict(track=track,repeat=rep,mode=mode,compared_steps=last+1,differences=bad));print(track,rep,mode,'differences',len(bad),flush=True)
assert all(not c['differences'] for c in checks if c['mode']=='first-live')
(p/'first-live-manager-proof.json').write_text(json.dumps(dict(scope='Pure Source-derived allowed first non-retiring manager predicts original actual enqueue history, both maps/repeats. Source snapshots preserve all900 V166 physical states. Dormant Source constraints dispatch no queries. Selected-hit and first-any models retained as negative controls. No measured order supplies runtime inputs.',checks=checks,goal_complete=False),indent=2)+'\n')
