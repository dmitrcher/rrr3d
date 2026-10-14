import struct,json,gzip,subprocess
from pathlib import Path
root=Path.cwd();bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];value=lambda u:struct.unpack('<f',struct.pack('<I',u))[0];checks=[];zero=0
for rep in (0,1):
 rows=list(map(json.loads,gzip.open(f'outputs/energy-sleep-v127/native-attempt2/run{rep}-private.jsonl.gz','rt')))
 car=[x for x in rows if 'energy_body' in x];frames={}
 for x in car:frames.setdefault((x['edi'],x['sample_tick_v71']),{})[x['function']]=x
 inputs=[];expected=[]
 for tick,d in frames.items():
  w=d['energy-sleep-entry']['energy_body']['words'];dt=d['energy-wake-decrement']['stack']['words'][15]
  inp=w[45:48]+w[60:63]+w[48:52]+w[63:67]+w[121:124]+w[120:121]+w[95:96]+[dt,sum(w[8:12])]+w[97:103]+w[103:104]+[0,0];assert len(inp)==30
  inputs.append(inp);out=d['energy-wake-store'];state=out['energy_body']['words'];exp=state[97:103]+[out['xmm_words'][0][0]]
  if 'energy-threshold-test' in d:
   test=d['energy-threshold-test'];energy=test['xmm_words'][0][0];thr=test['xmm_words'][1][0];exp=[energy,thr]+exp
  else:exp=[0,0]+exp
  exp.append(bool(d['energy-sleep-exit']['energy_body']['words'][84]&0x100000));expected.append(exp)
 inc='/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics'
 for opt in ('O0','O2'):
  exe=root/f'work/energy-sleep-v127/check-energy-{opt}';subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+inc,'work/energy-sleep-v127/check-energy.cpp','-o',str(exe)],check=True)
  result=[list(map(int,x.split())) for x in subprocess.check_output([str(exe)],input='\n'.join(' '.join(map(str,x)) for x in inputs)+'\n',text=True).splitlines()];fail=[]
  for (tick,d),actual,exp in zip(frames.items(),result,expected):
   selected=actual[6:15]+[actual[17]]
   for i,(a,b) in enumerate(zip(selected,exp)):
    if a!=b and (a&0x7fffffff or b&0x7fffffff):fail.append(dict(tick=tick,field=i,Source=value(a) if i<9 else bool(a),Native=value(b) if i<9 else bool(b)))
  checks.append(dict(repeat=rep,optimization=opt,frames=len(frames),failure_count=len(fail),failures=fail));print(rep,opt,len(fail),fail[:8])
Path('work/energy-sleep-v127/energy-helper-proof.json').write_text(json.dumps(dict(scope='Actual Native per-step input -> standalone scalar energy/accumulation/wake model; no runtime state correction. General scene interaction counts and sleep commit still unimplemented.',checks=checks,goal_complete=False),indent=2)+'\n')
