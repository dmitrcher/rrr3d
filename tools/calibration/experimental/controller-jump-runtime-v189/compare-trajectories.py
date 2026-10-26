from pathlib import Path
import gzip,hashlib,importlib.util,json
R=Path.cwd();W=R/'work/controller-jump-runtime-v189';O=R/'outputs/controller-jump-runtime-v189/trajectories';REF=R/'work/v187-release-download/expanded/MotorRock-Mac-Jump-v187/native-sdk.jsonl.gz';BUNDLE=REF.parent
manifest=json.loads((BUNDLE/'manifest.json').read_text());assert manifest['source_golden_sha256']=='4bcc66c8f4f28ccb77a07759175573cc56d72035636642617fae4e75addecc1f'
assert hashlib.sha256(REF.read_bytes()).hexdigest()==manifest['files']['native-sdk.jsonl.gz']
spec=importlib.util.spec_from_file_location('floatwords',R/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');helper=importlib.util.module_from_spec(spec);spec.loader.exec_module(helper)
def load(path):
 with gzip.open(path,'rt') as f:
  rows=[json.loads(s) for s in f]
 return {name:[x['sample'] for x in rows if x['event']=='calibration.sample' and x['detail']==name] for name in ['gravity','game-air-gravity','game-air-pitch']},rows
ref,_=load(REF);assert all(len(v)==120 for v in ref.values());reports=[];previous={}
for mode in ['sdk','actual-method','mac']:
 for rep in range(2):
  p=O/f'{mode}{rep}.jsonl.gz';a,rows=load(p);expected=['gravity','game-air-gravity','game-air-pitch'] if mode=='sdk' else ['game-air-gravity','game-air-pitch'];assert all(len(a[n])==120 for n in expected)
  sem={n:[helper.physical(x) for x in a[n]] for n in expected};assert mode not in previous or previous[mode]==sem;previous[mode]=sem
  for name in expected:
   bad=[dict(tick=i,fields=[k for k in helper.physical(x) if helper.physical(x)[k]!=y[k]]) for i,(x,y) in enumerate(zip(ref[name],sem[name])) if helper.physical(x)!=y]
   reports.append(dict(mode=mode,repeat=rep,case=name,states=120,exact=120-len(bad),first_differences=bad[:4]));print(mode,rep,name,reports[-1]['exact'],'/120',flush=True)
report=dict(scope='Unchanged saved Windows SDK airborne trajectories vs fresh SDK harness, actual original JumpProgress on same SDK fixtures under CrossOver, and actual Source GameCar/Jolt. Original GameDLL not executed under native Windows in this checkpoint. No ground/contact transition, full OnFixedStep, AI or general-game equivalence.',reference=str(REF.relative_to(R)),reference_sha256=hashlib.sha256(REF.read_bytes()).hexdigest(),runs=reports,repeats_identical=True,vm_started=False,full_goal_complete=False)
(O/'comparison.json').write_text(json.dumps(report,indent=2)+'\n');assert all(x['exact']==120 for x in reports)
