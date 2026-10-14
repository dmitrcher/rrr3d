from pathlib import Path
import json,os,subprocess,importlib.util,hashlib
r=Path.cwd();out=r/'outputs/spring-force-v115/focused';out.mkdir(parents=True,exist_ok=False)
audit=json.loads((r/'outputs/physics-v114-validated-broad-audit.json').read_text())
names=list(audit['remaining'])+['settle','wheel-drive','brake','reverse-drive','wall-hit','wall-oblique','target-half','bump-crossing','curb-side','border-grounded','wall-air-straight-24','slope-longitudinal','slope-lateral']
spec=importlib.util.spec_from_file_location('words',r/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
def samples(p):
 data={}
 for x in map(json.loads,p.open()):
  if x['event']=='calibration.sample' and x['detail'] in names:data.setdefault(x['detail'],[]).append(x['sample'])
 assert set(data)==set(names);return data
reference=samples(r/'outputs/calibration-v14-source-windows.jsonl')
previous=samples(r/'outputs/air-torque-v114/broad-final/candidate-run0.jsonl')
probe=r/'work/frozen-v115-candidate/rrr3d_physics_calibration';base={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};base.update(json.loads((r/'work/frozen-v115-candidate/manifest.json').read_text())['environment']);results={}
for mode in ['control','candidate']:
 env=dict(base)
 if mode=='control':env.pop('RRR3D_EXPERIMENT_NATIVE_SUSPENSION_FORCE_STORE_V115')
 runs=[]
 for rep in [0,1]:
  p=out/f'{mode}-{rep}.jsonl';subprocess.run([str(probe),str(p),'list:'+','.join(names)],env=env,capture_output=True,check=True,timeout=300);runs.append(samples(p));print(mode,rep,'complete',flush=True)
 assert runs[0]==runs[1]
 if mode=='control':assert runs[0]==previous
 detail={}
 for n in names:
  assert len(runs[0][n])==len(reference[n]);dif=[i for i,(a,b) in enumerate(zip(reference[n],runs[0][n])) if words.physical(a)!=words.physical(b)];detail[n]={'states':len(runs[0][n]),'first_difference':dif[0] if dif else None,'exact':not dif}
 results[mode]={'repeats_exact':True,'cases':detail,'strict_cases':sum(x['exact'] for x in detail.values())}
(out/'report.json').write_text(json.dumps({'scope':'All17 remaining V114 cases plus13 retained controls; new flag off preserves V114 complete decimal histories, two repeats each.','probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'default_off_matches_v114_selected':True,'modes':results,'goal_complete':False},indent=2)+'\n')
print({mode:data['strict_cases'] for mode,data in results.items()},flush=True)
