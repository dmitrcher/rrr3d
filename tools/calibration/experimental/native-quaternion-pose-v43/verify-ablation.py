"""Check the cumulative six-flag ladder on the final candidate executable."""
import argparse,json,pathlib,sys,hashlib
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--probe',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]));import verify_isolation
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
reference=compare.load(root/'expanded-native/expanded-pose-run0-windows.jsonl','windows-physx-d3d9');baseline=compare.load(root/'expanded-before/expanded-pose-ordinary-run0-mac.jsonl','mac-jolt-metal');flags=['INITIAL_COM','ACTOR_ORIGIN','ACTOR_OUTPUT','SHAPE_TRANSLATION','MASS_DOT_ORDER','FROZEN_CM_EXACT'];report={'probe_sha256':sha(a.probe),'stages':{}}
def physics_digest(path):
 h=hashlib.sha256();current=None
 for line in path.open():
  x=json.loads(line)
  if x.get('event')=='calibration.begin':current=x['detail']
  if x.get('event','').startswith('physics.'):h.update(json.dumps([current,x['event'],x['detail'],x['sample']],sort_keys=True,separators=(',',':')).encode())
 return h.hexdigest()
for stage,expected_exact in enumerate([2,2,3,5,5,6],1):
 d=root/f'ablation-stage{stage}';manifest=json.loads((d/'capture-manifest.json').read_text());assert manifest['probe_sha256']==report['probe_sha256'];captures={}
 for mode in ['ordinary','control']:
  runs=[];digests=[]
  for repeat in range(2):
   key=mode+str(repeat);info=manifest['captures'][key];path=d/f'expanded-pose-{mode}-run{repeat}-mac.jsonl';assert sha(path)==info['sha256']
   for j,flag in enumerate(flags):assert (info['environment'].get('RRR3D_EXPERIMENT_NATIVE_'+flag)=='1')==(mode=='ordinary' and j<stage)
   cases,_=verify_isolation.capture(path,'mac-jolt-metal','pose-v42-frozen-');assert set(cases)==set(reference);runs.append(cases);digests.append(physics_digest(path))
   if mode=='control':assert cases==baseline
   stats={n:{'first_difference':next((x['tick'] for x,y in zip(reference[n]['rows'],cases[n]['rows']) if x!=y),None),'max_position':max(compare.norm(x['position'],y['position']) for x,y in zip(reference[n]['rows'],cases[n]['rows'])),'max_velocity':max(compare.norm(x['velocity'],y['velocity']) for x,y in zip(reference[n]['rows'],cases[n]['rows']))} for n in cases};assert stats==info['cases'];captures[key]=stats
  assert runs[0]==runs[1] and digests[0]==digests[1]
 exact=sum(v['first_difference'] is None for v in captures['ordinary0'].values());assert exact==expected_exact;report['stages'][str(stage)]={'enabled':flags[:stage],'exact':exact,'six_flag_control_equals_v42':True,'repeatable_physics_diagnostics':True,'captures':captures}
(root/'physics-v43-ablation-audit.json').write_text(json.dumps(report,indent=2)+'\n');print('Final-binary ablation exact counts:',[v['exact'] for v in report['stages'].values()])
