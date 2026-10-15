from pathlib import Path
import json,subprocess,gzip
p=Path(__file__).resolve().parent;checks=[]
for opt in [0,2]:
 exe=p/f'check-plane-O{opt}';subprocess.run(['clang++','-std=c++17',f'-O{opt}','-I/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header',str(p/'check-plane.cpp'),'-o',str(exe)],check=True);actual=list(map(int,subprocess.check_output([str(exe)]).split()))
 for track in [16,64]:
  for rep in [0,1]:
   rows=[r for r in map(json.loads,gzip.open(f'outputs/cold-manager-v162/native-initial/track{track}-run{rep}-private.jsonl.gz','rt')) if r.get('function')=='endpoint-sort-before'];expected=[0]*6
   for axis,r in enumerate(rows):
    w=r['sort_endpoints']['words'];idx=len(w)//4-5;keys={w[i+1]:w[i] for i in range(0,len(w),2)}
    for side,key in enumerate([idx|2147483648,idx]):
     v=keys[key];expected[side*3+axis]=v^2147483648 if v&2147483648 else ~v&0xffffffff
   assert actual==expected,(track,rep,opt,actual,expected);checks.append(dict(track=track,repeat=rep,optimization=opt,exact_words=6))
(p/'plane-bounds-proof.json').write_text(json.dumps(dict(scope='Actual original plane SAP inputs, Source bounds derived only from original Core finite extent and FLT_EPSILON operations.',checks=checks,goal_complete=False),indent=2)+'\n');print('all8 plane bounds checks exact')
