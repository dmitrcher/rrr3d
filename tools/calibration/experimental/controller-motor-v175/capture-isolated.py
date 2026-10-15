from pathlib import Path
import subprocess,json,gzip,hashlib,re
p=Path('outputs/controller-motor-v175/accepted');folder=r'C:\MotorRock\calibration-probes\controller-motor-v175'
def guest(cmd):
 r=subprocess.run(['prlctl','exec','Windows 11','cmd.exe','/c',cmd],capture_output=True,timeout=240)
 if r.returncode:raise RuntimeError((r.stdout+r.stderr).decode(errors='replace'))
 return r.stdout+r.stderr

def sha(f):return re.search(r'(?im)^([a-f0-9]{64})\s*$',guest('certutil -hashfile '+f+' SHA256').decode()).group(1)
files=[r'C:\MotorRock\game-debug\Rock3dGame.dll',r'C:\MotorRock\game-debug\Rock3dGame.pdb',folder+r'\MotorProbeV175.exe',folder+r'\cases.words',folder+r'\PhysXCore.dll',folder+r'\PhysXCooking.dll'];hashes={f:sha(f) for f in files};results=[];previous=None
for rep in [0,1]:
 output=folder+f'\\isolated{rep}.jsonl';guest(folder+r'\MotorProbeV175.exe '+folder+r'\cases.words '+output);data=guest('type '+output);rows=list(map(json.loads,data.decode().splitlines()));samples=[r for r in rows if r['event']=='motor.sample'];old=[r for r in map(json.loads,gzip.open(p/f'native{rep}.jsonl.gz','rt')) if r['event']=='motor.sample'];assert samples==old and len(samples)==13512;contact=[r for r in rows if r['event']=='motor.contact'];assert len(contact)==2 and all(r['dynamic_actors']==1 for r in contact)
 if previous is not None:assert rows==previous
 previous=rows;(p/f'isolated-native{rep}.jsonl.gz').write_bytes(gzip.compress(data,mtime=0));results.append(dict(repeat=rep,states=13512,exact_original_reference_and_inputs=True,one_dynamic_body_per_scene=True));print('Single-body scenes',rep,'all13512 inputs and controller results unchanged',flush=True)
for f,h in hashes.items():assert sha(f)==h
source=Path('work/controller-motor-v175/SourceMotorProbe');old=json.loads((p/'comparison.json').read_text());assert hashlib.sha256(source.read_bytes()).hexdigest()==old['source_probe_sha256']
(p/'isolation-verification.json').write_text(json.dumps(dict(scope='Reference strengthened to independent contact/no-contact SDK scenes with exactly one dynamic actor each. Both full Native state/output histories identical to previously accepted histories; existing compiled Source probe and input/output histories are unchanged and reused.',runs=results,hashes_before_after=hashes,source_probe_unchanged=True,goal_complete=False),indent=2)+'\n')
