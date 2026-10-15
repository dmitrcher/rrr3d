from pathlib import Path
import json,gzip,struct
p=Path(__file__).resolve().parent;checks=[]
f=lambda u:struct.unpack('<f',struct.pack('<I',u))[0];w=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];mul=lambda a,b:f(w(a*b));ratios=[1.5,2.66,1.78,1.30,1,.74];previous=None
for rep in [0,1]:
 rows=list(map(json.loads,gzip.open(f'outputs/controller-audit-v170/native/run{rep}.jsonl.gz','rt')))
 if previous is not None:assert rows==previous
 previous=rows
 for r in rows:
  if r.get('event')!='motor.sample':continue
  torque=f(r['maximum_torque_word']);sem=f(r['sem_word']);diff=f(r['differential_word']);gear=f(w(ratios[r['gear']]));gamek=f(w(1.15));prefix=mul(mul(torque,gear),diff)
  checks.append(dict(repeat=rep,precision=r['precision'],maximum_torque=torque,gear=r['gear'],native_word=r['result_word'],folded_word=w(mul(prefix,mul(sem,gamek))),sequential_f32_word=w(mul(mul(prefix,sem),gamek))))
for rep in [0,1]:
 for pc in [24,53]:
  rows=[r for r in checks if r['repeat']==rep and r['precision']==pc];assert len(rows)==24;print('repeat',rep,'precision',pc,'folded mismatches',sum(r['native_word']!=r['folded_word'] for r in rows),'sequentialF32 mismatches',sum(r['native_word']!=r['sequential_f32_word'] for r in rows))
assert all(r['native_word']==r['sequential_f32_word'] for r in checks if r['precision']==24)
(p/'torque-association-proof.json').write_text(json.dumps(dict(scope='Original unchanged GameCar actual constructor/PDB motor and actual CalcTorque versus current folded SEM*1.15 Source arithmetic. Production FP context must be observed before choosing a repair.',checks=checks,goal_complete=False),indent=2)+'\n')
