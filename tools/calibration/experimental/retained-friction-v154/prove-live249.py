import json,gzip,struct
from pathlib import Path
root=Path.cwd();w=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
rows=list(map(json.loads,open('outputs/retained-friction-v154/source-track16-step249/track16-physical.jsonl')))
s=[r['sample'] for r in rows if r['event']=='physics.native-contact-row'];samples=[r['sample'] for r in rows if r['event']=='solo.sample'];plain=[r['sample'] for r in map(json.loads,open('outputs/retained-friction-v154/moving-maps/track16-on0.jsonl')) if r['event']=='solo.sample'];assert len(samples)==900 and samples==plain
n=[r for r in map(json.loads,gzip.open('outputs/mixed-contact-v152/native-track16-step249/track16-run0-private.jsonl.gz','rt')) if r.get('function')=='solver-initialize-exit'];assert len(s)==len(n)==10
checks=[]
for i,(a,b) in enumerate(zip(s,n)):
 expected=b['native_body0']['words'];check=dict(row=i,iteration=a['iteration'],point=a['index'],velocity_exact=list(map(w,a['velocity']))==expected[:3],angular_exact=list(map(w,a['angular']))==expected[4:7]);assert check['velocity_exact'] and check['angular_exact'];checks.append(check)
report=dict(scope='Actual map16 physical249,10 pre-response solver states,one car/no AI. Unchanged original Windows private trace and complete900 candidate replay; diagnostics preserve all900 states.',rows=checks,diagnostic_states_exact_to_plain_replay=900,coefficient_changes=False,first_body_mismatch=420,previous_first_body_mismatch=249,goal_complete=False)
Path('work/retained-friction-v154/live249-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('All10 solver states exact; all900 diagnostics states unchanged')
