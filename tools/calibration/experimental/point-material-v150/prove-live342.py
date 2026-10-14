from pathlib import Path
import json,gzip,struct,hashlib
root=Path.cwd()
word=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
def capture(path):
 return list(map(json.loads,gzip.open(path,'rt') if str(path).endswith('.gz') else open(path)))
source=capture(root/'outputs/point-material-v150/source-rows342/track0-physical.jsonl')
native=capture(root/'outputs/body-contact-v144/native-step342/track0-run0-private.jsonl.gz')
prepared=[x['native_solver_row']['words'] for x in native if x.get('function')=='solver-initialize-exit'][:4]
rows=[x['sample'] for x in source if x['event']=='physics.native-contact-row' and x['sample']['step']==342 and x['sample']['iteration']==0]
frames=[x['sample'] for x in source if x['event']=='physics.native-point-material-v150']
assert len(rows)==len(frames)==len(prepared)==4
checks=[]
for i,(s,f,n) in enumerate(zip(rows,frames,prepared)):
 check=dict(point=i,triangle=f['triangle'],mass_exact=word(s['effective_mass'])==n[20],bias_exact=word(s['scaled_bias'])==n[24],lever_exact=list(map(word,s['lever_arm']))==n[4:7],solver_quaternion_exact=list(map(word,f['solver_quaternion']))==n[16:20])
 assert all(check[k] for k in ['mass_exact','bias_exact','lever_exact','solver_quaternion_exact']),check
 checks.append(check)
cache=next(x['sample'] for x in source if x['event']=='physics.native-cache-input')
transport=next(x for x in native if x.get('function')=='point-transport-entry')
for i in range(4):
 expected=transport[f'transport_input_point_{i}']['words']
 assert list(map(word,cache['current'][i]))+[word(cache['separations'][i])]==expected[:3]+expected[6:7]
def physical(x):
 if isinstance(x,dict):return {k:physical(v) for k,v in x.items()}
 if isinstance(x,list):return [physical(v) for v in x]
 return x if isinstance(x,bool) else word(x)
n=[x.get('sample',x) for x in capture(root/'outputs/game-control-wake-v133/native/track0-rep0.jsonl.gz') if x['event']=='world.sample']
s=[x['sample'] for x in source if x['event']=='solo.sample']
plain=[x['sample'] for x in capture(root/'outputs/point-material-v150/moving-maps/track0-on0.jsonl') if x['event']=='solo.sample']
assert s==plain and len(s)==len(n)==900
fields=['position','velocity','angular_momentum','rotation','wheels','body_awake']
same=[physical({k:a[k] for k in fields})==physical({k:b[k] for k in fields}) for a,b in zip(n,s)]
assert all(same[:358]) and not same[358]
report=dict(scope='Actual moving replay, one car, original controls and fixtures, no AI opponents; read-only diagnostics preserve all 900 candidate states.',prepared_rows=checks,all_four_points_and_separations_exact=True,physical342_exact=True,exact_initial_states=358,first_divergence=358,previous_first_divergence=342,coefficient_changes=False,goal_complete=False)
(root/'work/point-material-v150/live342-proof.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
