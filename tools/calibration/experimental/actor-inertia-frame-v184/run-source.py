from pathlib import Path
import subprocess,os,json,struct,gzip,hashlib,sys
modeName=sys.argv[1] if len(sys.argv)>1 else 'base';assert modeName in ['base','offset','tilt','tilt-offset','tilt-yaw90','tilt-yaw126','tilt-yaw180'];offset='offset' in modeName;tilted='tilt' in modeName;p=Path('outputs/actor-inertia-frame-v184')/modeName;p.mkdir(parents=True,exist_ok=True);exe=Path('work/actor-inertia-frame-v184/SourceSteeringWorldProbe').resolve();native=[x for x in map(json.loads,gzip.open('outputs/controller-steering-transform-v180'+('-'+modeName if modeName!='base' else '')+'/accepted-native0.jsonl.gz','rt')) if x['event']=='steering.pose'];base=json.load(open('outputs/scene-composition-v169/final-broad/manifest.json'))['environment'];reports=[]
if tilted:
 assert all(n['rotation_initial']==native[0]['rotation_initial'] for n in native);(p/'initial-rotation.words').write_text(' '.join(map(str,native[0]['rotation_initial']))+'\n')
receipt=json.load(open('work/actor-inertia-frame-v184/source-build.json'));assert receipt['status']=='compiled'
sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
assert receipt['source_sha256']==sha(Path('work/controller-steering-direction-v183/source-world-probe.cpp'))
assert receipt['executable_sha256']==sha(exe), 'Stale Source driver executable'
active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d')
assert all(sha(active/f)==h for f,h in receipt['libraries'].items()), 'Libraries changed after Source driver build'
def cz(a):return [0 if x==0x80000000 else x for x in a]
for mode in ('off','direction-off','on'):
 for rep in range(2):
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env.update(RRR3D_EXPERIMENT_GAMECAR_STEER_V177='1',RRR3D_EXPERIMENT_GAMECAR_WAKE_V171='1',RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173='1',RRR3D_EXPERIMENT_GAMECAR_STEER_DEADBAND_V179='1',RRR3D_EXPERIMENT_GAMECAR_STEER_MATRIX_V181='0' if mode=='off' else '1');env['RRR3D_EXPERIMENT_GAMECAR_STEER_DIRECTION_V183']='1' if mode=='on' else '0';env['RRR3D_EXPERIMENT_NATIVE_ACTOR_INERTIA_FRAME_V184']='1';out=p/f'{mode}{rep}.jsonl';subprocess.run([str(exe),str(out),*([modeName,str(p/'initial-rotation.words')] if tilted else ['offset'] if offset else [])],env=env,check=True,timeout=90);allrows=list(map(json.loads,out.read_text().splitlines()));rows=[x for x in allrows if x['event']=='steering.pose'];commands=[x for x in allrows if x['event']=='steering.command'];assert len(rows)==len(commands)==45;assert all((tilted or c['physical_speed_word']==n['speed_word']) and c['all_wheels_contact'] and c['rear_pivot_word']==3212836864 for n,c in zip(native,commands));bad=[];angles=[]
  for n,s,c in zip(native,rows,commands):
   assert all(cz(n[k])==cz(s[k]) for k in ['position_before','rotation_before']),('initial-state',n['case'])
   d={k:dict(native=n[k],mac=s[k]) for k in ['position_after','rotation_after'] if cz(n[k])!=cz(s[k])}
   if d:bad.append(dict(case=n['case'],fields=d))
   if n['angle_hook_hit'] and cz([n['angle_word']])!=cz([c['yaw_word']]):angles.append(dict(case=n['case'],native=n['angle_word'],mac=c['yaw_word']))
   if mode=='on':assert c['apply_transform']==n['angle_hook_hit']
  reports.append(dict(mode=mode,repeat=rep,cases=45,exact_poses=45-len(bad),differences=bad,angle_differences=angles));print(mode,rep,'exact poses',45-len(bad),'angle differences',len(angles),bad[:1],flush=True)
  if mode=='on':assert not angles and all('rotation_after' not in x['fields'] for x in bad)
  elif mode=='off' and modeName in ['base','offset','tilt']:assert bad and len(angles)==8
  elif mode=='direction-off' and modeName in ['base','offset','tilt']:assert len(angles)==(8 if tilted else 0)
 assert (p/f'{mode}0.jsonl').read_bytes()==(p/f'{mode}1.jsonl').read_bytes()
(p/'comparison.json').write_text(json.dumps(dict(scope='Actual original WheelsProgress plus actual SDK step versus actual Source GameCar plus Jolt step, equal zero COM and exact measured initial states, one car/noAI, four owned wheels, actual contacts. Position, body quaternion and observed original angle all compared as float32; coordinate signed zero canonicalized.',runs=reports,repeats_identical=True,source_probe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),environment_base=base,goal_complete=False),indent=2)+'\n')

assert all(not r['differences'] for r in reports if r['mode']=='on'), 'Unresolved full-pose integration differences retained in comparison.json'
