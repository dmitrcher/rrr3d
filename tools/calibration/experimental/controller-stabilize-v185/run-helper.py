from pathlib import Path
import subprocess,json,gzip,hashlib
p=Path('outputs/controller-stabilize-v185');work=Path('work/controller-stabilize-v185');header=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header');cz=lambda a:[0 if x==0x80000000 else x for x in a];reports=[]
for recipe in ['box-only','four-wheels']:
 folder=p/'box-only' if recipe=='box-only' else p
 native=[n for n in map(json.loads,gzip.open(folder/'accepted-native0.jsonl.gz','rt')) if n['event']=='stabilize.pose' and n['clutch_case']==0];assert len(native)==360
 text=''.join(' '.join(map(str,[n['case'],*n['rotation_before'],*n['inverse_inertia'],*n['omega_before'],*n['damping'],n['roll_word'],n['pitch_word'],int(n['contact'])]))+'\n' for n in native)
 for mode in ['before','production-O0','production-O2']:
  exe=work/('SourceStabilizeHelper-'+mode);source=work/('source-helper-before.cpp' if mode=='before' else 'source-helper-probe.cpp');subprocess.run(['clang++','-std=c++17','-O0' if mode.endswith('O0') else '-O2','-ffp-contract=off','-I',str(header),str(source),'-o',str(exe)],check=True)
  previous=None
  for rep in range(2):
   r=subprocess.run([str(exe)],input=text,capture_output=True,text=True,check=True);data=r.stdout.encode();assert previous is None or previous==data;previous=data;out=p/f'helper-{recipe}-{mode}-{rep}.jsonl.gz';out.write_bytes(gzip.compress(data,mtime=0));rows=list(map(json.loads,r.stdout.splitlines()));assert len(rows)==len(native);bad=[]
   for n,m in zip(native,rows):
    assert n['case']==m['case'];diff={k:dict(native=n[k],mac=m[k]) for k in ['omega_after','rotation_after'] if cz(n[k])!=cz(m[k])}
    if diff:bad.append(dict(case=n['case'],fields=diff))
    if mode!='before':assert cz(n['checkpoint2'])==cz(m['momentum']),('pre-setter momentum',recipe,n['case'])
   reports.append(dict(recipe=recipe,mode=mode,repeat=rep,cases=360,exact=360-len(bad),differences=bad,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),exe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest()));print(recipe,mode,rep,360-len(bad),'/360 exact',flush=True)
(p/'helper-final-comparison.json').write_text(json.dumps(dict(runs=reports,production_header_sha256=hashlib.sha256((header/'physics/NativeGameStabilizeV185.h').read_bytes()).hexdigest(),scope='Exact original pre-setter world momentum, post-setter quaternion and angular velocity from actual GameCar at PC24; one box-only and one four-wheel inertia fixture. Clutch excluded. O0/O2 and repeated outputs checked.',goal_complete=False),indent=2)+'\n');assert all(r['exact']==360 for r in reports if r['mode']!='before')
