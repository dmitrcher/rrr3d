from pathlib import Path
import argparse,hashlib,gzip,json,os,subprocess
parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path,default=Path('verification'));args=parser.parse_args();root=Path(__file__).resolve().parent;manifest=json.loads((root/'manifest.json').read_text())
for name,expected in manifest['files'].items():
 rel=Path(name);assert not rel.is_absolute() and '..' not in rel.parts;assert hashlib.sha256((root/rel).read_bytes()).hexdigest()==expected,('checksum',name)
args.output.mkdir(parents=True,exist_ok=True);native=[n for n in map(json.loads,gzip.open(root/'native.jsonl.gz','rt')) if n['event']=='stabilize.pose' and n['clutch_case']==0 and not n['contact']];assert len(native)==180
inputs=args.output/'input.words';inputs.write_text(''.join(' '.join(map(str,[n['case'],*n['rotation_before'],*n['omega_before'],*n['damping'],n['roll_word'],n['pitch_word']]))+'\n' for n in native));env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'profile.json').read_text()));env['RRR3D_CALIBRATION_PROFILE']=str(root/'windows-source-v14.cfg');cz=lambda a:[0 if x==0x80000000 else x for x in a];reports=[]
for rep in range(2):
 trace=args.output/f'run{rep}.stabilize.jsonl';trace.unlink(missing_ok=True);capture=args.output/f'run{rep}.jsonl';env['RRR3D_STABILIZE_TRACE_FILE_V113']=str(trace.resolve());subprocess.run([str(root/'SourceStabilizeWorldProbe'),str(inputs.resolve()),str(capture.resolve())],env=env,check=True,timeout=120);mac=list(map(json.loads,capture.read_text().splitlines()));traces=list(map(json.loads,trace.read_text().splitlines()));assert len(mac)==180 and len(traces)==720
 for i,(n,m) in enumerate(zip(native,mac)):
  assert n['case']==m['case'] and m['callbacks']==1;incoming,applied=traces[i*4+1],traces[i*4+3];assert incoming['phase']=='game-input-v185' and applied['phase']=='applied'
  for key in ['rotation_before','rotation_after_step','momentum_after_step','position_after_step']:assert cz(n[key])==cz(m[key]),(rep,n['case'],key)
  for key,nv,mv in [('inverse_inertia',n['inverse_inertia'],incoming['words'][4:7]),('omega_before',n['omega_before'],incoming['words'][7:10]),('momentum_before',n['momentum_before'],incoming['words'][10:13]),('omega_after',n['omega_after'],applied['words'][7:10]),('rotation_after',n['rotation_after'],applied['words'][:4])]:assert cz(nv)==cz(mv),(rep,n['case'],key,nv,mv)
 if rep:assert capture.read_bytes()==(args.output/'run0.jsonl').read_bytes() and trace.read_bytes()==(args.output/'run0.stabilize.jsonl').read_bytes()
 reports.append(dict(repeat=rep,exact_states=180));print(rep,'180/180 stabilization plus solver states exact',flush=True)
(args.output/'report.json').write_text(json.dumps(dict(runs=reports,repeats_identical=True,full_goal_complete=False,scope=manifest['scope']),indent=2)+'\n')
