from pathlib import Path
import json,gzip,subprocess,hashlib
work=Path('work/controller-clutch-v186');out=Path('outputs/controller-clutch-v186');header=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header');cz=lambda a:[0 if x==0x80000000 else x for x in a];reports=[]
for recipe in ['box-only','four-wheels']:
 folder=Path('outputs/controller-stabilize-v185')/('box-only' if recipe=='box-only' else '')
 ns=[x for x in map(json.loads,gzip.open(folder/'accepted-native0.jsonl.gz','rt')) if x['event']=='stabilize.pose'];assert len(ns)==1440
 text=''.join(' '.join(map(str,[n['case'],*n['rotation_before'],*n['inverse_inertia'],*n['omega_before'],*n['damping'],n['roll_word'],n['pitch_word'],int(n['contact']),int(n['clutch_case']!=0),n['clutch_strength_word'],n['mass_word']]))+'\n' for n in ns)
 for opt in ['O0','O2']:
  exe=work/('SourceClutchHelper-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I',str(header),str(work/'source-helper-probe.cpp'),'-o',str(exe)],check=True);prev=None
  for rep in range(2):
   data=subprocess.check_output([str(exe)],input=text.encode());assert prev is None or prev==data;prev=data;(out/f'{recipe}-{opt}-{rep}.jsonl.gz').write_bytes(gzip.compress(data,mtime=0));rows=list(map(json.loads,data.splitlines()));assert len(rows)==1440;bad=[]
   for n,m in zip(ns,rows):
    diff={k:dict(native=n[k],mac=m[k]) for k in ['omega_after','rotation_after'] if cz(n[k])!=cz(m[k])}
    if cz(n['checkpoint2'])!=cz(m['momentum']):diff['momentum']=dict(native=n['checkpoint2'],mac=m['momentum'])
    if diff:bad.append(dict(case=n['case'],clutch=n['clutch_case'],fields=diff))
   reports.append(dict(recipe=recipe,optimization=opt,repeat=rep,cases=1440,exact=1440-len(bad),differences=bad));print(recipe,opt,rep,1440-len(bad),'/1440 exact',flush=True)
(out/'helper-comparison.json').write_text(json.dumps(dict(runs=reports,scope='Candidate arithmetic only, actual original GameCar StabilizeForce. No Source controller/session integration yet. Four clutch modes including zero strength, positive, negative, and unlocked. Two inertia fixtures.',goal_complete=False),indent=2)+'\n');assert all(r['exact']==1440 for r in reports)
