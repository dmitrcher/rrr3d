from pathlib import Path
import json,gzip,subprocess,os,hashlib,collections
p=Path('outputs/controller-stabilize-v185');rows=[x for x in map(json.loads,gzip.open(p/'accepted-native0.jsonl.gz','rt')) if x['event']=='stabilize.pose' and x['clutch_case']==0 and not x['contact']];assert len(rows)==180
(p/'world-input.words').write_text(''.join(' '.join(map(str,[n['case'],*n['rotation_before'],*n['omega_before'],*n['damping'],n['roll_word'],n['pitch_word']]))+'\n' for n in rows));base=json.loads(Path('outputs/actor-inertia-frame-v184/base/comparison.json').read_text())['environment_base'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env['RRR3D_EXPERIMENT_NATIVE_ACTOR_INERTIA_FRAME_V184']='1';cz=lambda a:[0 if x==0x80000000 else x for x in a];reports=[]
for mode in ['off','on']:
 env['RRR3D_EXPERIMENT_GAMECAR_STABILIZE_V185']='1' if mode=='on' else '0'
 for rep in range(2):
  trace=p/f'world-{mode}{rep}.stabilize.jsonl';trace.unlink(missing_ok=True);out=p/f'world-{mode}{rep}.jsonl';env['RRR3D_STABILIZE_TRACE_FILE_V113']=str(trace.resolve());subprocess.run(['work/controller-stabilize-v185/SourceStabilizeWorldProbe',str(p/'world-input.words'),str(out)],env=env,check=True,timeout=120);mac=list(map(json.loads,out.read_text().splitlines()));traces=list(map(json.loads,trace.read_text().splitlines()));assert len(mac)==180 and len(traces)==(720 if mode=='on' else 540);bad=[];initial=[]
  for i,(n,m) in enumerate(zip(rows,mac)):
   assert n['case']==m['case'] and m['callbacks']==1
   stride=4 if mode=='on' else 3; inputs=traces[i*stride];applied=traces[i*stride+stride-1];assert inputs['phase']=='input' and applied['phase']=='applied';assert not inputs['contact']
   d={k:dict(native=n[k],mac=m[k]) for k in ['rotation_before','rotation_after_step','momentum_after_step','position_after_step'] if cz(n[k])!=cz(m[k])}
   for key,native,source in [('omega_before',n['omega_before'],inputs['words'][7:10]),('omega_after',n['omega_after'],applied['words'][7:10]),('rotation_after',n['rotation_after'],applied['words'][:4])]:
    if cz(native)!=cz(source):d[key]=dict(native=native,mac=source)
   if cz(n['inverse_inertia'])!=cz(inputs['words'][4:7]):d['inverse_inertia']=dict(native=n['inverse_inertia'],mac=inputs['words'][4:7])
   if mode=='on':
    incoming=traces[i*stride+1];assert incoming['phase']=='game-input-v185'
    if cz(n['momentum_before'])!=cz(incoming['words'][10:13]):d['momentum_before']=dict(native=n['momentum_before'],mac=incoming['words'][10:13])
   if any(k in d for k in ['rotation_before','momentum_before','omega_before','inverse_inertia']):initial.append(n['case'])
   if d:bad.append(dict(case=n['case'],orientation=n['orientation_case'],damping=n['damping_case'],fields=d))
  reports.append(dict(mode=mode,repeat=rep,cases=180,exact=180-len(bad),initial_mismatches=initial,differences=bad));print(mode,rep,'exact',180-len(bad),'initial',len(initial),'field counts',dict(collections.Counter(k for b in bad for k in b['fields'])),flush=True)
  if rep:assert out.read_bytes()==(p/f'world-{mode}0.jsonl').read_bytes() and trace.read_bytes()==(p/f'world-{mode}0.stabilize.jsonl').read_bytes()
(p/'world-comparison.json').write_text(json.dumps(dict(scope='Real current Jolt world fixed-step stabilization vs actual original GameCar StabilizeForce and SDK step. One car, four free wheels, no contacts/AI, gravity/damping/motor/brake zero. Clutch excluded. Exact incoming quaternion, inverse tensor, omega and momentum checked.',runs=reports,repeats_identical=True,source_probe_sha256=hashlib.sha256(Path('work/controller-stabilize-v185/SourceStabilizeWorldProbe').read_bytes()).hexdigest(),goal_complete=False),indent=2)+'\n');(p/'world-environment.json').write_text(json.dumps({k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},indent=2)+'\n')
assert all(not r['initial_mismatches'] for r in reports),'Initial physical states differ; do not attribute final differences to stabilization'
assert all(not r['differences'] for r in reports if r['mode']=='on'),'World parity remains open'
