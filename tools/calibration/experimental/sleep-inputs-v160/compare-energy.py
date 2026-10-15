from pathlib import Path
import struct,json,gzip
p=Path('work/sleep-inputs-v160');v=lambda u:struct.unpack('<f',struct.pack('<I',u))[0];b=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
source={x['step']:x for x in map(json.loads,open('outputs/cooking-order-v159/moving-maps/track64-on0.energy.jsonl'))}
poses=list(map(json.loads,open('outputs/cooking-order-v159/source-track64-step656/track64-pose.jsonl')));before={x['step']:x for x in poses if x['phase']=='before-update'};after={x['step']-1:x for x in poses if x['phase']=='after-restore'}
checks=[]
for rep in [0,1]:
 rows=list(map(json.loads,gzip.open(f'outputs/sleep-inputs-v160/native-energy656-v160/run{rep}-private.jsonl.gz','rt')));frames={}
 for x in rows:
  if 'energy_body' in x and abs(v(x['energy_body']['words'][120])-.0005)<1e-8:frames.setdefault(x['sample_tick_v71'],{})[x['function']]=x
 out=[]
 for tick,d in sorted(frames.items()):
  w=d['energy-sleep-entry']['energy_body']['words'];source_input=None
  if tick in before and tick in after:
   a=after[tick];z=before[tick];source_input=list(map(b,a['center']+z['center']+a['rotation']+z['rotation']+a['inverse_inertia']+[a['inverse_mass']]))
   native_input=w[45:48]+w[60:63]+w[48:52]+w[63:67]+w[121:124]+w[120:121]
   delta=[i for i,(x,y) in enumerate(zip(source_input,native_input)) if x!=y and (x&0x7fffffff or y&0x7fffffff)]
  else:delta=None
  entry=dict(step=tick,native_count=sum(w[8:12]),native_count_slots=w[8:12],source_count=source.get(tick,{}).get('interactions'),input_differences=delta)
  native={'wake':d['energy-wake-store']['xmm_words'][0][0]}
  if 'energy-threshold-test' in d:native.update(energy=d['energy-threshold-test']['xmm_words'][0][0],threshold=d['energy-threshold-test']['xmm_words'][1][0])
  else:native.update(energy=0,threshold=0)
  entry.update(native={k:v(x) for k,x in native.items()},source=source.get(tick),state_difference_fields=[k for k,x in native.items() if tick in source and x!=b(source[tick][k]) and (x&0x7fffffff or b(source[tick][k])&0x7fffffff)])
  out.append(entry)
 checks.append(dict(repeat=rep,frames=len(out),first_input_difference=next((r for r in out if r['input_differences']),None),first_count_difference=next((r for r in out if r['source_count'] is not None and r['source_count']!=r['native_count']),None),first_state_difference=next((r for r in out if r['state_difference_fields']),None),last20=out[-20:]))
 print('rep',rep,'frames',len(out))
 for key in ['first_input_difference','first_count_difference','first_state_difference']:print(key,checks[-1][key])
(p/'energy-input-comparison.json').write_text(json.dumps(dict(scope='Read-only original energy values and actual Source private before/after poses; only correctly aligned same900histories through655. Actual Source656 sleeping is independent failure.',checks=checks,goal_complete=False),indent=2)+'\n')
