from pathlib import Path
import json,gzip,struct
root=Path.cwd();p=root/'work/edge-normal-v155';word=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
s=list(map(json.loads,open('outputs/edge-normal-v155/revision2/source-rows478/track0-physical.jsonl')))
n=list(map(json.loads,gzip.open('outputs/retained-friction-v154/native-step478/track0-run0-private.jsonl.gz','rt')))
rows=[x['sample'] for x in s if x['event']=='physics.native-contact-row'];nr=[x for x in n if x.get('function')=='solver-initialize-exit'];frames=[x['sample'] for x in s if x['event']=='physics.native-point-material-v150'];caches=[x['sample'] for x in s if x['event']=='physics.native-cache-input'];transports=[x for x in n if x.get('function')=='point-transport-entry' and x.get('transport_input_point_size')]
print('row/frame/cache/transport sizes',len(rows),len(nr),len(frames),len(caches),[x['transport_input_point_size'] for x in transports])
assert len(caches)==len(transports)==1
cache=caches[0];transport=transports[0];count=len(cache['current']);assert count==transport['transport_input_point_size']
checks=[]
for i,(a,b) in enumerate(zip(rows,nr)):
 words=b['native_solver_row']['words'];body=b['native_body0']['words'];idx=i%count
 checks.append(dict(row=i,iteration=a['iteration'],point=a['index'],geometry_exact=list(map(word,cache['current'][idx]))+[word(cache['separations'][idx])]==transport[f'transport_input_point_{idx}']['words'][:3]+transport[f'transport_input_point_{idx}']['words'][6:7],mass_exact=word(a['effective_mass'])==words[20],bias_exact=word(a['scaled_bias'])==words[24],lever_exact=list(map(word,a['lever_arm']))==words[4:7],quaternion_exact=list(map(word,frames[idx]['solver_quaternion']))==words[16:20],velocity_exact=list(map(word,a['velocity']))==body[:3],angular_exact=list(map(word,a['angular']))==body[4:7]))
report=dict(scope='Actual next map0/478 private rows; source trace900states, Native readonly rep0. Later integration stages still open.',rows=checks,goal_complete=False)
(p/'revision2-live478-proof.json').write_text(json.dumps(report,indent=2)+'\n')
for x in checks:print(x)
print('SOURCE CACHE',json.dumps(cache))
