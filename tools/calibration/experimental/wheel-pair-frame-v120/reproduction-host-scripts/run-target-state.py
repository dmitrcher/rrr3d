from pathlib import Path
import json,os,subprocess,gzip,importlib.util
r=Path.cwd();o=r/'outputs/pair-pole-v120/target-state';o.mkdir();f=r/'work/frozen-v120-state-candidate';spec=importlib.util.spec_from_file_location('words',r/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
def rows(path):
 opener=gzip.open(path,'rt') if str(path).endswith('.gz') else path.open()
 with opener as stream:return [x['sample'] for x in map(json.loads,stream) if x.get('event')=='calibration.sample' and x['detail']=='pair-stock-pole-1']
base=rows(r/'outputs/pair-pole-v120/mac-contact/pair-stock-pole-1-32-0-physical.jsonl.gz');golden=rows(r/'outputs/pair-pole-v120/native-kernel/kernel-pair-stock-pole-1-32-0-physical.jsonl.gz');assert len(base)==len(golden)==300;summary={}
for mode in ['control','candidate']:
 captures=[]
 for rep in [0,1]:
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((f/'manifest.json').read_text())['environment'])
  if mode=='control':env.pop('RRR3D_EXPERIMENT_NATIVE_WHEEL_PAIR_FRAME_V120')
  path=o/f'{mode}-{rep}.jsonl';subprocess.run([str(f/'rrr3d_physics_calibration'),str(path),'list:pair-stock-pole-1'],env=env,capture_output=True,check=True,timeout=180);captures.append(rows(path));print(mode,rep,'complete',flush=True)
 assert captures[0]==captures[1]
 if mode=='control':assert captures[0]==base
 differences=[i for i,(a,b) in enumerate(zip(captures[0],golden)) if words.physical(a)!=words.physical(b)];summary[mode]={'states':len(captures[0]),'first_difference':differences[0] if differences else None,'differences':len(differences),'repeats_exact':True};print(mode,summary[mode],flush=True)
(o/'report.json').write_text(json.dumps({'scope':'Complete300-step pair-stock-pole-1 original fixture, two uninterrupted repeats per mode. Control preserves acceptedV119 histories. Two-body frame candidate, not general matrix acceptance.','modes':summary,'goal_complete':False},indent=2)+'\n')
