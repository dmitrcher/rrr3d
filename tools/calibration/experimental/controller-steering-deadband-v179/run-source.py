from pathlib import Path
import subprocess,os,json,struct,gzip,hashlib
p=Path('outputs/controller-steering-deadband-v179');p.mkdir(exist_ok=True);exe=Path('work/controller-steering-deadband-v179/SourceSteeringWorldProbe').resolve();native=[x for x in map(json.loads,gzip.open('outputs/controller-steering-pose-v178/accepted-native0.jsonl.gz','rt')) if x['event']=='steering.pose'];base=json.load(open('outputs/scene-composition-v169/final-broad/manifest.json'))['environment'];reports=[];f=lambda u:struct.unpack('<f',struct.pack('<I',u))[0]
def cz(a):return [0 if x==0x80000000 else x for x in a]
for mode in ('off','on'):
 for rep in range(2):
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env.update(RRR3D_EXPERIMENT_GAMECAR_STEER_V177='1',RRR3D_EXPERIMENT_GAMECAR_WAKE_V171='1',RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173='1',RRR3D_EXPERIMENT_GAMECAR_STEER_DEADBAND_V179='1' if mode=='on' else '0');out=p/f'{mode}{rep}.jsonl';subprocess.run([str(exe),str(out)],env=env,check=True,timeout=90);allrows=list(map(json.loads,out.read_text().splitlines()));rows=[x for x in allrows if x['event']=='steering.pose'];commands=[x for x in allrows if x['event']=='steering.command'];assert len(rows)==len(commands)==45;assert all(c['physical_speed_word']==n['speed_word'] and c['all_wheels_contact'] and c['any_wheel_contact'] and c['rear_pivot_word']==3212836864 for n,c in zip(native,commands));dead=[];other=[];initial=[]
  for n,s in zip(native,rows):
   before=all(cz(n[k])==cz(s[k]) for k in ['position_before','rotation_before']);after=all(cz(n[k])==cz(s[k]) for k in ['position_after','rotation_after'])
   if not before:initial.append(n['case'])
   if 0<abs(f(n['speed_word']))<1 and n['input_word']!=0:
    assert before;assert n['rotation_before']==n['rotation_after'];dead.append(dict(case=n['case'],exact_rotation=cz(n['rotation_after'])==cz(s['rotation_after']),exact_pose=after,native=n,source=s))
   elif not after:other.append(n['case'])
  assert len(dead)==8 and sum(x['exact_rotation'] for x in dead)==(8 if mode=='on' else 0)
  reports.append(dict(mode=mode,repeat=rep,deadband_cases=8,deadband_rotation_exact=sum(x['exact_rotation'] for x in dead),cases=dead,other_pose_differences_not_closed=other,initial_state_mismatch_excluded=initial));print(mode,rep,'deadband rotation exact',sum(x['exact_rotation'] for x in dead),'other differences',len(other),flush=True)
 for suffix in ['.jsonl']:assert (p/f'{mode}0{suffix}').read_bytes()==(p/f'{mode}1{suffix}').read_bytes()
# Every pose outside the deadband stays unchanged, preserving the next investigation.
a=list(map(json.loads,(p/'off0.jsonl').read_text().splitlines()));b=list(map(json.loads,(p/'on0.jsonl').read_text().splitlines()));outside=[i for i,n in enumerate(native) if abs(f(n['speed_word']))>=1 or n['speed_word']==0];assert all(a[i*2:i*2+2]==b[i*2:i*2+2] for i in outside)
(p/'comparison.json').write_text(json.dumps(dict(scope='Actual original Windows WheelsProgress body pose versus actual Source GameCar -> Jolt world with four owned logical/physical wheels; eight nonzero-speed deadband rotation cases isolated; translation still differs and is not accepted as fixed. Broader pose differences remain open. Float32 words compared, coordinate signed zero canonicalized only. No AI.',runs=reports,repeats_identical=True,outside_deadband_unchanged=True,source_probe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),source_environment={**base,'RRR3D_EXPERIMENT_GAMECAR_STEER_V177':'1','RRR3D_EXPERIMENT_GAMECAR_WAKE_V171':'1','RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173':'1'},goal_complete=False),indent=2)+'\n')
