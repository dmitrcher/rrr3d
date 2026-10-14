from pathlib import Path
import json,gzip,struct
root=Path.cwd();word=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
source=list(map(json.loads,open(root/'outputs/shape-center-v153/source-rows465/track0-physical.jsonl')))
native=list(map(json.loads,gzip.open(root/'outputs/mixed-contact-v152/native-step465/track0-run0-private.jsonl.gz','rt')))
prepared=[x['native_solver_row']['words'] for x in native if x.get('function')=='solver-initialize-exit'][:4]
rows=[x['sample'] for x in source if x['event']=='physics.native-contact-row' and x['sample']['step']==465 and x['sample']['iteration']==0]
frames=[x['sample'] for x in source if x['event']=='physics.native-point-material-v150']
caches=[x['sample'] for x in source if x['event']=='physics.native-cache-input'];assert len(caches)==1
cache=caches[0];transport=next(x for x in native if x.get('function')=='point-transport-entry')
assert len(rows)==len(frames)==len(prepared)==len(cache['current'])==4
assert len(frames)==4
checks=[]
for i,(s,f,n) in enumerate(zip(rows,frames,prepared)):
 expected=transport[f'transport_input_point_{i}']['words']
 check=dict(point=i,triangle=f['triangle'],geometry_exact=list(map(word,cache['current'][i]))+[word(cache['separations'][i])]==expected[:3]+expected[6:7],mass_exact=word(s['effective_mass'])==n[20],bias_exact=word(s['scaled_bias'])==n[24],lever_exact=list(map(word,s['lever_arm']))==n[4:7],solver_quaternion_exact=list(map(word,f['solver_quaternion']))==n[16:20])
 assert all(check[k] for k in ['geometry_exact','mass_exact','bias_exact','lever_exact','solver_quaternion_exact']);checks.append(check)
def physical(x):
 if isinstance(x,dict):return {k:physical(v) for k,v in x.items()}
 if isinstance(x,list):return [physical(v) for v in x]
 return x if isinstance(x,bool) else word(x)
n=[x.get('sample',x) for x in map(json.loads,gzip.open(root/'outputs/game-control-wake-v133/native/track0-rep0.jsonl.gz','rt')) if x['event']=='world.sample']
s=[x['sample'] for x in source if x['event']=='solo.sample']
plain=[x['sample'] for x in map(json.loads,open(root/'outputs/shape-center-v153/moving-maps/track0-on0.jsonl')) if x['event']=='solo.sample'];assert len(s)==len(n)==900 and s==plain
fields=['position','velocity','angular_momentum','rotation','wheels','body_awake'];same=[physical({k:a[k] for k in fields})==physical({k:b[k] for k in fields}) for a,b in zip(n,s)]
assert all(same[:478]) and not same[478]
report=dict(scope='Actual moving map0, one car/no AI opponents, fixed original controls and fixtures. Read-only diagnostics preserve all900 candidate states; no Native states are implementation inputs.',contacts=checks,one_pair_cache_preparation_at465=True,initial_states_exact=478,first_difference=478,previous_first_difference=465,coefficient_changes=False,goal_complete=False)
(root/'work/shape-center-v153/live465-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Actual four contact geometry/order/quaternions/masses/bias/levers exact;478 initial physical states exact')
