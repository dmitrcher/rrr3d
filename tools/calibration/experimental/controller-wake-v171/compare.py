from pathlib import Path
import json,gzip
p=Path(__file__).resolve().parent;checks=[]
for mode in ['off','on']:
 last=None
 for rep in [0,1]:
  n=list(map(json.loads,gzip.open(f'outputs/controller-wake-v171/native-schedule-d/run{rep}.jsonl.gz','rt')));s=list(map(json.loads,open(f'outputs/controller-wake-v171/source-d/{mode}-run{rep}.jsonl')))
  if last is not None:assert s==last
  last=s;ns=[r for r in n if r['event'] in ['wake.sample','wake.sequence']];ss=[r for r in s if r['event'] in ['wake.sample','wake.sequence']];assert len(ns)==len(ss)==40
  control=[dict(native=a,source=b) for a,b in zip(ns,ss) if a['awake']!=b['requested']];ns=[r for r in n if r['event']=='wake.schedule'];ss=[r for r in s if r['event']=='wake.schedule'];assert len(ns)==len(ss)==360
  physical=[dict(tick=a['tick'],native=a['awake'],source=b['awake']) for a,b in zip(ns,ss) if a!=b]
  entry=dict(mode=mode,repeat=rep,original_transition_cases=40,transition_differences=control,original_schedule_frames=360,schedule_differences=physical,native_awake_transitions=[dict(tick=r['tick'],awake=r['awake']) for i,r in enumerate(ns) if i==0 or r['awake']!=ns[i-1]['awake']],source_awake_transitions=[dict(tick=r['tick'],awake=r['awake']) for i,r in enumerate(ss) if i==0 or r['awake']!=ss[i-1]['awake']]);checks.append(entry)
  print(mode,rep,'transition mismatches',len(control),'schedule mismatches',len(physical),'first',physical[:1],flush=True)
(p/'comparison.json').write_text(json.dumps(dict(scope='Actual original GameCar::SetMoveCar/NxActor and both-nullable binding lifecycle versus actual Source GameCar and Jolt libraries. Schedule isolates wake and sleep: gravity, tire contacts, torques, steering, angular stabilization and extra gravity disabled; same dt, body, far mesh, disabled wheels. Both repeats. Only wake/sleep observable claimed, not ordinary MotorProgress trajectory parity.',checks=checks,goal_complete=False),indent=2)+'\n')
assert all(not r['transition_differences'] and not r['schedule_differences'] for r in checks if r['mode']=='on')
