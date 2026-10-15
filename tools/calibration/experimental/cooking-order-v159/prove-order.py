from pathlib import Path
import json,subprocess,hashlib
p=Path(__file__).resolve().parent
assert (p/'NativeCookedOrderV159.h').read_bytes()==(p/'after/source/src/Rock3dEngine/header/physics/NativeCookedOrderV159.h').read_bytes()
prior=json.load(open(p/'face-order-proof.json'))['checks'];checks=[]
for mode in ['O0','O2']:
 exe=p/('order-probe-'+mode)
 subprocess.run(['clang++','-std=c++17','-ffp-contract=off','-'+mode,str(p/'order-probe.cpp'),'-o',str(exe)],check=True)
 for file,track in [('raw32.words',0),('raw52.words',64)]:
  actual=list(map(int,subprocess.check_output([str(exe)],input=(p/file).read_bytes()).split()))
  for expected in [r for r in prior if r['track']==track]:
   assert actual==expected['cooked_to_raw'];checks.append(dict(optimization=mode,track=track,repeat=expected['repeat'],triangles=len(actual),all_original_cooked_indices_exact=True))
(p/'cpp-order-proof.json').write_text(json.dumps(dict(checks=checks,geometry_only=True,goal_complete=False),indent=2)+'\n');print('Production helper8 full permutation checks passed')
