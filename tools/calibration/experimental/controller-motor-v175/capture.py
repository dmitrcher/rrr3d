from pathlib import Path
import subprocess,json,hashlib,gzip,os,re
p=Path('work/controller-motor-v175');out=Path('outputs/controller-motor-v175/accepted');out.mkdir(parents=True,exist_ok=False);folder=r'C:\MotorRock\calibration-probes\controller-motor-v175'
def guest(cmd):
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=240)
 if r.returncode:raise RuntimeError((r.stdout+r.stderr).decode(errors='replace'))
 return r.stdout+r.stderr

def sha(path):return re.search(r'(?im)^([a-f0-9]{64})\s*$',guest('certutil -hashfile '+path+' SHA256').decode()).group(1)
files=[r'C:\MotorRock\game-debug\Rock3dGame.dll',r'C:\MotorRock\game-debug\Rock3dGame.pdb',folder+r'\MotorProbeV175.exe',folder+r'\cases.words',folder+r'\PhysXCore.dll',folder+r'\PhysXCooking.dll'];hashes={f:sha(f) for f in files};assert hashes[files[3]]==hashlib.sha256((p/'cases.words').read_bytes()).hexdigest();assert hashes[files[4]]=='c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb';assert hashes[files[5]]=='2e5657ae13de0a0c79c56453b8c73ba3978069d55440134873c6534bbada522b'
base=json.loads(Path('outputs/scene-composition-v169/final-broad/manifest.json').read_text())['environment'];report=[];previous=None
for rep in [0,1]:
 path=folder+f'\\matrix{rep}.jsonl';guest(folder+r'\MotorProbeV175.exe '+folder+r'\cases.words '+path);data=guest('type '+path);rows=list(map(json.loads,data.decode().splitlines()));samples=[r for r in rows if r['event']=='motor.sample'];assert len(samples)==13512;assert [r['case'] for r in samples]==list(range(13512))
 if previous is not None:assert rows==previous
 previous=rows;(out/f'native{rep}.jsonl.gz').write_bytes(gzip.compress(data,mtime=0));print('Native',rep,len(samples),'complete',flush=True)
 # Only measured INPUT state and setup parameters cross this bridge. No native result is sent to Source.
 keys=['case','contact','maximum_torque_word','automatic','initial_gear','move','velocity_word','axle_word','scale_word'];words='MOTORROCK_MOTOR_STATE_V175\n'+str(len(samples))+'\n'+''.join(' '.join(str(r[k]) for k in keys)+'\n' for r in samples);inputfile=out/f'inputs{rep}.words';inputfile.write_text(words)
 for mode in ['off','on']:
  env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(base);env['RRR3D_EXPERIMENT_GAMECAR_WAKE_V171']='1';env['RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173']='1' if mode=='on' else '0';source=out/f'{mode}{rep}.jsonl';subprocess.run([str((p/'SourceMotorProbe').resolve()),str(inputfile.resolve()),str(source.resolve())],env=env,check=True,timeout=90)
  sourceRows=list(map(json.loads,source.open()));assert len(sourceRows)==len(samples);keys=['case','speed_word','motor_word','brake_word','rpm_word','gear'];bad=[dict(case=a['case'],native={k:a[k] for k in keys},source=b) for a,b in zip(samples,sourceRows) if {k:a[k] for k in keys}!={k:b[k] for k in keys}];report.append(dict(mode=mode,repeat=rep,cases=len(samples),exact=len(samples)-len(bad),differences=bad));print('Source',mode,rep,'differences',len(bad),bad[:2],flush=True)
for f,h in hashes.items():assert sha(f)==h
for mode in ['off','on']:assert (out/f'{mode}0.jsonl').read_bytes()==(out/f'{mode}1.jsonl').read_bytes()
(out/'comparison.json').write_text(json.dumps(dict(scope='Actual original GameCar MotorProgress and TransmissionProgress with actual original constructed wheel collection and wrapper bound to a real SDK wheel, versus actual compiled Source OnFixedStepDrive. SDK axle override isolates controller input; contact/no-contact are physically queried. Only speed, torque, brake, RPM and final gear compared. WheelsProgress, stabilization, jumping and actual race trajectory remain open.',checks=report,original_and_sdk_hashes_before_after=hashes,source_probe_sha256=hashlib.sha256((p/'SourceMotorProbe').read_bytes()).hexdigest(),source_environment_profile=base,source_input_fields_only=['case','contact','maximum_torque_word','automatic','initial_gear','move','velocity_word','axle_word','scale_word'],repeats_identical=True,goal_complete=False),indent=2)+'\n')
assert all(r['exact']==r['cases'] for r in report if r['mode']=='on'), 'Controller difference remains'
