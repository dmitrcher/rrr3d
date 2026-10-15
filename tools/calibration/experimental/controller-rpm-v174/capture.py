from pathlib import Path
import subprocess,json,hashlib,gzip,os,re
p=Path('work/controller-rpm-v174');out=Path('outputs/controller-rpm-v174');folder=r'C:\MotorRock\calibration-probes\controller-rpm-v174'
def guest(command):return subprocess.check_output(['prlctl','exec','Windows 11','cmd.exe','/c',command],timeout=90)
def sha(path):return re.search(r'(?im)^([a-f0-9]{64})\s*$',guest('certutil -hashfile '+path+' SHA256').decode()).group(1)
files=[r'C:\MotorRock\game-debug\Rock3dGame.dll',r'C:\MotorRock\game-debug\Rock3dGame.pdb',folder+r'\RpmProbeV174.exe',folder+r'\cases.words'];before={f:sha(f) for f in files};assert before[files[-1]]==hashlib.sha256((p/'cases.words').read_bytes()).hexdigest()
profile=json.loads(Path('outputs/scene-composition-v169/final-broad/manifest.json').read_text())['environment'];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(profile);env.update(RRR3D_EXPERIMENT_GAMECAR_WAKE_V171='1',RRR3D_EXPERIMENT_GAMECAR_TORQUE_V173='1')
checks=[]
for rep in [0,1]:
 output=folder+f'\\rpm{rep}.jsonl';guest(folder+r'\RpmProbeV174.exe '+folder+r'\cases.words '+output);data=guest('type '+output);(out/f'native{rep}.jsonl.gz').write_bytes(gzip.compress(data,mtime=0))
 src=out/f'source{rep}.jsonl';subprocess.run([str((p/'SourceRpmProbe').resolve()),str((p/'cases.words').resolve()),str(src.resolve())],env=env,check=True,timeout=90)
 n=[r for r in map(json.loads,data.decode().splitlines()) if r['event']=='rpm.sample'];s=list(map(json.loads,src.open()));assert len(n)==len(s)==4893;bad=[dict(native=a,source=b) for a,b in zip(n,s) if a!=b];checks.append(dict(repeat=rep,cases=len(n),exact=len(n)-len(bad),differences=bad));print(rep,len(n),'differences',len(bad),bad[:2],flush=True)
for f,h in before.items():assert sha(f)==h
assert gzip.decompress((out/'native0.jsonl.gz').read_bytes())==gzip.decompress((out/'native1.jsonl.gz').read_bytes());assert (out/'source0.jsonl').read_bytes()==(out/'source1.jsonl').read_bytes()
(out/'comparison.json').write_text(json.dumps(dict(scope='Original unchanged DLL CalcRPM at production-observed PC24 vs actual compiled Source GameCar OnFixedStepDrive. Identical finite signed axle words and motor/gear fields; neutral, idle, clamp and float32 neighbors around shift boundaries. This does not claim full TransmissionProgress equivalence.',checks=checks,hashes_before_and_after=before,source_probe_sha256=hashlib.sha256((p/'SourceRpmProbe').read_bytes()).hexdigest(),source_environment={k:v for k,v in env.items() if k.startswith(('RRR3D_','NX_'))},repeats_identical=True,goal_complete=False),indent=2)+'\n')
