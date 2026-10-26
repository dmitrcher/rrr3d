from pathlib import Path
import json,gzip,subprocess,os,hashlib,collections
p=Path('outputs/controller-clutch-v186');rows=[x for x in map(json.loads,gzip.open(Path('outputs/controller-stabilize-v185/accepted-native0.jsonl.gz'),'rt')) if x['event']=='stabilize.pose' and not x['contact']];assert len(rows)==720
(p/'world-input.words').write_text(''.join(' '.join(map(str,[n['case'],*n['rotation_before'],*n['omega_before'],*n['damping'],n['roll_word'],n['pitch_word'],int(n['clutch_case']!=0),n['clutch_strength_word']]))+'\n' for n in rows));base=json.loads(Path('outputs/actor-inertia-frame-v184/base/comparison.json').read_text())['environment_base'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env['RRR3D_EXPERIMENT_NATIVE_ACTOR_INERTIA_FRAME_V184']='1';cz=lambda a:[0 if x==0x80000000 else x for x in a];reports=[]
for mode in ['off','on']:
 env['RRR3D_EXPERIMENT_GAMECAR_STABILIZE_V185']='1';env['RRR3D_EXPERIMENT_GAMECAR_CLUTCH_V186']='1' if mode=='on' else '0'
 for rep in range(2):
  trace=p/f'world-{mode}{rep}.stabilize.jsonl';trace.unlink(missing_ok=True);out=p/f'world-{mode}{rep}.jsonl';env['RRR3D_STABILIZE_TRACE_FILE_V113']=str(trace.resolve());subprocess.run(['work/controller-clutch-v186/SourceClutchWorldProbe',str(p/'world-input.words'),str(out)],env=env,check=True,timeout=120);mac=list(map(json.loads,out.read_text().splitlines()));traces=list(map(json.loads,trace.read_text().splitlines()));assert len(mac)==720 and len(traces)==2880;bad=[];initial=[]
  for i,(n,m) in enumerate(zip(rows,mac)):
   assert n['case']==m['case'] and m['callbacks']==1
   stride=4; inputs=traces[i*stride];applied=traces[i*stride+stride-1];assert inputs['phase']=='input' and applied['phase']=='applied';assert not inputs['contact']
   d={k:dict(native=n[k],mac=m[k]) for k in ['rotation_before','rotation_after_step','momentum_after_step','position_after_step'] if cz(n[k])!=cz(m[k])}
   for key,native,source in [('omega_before',n['omega_before'],inputs['words'][7:10]),('omega_after',n['omega_after'],applied['words'][7:10]),('rotation_after',n['rotation_after'],applied['words'][:4])]:
    if cz(native)!=cz(source):d[key]=dict(native=native,mac=source)
   if cz(n['inverse_inertia'])!=cz(inputs['words'][4:7]):d['inverse_inertia']=dict(native=n['inverse_inertia'],mac=inputs['words'][4:7])
   if True:
    incoming=traces[i*stride+1];assert incoming['phase']=='game-input-v185'
    if cz(n['momentum_before'])!=cz(incoming['words'][10:13]):d['momentum_before']=dict(native=n['momentum_before'],mac=incoming['words'][10:13])
   if any(k in d for k in ['rotation_before','momentum_before','omega_before','inverse_inertia']):initial.append(n['case'])
   if d:bad.append(dict(case=n['case'],orientation=n['orientation_case'],damping=n['damping_case'],fields=d))
  reports.append(dict(mode=mode,repeat=rep,cases=720,exact=720-len(bad),initial_mismatches=initial,differences=bad));print(mode,rep,'exact',720-len(bad),'initial',len(initial),'field counts',dict(collections.Counter(k for b in bad for k in b['fields'])),flush=True)
  if rep:assert out.read_bytes()==(p/f'world-{mode}0.jsonl').read_bytes() and trace.read_bytes()==(p/f'world-{mode}0.stabilize.jsonl').read_bytes()
(p/'world-comparison.json').write_text(json.dumps(dict(scope='Real Source GameCar OnFixedStepDrive -> Jolt stabilizer vs original StabilizeForce/SDK integration. Four clutch modes, no contacts/AI, identical body inputs, dt1/60. Timer expiry is not covered by this positive-interval matrix.',runs=reports,repeats_identical=True,source_probe_sha256=hashlib.sha256(Path('work/controller-clutch-v186/SourceClutchWorldProbe').read_bytes()).hexdigest(),goal_complete=False),indent=2)+'\n');(p/'world-environment.json').write_text(json.dumps({k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},indent=2)+'\n')
assert all(not r['initial_mismatches'] for r in reports),'Initial physical states differ; do not attribute final differences to stabilization'
assert all(not r['differences'] for r in reports if r['mode']=='on'),'World parity remains open'
