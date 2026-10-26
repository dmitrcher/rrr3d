from pathlib import Path
import argparse,gzip,hashlib,importlib.util,json,os,subprocess

p=argparse.ArgumentParser();p.add_argument('--output',type=Path,default=Path('verification'));args=p.parse_args()
root=Path(__file__).resolve().parent;manifest=json.loads((root/'manifest.json').read_text())
for name,expected in manifest['files'].items():
    rel=Path(name);assert not rel.is_absolute() and '..' not in rel.parts
    assert hashlib.sha256((root/rel).read_bytes()).hexdigest()==expected,('checksum',name)
args.output.mkdir(parents=True,exist_ok=False)
spec=importlib.util.spec_from_file_location('words',root/'compare-float32.py');words=importlib.util.module_from_spec(spec);spec.loader.exec_module(words)
names={'gravity':'0','game-air-gravity':'-20','game-air-pitch':'-20'};native={n:[] for n in names}
for r in map(json.loads,gzip.open(root/'native-sdk.jsonl.gz','rt')):
    if r['event']=='calibration.sample':native[r['detail']].append(r['sample'])
assert all(len(r)==120 for r in native.values())
profile=json.loads((root/'profile.json').read_text());reports=[]
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_','MOTORROCK_JUMP_'))};env.update(profile);env['RRR3D_CALIBRATION_PROFILE']=str(root/'windows-source-v14.cfg');env['RRR3D_EXPERIMENT_GAMECAR_JUMP_V187']='1'
for name,gravity in names.items():
    previous=None
    for rep in range(2):
        env['MOTORROCK_JUMP_SCENE_GRAVITY']=gravity;out=args.output/f'{name}-{rep}.jsonl'
        with out.with_suffix('.log').open('w') as log:subprocess.run([str(root/'SourceJumpGravityProbe'),str(out.resolve()),'list:'+name],env=env,stdout=log,stderr=log,check=True,timeout=120)
        samples=[r['sample'] for r in map(json.loads,out.read_text().splitlines()) if r['event']=='calibration.sample'];assert len(samples)==120
        physical=[words.physical(r) for r in samples]
        assert physical==[words.physical(r) for r in native[name]],(name,rep,'physical words')
        if previous is not None:assert physical==previous
        previous=physical;reports.append(dict(case=name,repeat=rep,exact_states=120))
        print(name,rep,'120/120 SDK trajectory states exact',flush=True)
for binary,count in [('SourceAccelerationLifetimeProbe',7),('SourceJumpGatesProbe',360)]:
    previous=None
    for rep in range(2):
        result=subprocess.run([str(root/binary)],env=env,capture_output=True,check=True,timeout=120)
        rows=list(map(json.loads,result.stdout.splitlines()));assert len(rows)==count
        if previous is not None:assert result.stdout==previous
        previous=result.stdout;(args.output/f'{binary}-{rep}.jsonl').write_bytes(result.stdout)
        if binary=='SourceAccelerationLifetimeProbe':assert all(r['force_reset'] and r['restore'] and r['reset_motion'] and r['mass_independent'] for r in rows)
    print(binary,count,'cases passed twice',flush=True)
(args.output/'report.json').write_text(json.dumps(dict(scope=manifest['scope'],sdk_trajectory_runs=reports,source_gate_cases=360,acceleration_lifetime_masses=7,repeats_identical=True,native_gamecar_runtime_observed=False,full_goal_complete=False),indent=2)+'\n')
