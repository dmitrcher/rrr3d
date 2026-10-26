from pathlib import Path
import json,hashlib,importlib.util
root=Path.cwd();w=root/'work/crossover-reference-v188';stage=w/'prefix/drive_c/MotorRock/sdk-reference';out=root/'outputs/crossover-reference-v188';names=['gravity','game-air-gravity','game-air-pitch'];native={n:[] for n in names}
spec=importlib.util.spec_from_file_location('words',root/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
with (root/'outputs/calibration-v14-source-windows.jsonl').open() as f:
 for r in map(json.loads,f):
  if r['event']=='calibration.sample' and r['detail'] in native:native[r['detail']].append(r['sample'])
reports=[];previous=None
for rep in range(2):
 rows=list(map(json.loads,(stage/f'controls{rep}.jsonl').read_text().splitlines()));states={n:[r['sample'] for r in rows if r['event']=='calibration.sample' and r['detail']==n] for n in names};assert all(len(r)==120 for r in states.values())
 physical={n:[words.physical(r) for r in states[n]] for n in names}
 if previous is not None:assert physical==previous
 previous=physical
 for n in names:
  differences=[dict(tick=i,fields=[k for k in words.physical(a) if words.physical(a)[k]!=b[k]],windows=a,crossover=states[n][i]) for i,(a,b) in enumerate(zip(native[n],physical[n])) if words.physical(a)!=b];reports.append(dict(case=n,repeat=rep,exact=120-len(differences),first_difference=differences[0] if differences else None));print(n,rep,reports[-1]['exact'],'/120',flush=True)
r=dict(environment='CrossOver26.3.0 on Mac arm64, task-owned prefix; original Windows PE observer and PhysXCore DLL',scope='Three original SDK trajectory controls; does not establish other scenarios or original GameCar equivalence',runs=reports,repeats_identical=True,vm_started=False,full_goal_complete=False);(out/'sdk-controls-comparison.json').write_text(json.dumps(r,indent=2)+'\n');assert all(r['exact']==120 for r in reports),'Compatibility runtime differs from immutable Windows controls'
