from pathlib import Path
import json,gzip
out=Path('outputs/controller-torque-v173');checks=[]
fp=json.loads(Path('outputs/controller-fp-v172/comparison.json').read_text());assert len([r for r in fp['runs'] if r['accepted_for_fp']])==2
for mode in ['off','on']:
 previous=None
 for rep in [0,1]:
  native=[r for r in map(json.loads,gzip.open(f'outputs/controller-audit-v170/native/run{rep}.jsonl.gz','rt')) if r['event']=='motor.sample' and r['precision']==24]
  source=list(map(json.loads,(out/f'source/{mode}-run{rep}.jsonl').open()));assert len(native)==len(source)==24
  assert all(all(a[k]==b[k] for k in ['maximum_torque_word','sem_word','differential_word','gear']) for a,b in zip(native,source))
  if previous is not None:assert source==previous
  previous=source;bad=[dict(case=i,gear=a['gear'],native_word=a['result_word'],source_word=b['result_word']) for i,(a,b) in enumerate(zip(native,source)) if a['result_word']!=b['result_word']]
  checks.append(dict(mode=mode,repeat=rep,exact_cases=24-len(bad),differences=bad));assert len(bad)==(9 if mode=='off' else 0)
(out/'comparison.json').write_text(json.dumps(dict(scope='Actual compiled Source GameCar OnFixedStepDrive torque against original unchanged DLL CalcTorque: four motor torques, all six gears, two repeats. Reverse sign normalized; same SEM/differential. Windows production PC24 directly observed in V172. No full MotorProgress/RPM/gear trajectory parity claim.',checks=checks,goal_complete=False),indent=2)+'\n')
print([(r['mode'],r['repeat'],r['exact_cases']) for r in checks])
