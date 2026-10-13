from pathlib import Path
import json,os,subprocess,gzip,math,struct,hashlib,importlib.util
root=Path.cwd();out=root/'outputs/pair-material-v76/focused';out.mkdir(exist_ok=False)
probe=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/Debug/rrr3d_physics_calibration')
assert all(v in probe.read_bytes() for v in [b'RRR3D_EXPERIMENT_NATIVE_PAIR_BOX_CACHE_V73',b'RRR3D_EXPERIMENT_NATIVE_PAIR_SOLVER_V73',b'RRR3D_EXPERIMENT_NATIVE_PAIR_FRAME_V74',b'RRR3D_EXPERIMENT_NATIVE_PAIR_INERTIA_V74',b'RRR3D_EXPERIMENT_NATIVE_PAIR_LEVERS_V74',b'RRR3D_EXPERIMENT_NATIVE_PAIR_MASS_V74',b'RRR3D_EXPERIMENT_NATIVE_PAIR_CLIP_V74',b'RRR3D_EXPERIMENT_NATIVE_PAIR_LOCAL_CACHE_V75',b'RRR3D_EXPERIMENT_NATIVE_PAIR_MATERIAL_TRANSPORT_V75',b'RRR3D_EXPERIMENT_NATIVE_PAIR_ERROR_V75',b'RRR3D_EXPERIMENT_NATIVE_PAIR_RAW_LOCAL_V76',b'RRR3D_EXPERIMENT_NATIVE_PAIR_MATERIAL_LOCAL_V76']),'Stale probe'
baseline=json.load(open(root/'outputs/pair-local-cache-v75/focused/manifest.json'));base={**baseline['environment'],**baseline['candidate_changes']}
names=list(json.load(open(root/'outputs/physics-v70-pair-onset.json'))['cases'])
def load(path):
 data={n:{'samples':[],'meta':None,'contacts':[]} for n in names}
 stream=gzip.open(str(path)+'.gz','rt') if not path.exists() else path.open()
 with stream:
  for l in stream:
   x=json.loads(l);n=x.get('detail')
   if n not in data:continue
   if x['event']=='calibration.sample':data[n]['samples'].append(x['sample'])
   elif x['event']=='calibration.begin':data[n]['meta']=x['sample']
   elif x['event']=='calibration.body-impulse':data[n]['contacts'].append(x['sample'])
 for n,d in data.items():
  assert d['meta'] and len(d['samples'])==d['meta']['steps'],n
  d['contacts'].sort(key=lambda x:(x['tick'],json.dumps(x,sort_keys=True)))
 return data
spec=importlib.util.spec_from_file_location('words',root/'work/sync-rrr3d/tools/calibration/experimental/native-angular-v49/compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
ref=load(root/'outputs/calibration-v14-source-windows.jsonl');saved=load(root/'outputs/pair-local-cache-v75/focused/full-0.jsonl')
dist=lambda a,b:math.sqrt(sum((x-y)**2 for x,y in zip(a,b)))
def angle(a,b):
 ax,ay,az,aw=a;bx,by,bz,bw=b
 v=(aw*bx-bw*ax-ay*bz+az*by,aw*by-bw*ay-az*bx+ax*bz,aw*bz-bw*az-ax*by+ay*bx)
 return 2*math.atan2(math.sqrt(sum(x*x for x in v)),abs(sum(x*y for x,y in zip(a,b))))*180/math.pi
report={'scope':'34 paired observable trajectories: fixed scripted commands, no AI; identical source parameters. V76 original local-getter and material-local scalar associations tested separately and together; source dimensions and parameters unchanged.','cases':{},'goal_complete':False};captures=[];mode_changes={}
for mode,changes in [('control',{}),('raw',{'RRR3D_EXPERIMENT_NATIVE_PAIR_RAW_LOCAL_V76':'1'}),('material',{'RRR3D_EXPERIMENT_NATIVE_PAIR_MATERIAL_LOCAL_V76':'1'}),('full',{'RRR3D_EXPERIMENT_NATIVE_PAIR_RAW_LOCAL_V76':'1','RRR3D_EXPERIMENT_NATIVE_PAIR_MATERIAL_LOCAL_V76':'1'})]:
 mode_changes[mode]=changes
 env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env.update(changes);runs=[]
 for rep in range(2):
  path=out/f'{mode}-{rep}.jsonl'
  with (out/f'{mode}-{rep}.log').open('wb') as log:subprocess.run([str(probe),str(path),'list:'+','.join(names)],env=env,stdout=log,stderr=log,check=True,timeout=180)
  runs.append(load(path));captures.append({'mode':mode,'repeat':rep,'file':path.name,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()});print(mode,rep,'captured',flush=True)
 assert runs[0]==runs[1],mode
 if mode=='control':assert runs[0]==saved,'Disabled flags must preserve V75'
 for n in names:
  a,b=ref[n]['samples'],runs[0][n]['samples'];dif=[i for i,(x,y) in enumerate(zip(a,b)) if words.physical(x)!=words.physical(y)]
  d={'position_max':max(dist(x['position'],y['position']) for x,y in zip(a,b)),'angular_momentum_max':max(dist(x['angular_momentum'],y['angular_momentum']) for x,y in zip(a,b)),'velocity_max':max(dist(x['velocity'],y['velocity']) for x,y in zip(a,b)),'orientation_max':max(angle(x['rotation'],y['rotation']) for x,y in zip(a,b)),'first_difference':dif[0] if dif else None,'exact':not dif,'wheel_contact_mismatches':sum(u['contact']!=v['contact'] for x,y in zip(a,b) for u,v in zip(x['wheels'],y['wheels'])),'first_body_contact':next((r['tick'] for r in runs[0][n]['contacts']),None)}
  report['cases'].setdefault(n,{})[mode]=d
  print(mode,n,d,flush=True)
 (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
(out/'manifest.json').write_text(json.dumps({'environment':base,'candidate_changes':mode_changes['full'],'mode_changes':mode_changes,'names':names,'probe_sha256':hashlib.sha256(probe.read_bytes()).hexdigest(),'two_repeats_exact':True,'control_preserves_v75':True,'captures':captures},indent=2)+'\n')
