from pathlib import Path
import json,gzip,struct
root=Path.cwd();word=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
native=list(map(json.loads,gzip.open(root/'outputs/point-material-v150/native-step358/track0-run0-private.jsonl.gz','rt')))
prepared=[x['native_solver_row']['words'] for x in native if x.get('function')=='solver-initialize-exit'][:4]
transport=next(x for x in native if x.get('function')=='point-transport-entry')
reports={}
for name,folder in [('baseline','point-material-v150/source-rows358'),('frame_only','world-contact-v151/ablation-frame-only'),('world_only','world-contact-v151/ablation-world-only'),('both','world-contact-v151/source-rows358')]:
 source=list(map(json.loads,open(root/f'outputs/{folder}/track0-physical.jsonl')))
 cache=next(x['sample'] for x in source if x['event']=='physics.native-cache-input')
 frames=[x['sample'] for x in source if x['event']=='physics.native-point-material-v150']
 rows=[x['sample'] for x in source if x['event']=='physics.native-contact-row' and x['sample']['step']==358 and x['sample']['iteration']==0]
 assert len(rows)==len(frames)==4
 checks=[]
 for i,(s,f,n) in enumerate(zip(rows,frames,prepared)):
  expected=transport[f'transport_input_point_{i}']['words']
  checks.append(dict(point=i,geometry_exact=list(map(word,cache['current'][i]))+[word(cache['separations'][i])]==expected[:3]+expected[6:7],solver_quaternion_exact=list(map(word,f['solver_quaternion']))==n[16:20],mass_exact=word(s['effective_mass'])==n[20],bias_exact=word(s['scaled_bias'])==n[24],lever_exact=list(map(word,s['lever_arm']))==n[4:7]))
 reports[name]=checks
assert all(all(v for k,v in r.items() if k!='point') for r in reports['both'])
assert all(r['solver_quaternion_exact'] for r in reports['frame_only'])
assert all(r['geometry_exact'] for r in reports['world_only'])
assert not reports['baseline'][2]['geometry_exact'] and not reports['baseline'][0]['solver_quaternion_exact']
def physical(x):
 if isinstance(x,dict):return {k:physical(v) for k,v in x.items()}
 if isinstance(x,list):return [physical(v) for v in x]
 return x if isinstance(x,bool) else word(x)
n=[x.get('sample',x) for x in map(json.loads,gzip.open(root/'outputs/game-control-wake-v133/native/track0-rep0.jsonl.gz','rt')) if x['event']=='world.sample']
s=[x['sample'] for x in source if x['event']=='solo.sample']
plain=[x['sample'] for x in map(json.loads,open(root/'outputs/world-contact-v151/moving-maps/track0-on0.jsonl')) if x['event']=='solo.sample']
assert s==plain and len(s)==len(n)==900
fields=['position','velocity','angular_momentum','rotation','wheels','body_awake']
same=[physical({k:a[k] for k in fields})==physical({k:b[k] for k in fields}) for a,b in zip(n,s)]
assert all(same[:359]) and not same[359]
report=dict(scope='Actual Source moving replay at358, independent flags tested separately and together; no Native values injected. Original Windows reference unchanged. Read-only Source diagnostics preserve all900 candidate states.',ablations=reports,all_four_prepared_contacts_exact=True,initial_physical_states_exact=359,first_divergence=359,previous_first_divergence=358,coefficient_changes=False,goal_complete=False)
(root/'work/world-contact-v151/live358-proof.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:{field:sum(x[field] for x in v) for field in ['geometry_exact','solver_quaternion_exact','mass_exact','bias_exact','lever_exact']} for k,v in reports.items()},indent=2))
