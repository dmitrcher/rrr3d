"""Run the compiled Mac steering/inertia observer against immutable original records."""
from pathlib import Path
import argparse,json,gzip,hashlib,struct,subprocess,os
parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path,default=Path('verification'));args=parser.parse_args();root=Path(__file__).resolve().parent;manifest=json.loads((root/'manifest.json').read_text());sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
for name,expected in manifest['files'].items():
 rel=Path(name);assert not rel.is_absolute() and '..' not in rel.parts;assert sha(root/rel)==expected,('checksum',name)
profile=json.loads((root/'profile.json').read_text());profile['RRR3D_CALIBRATION_PROFILE']=str(root/'windows-source-v14.cfg');env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(profile);args.output.mkdir(parents=True,exist_ok=True);reports=[];cz=lambda a:[0 if x==0x80000000 else x for x in a]
for mode in manifest['modes']:
 native=[x for x in map(json.loads,gzip.open(root/'references'/f'{mode}.jsonl.gz','rt')) if x['event']=='steering.pose'];assert len(native)==45
 if 'tilt' in mode:
  rotation=args.output/f'{mode}-rotation.words';rotation.write_text(' '.join(map(str,native[0]['rotation_initial']))+'\n');tail=[mode,str(rotation.resolve())]
 else:tail=['offset'] if mode=='offset' else []
 previous=None
 for rep in range(2):
  capture=args.output/f'{mode}-{rep}.jsonl';subprocess.run([str(root/'SourceSteeringWorldProbe'),str(capture.resolve()),*tail],env=env,check=True,timeout=120);data=capture.read_bytes();rows=list(map(json.loads,data.splitlines()));poses=[x for x in rows if x['event']=='steering.pose'];commands=[x for x in rows if x['event']=='steering.command'];assert len(poses)==len(commands)==45
  for n,p,c in zip(native,poses,commands):
   assert c['case']==p['case']==n['case'];assert c['all_wheels_contact'] and c['rear_pivot_word']==3212836864
   for key in ['position_before','rotation_before','position_after','rotation_after']:assert cz(n[key])==cz(p[key]),(mode,rep,n['case'],key,n[key],p[key])
   if n['angle_hook_hit']:assert cz([n['angle_word']])==cz([c['yaw_word']]),(mode,n['case'],'angle')
  if previous is not None:assert previous==data,(mode,'repeat differs')
  previous=data;reports.append(dict(mode=mode,repeat=rep,exact_poses=45));print(mode,rep,'45/45 exact',flush=True)
report=dict(scope=manifest['scope'],runs=reports,total_exact_poses=45*len(reports),repeats_identical=True,installed_game_replaced=False,full_goal_complete=False);(args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print('All packaged steering/inertia checks passed. Full game goal remains open.')
