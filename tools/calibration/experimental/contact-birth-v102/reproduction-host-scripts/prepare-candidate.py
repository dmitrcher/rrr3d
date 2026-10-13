from pathlib import Path
import json,gzip,struct,subprocess,os,hashlib
root=Path.cwd();w=root/'work/contact-birth-v102';inc=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics');p=inc/'NativeContactOrderV79.h';before=p.read_text();s=(w/'NativeContactOrderCandidateV102.h').read_text();assert 'RRR3D_EXPERIMENT_NATIVE_INITIAL_MAX_SCAN_V102' not in before
flt=lambda word:struct.unpack('<f',struct.pack('<I',word))[0];decode=lambda v:flt(v & 0x7fffffff if v & 0x80000000 else (~v)&0xffffffff);fixtures=[]
for name in ['slope-lateral','slope-longitudinal','settle']:
 for rep in range(2):
  n=list(map(json.loads,gzip.open(root/f'outputs/contact-birth-v102/native-table19/endpoint-{name}-19-{rep}-private.jsonl.gz','rt')));api=next((x for x in n if x.get('function')=='broadphase-created-api'),None)
  if api is None:continue
  tick=25 if name=='settle' else 19;m=list(map(json.loads,(root/f'outputs/contact-birth-v102/mac-order/{name}-{tick}-{rep}-order.jsonl').open()));inputbounds={i:[[None]*3,[None]*3] for i in range(1,7)}
  for axis in range(3):
   native=api[f'initial_axis_{axis}']['words'];native=[(a,b) for a,b in zip(native[::2],native[1::2]) if b&0x7fffffff];mac=m[axis]['words'];assert native==list(zip(mac[::2],mac[1::2]))
   for value,owner in native:inputbounds[owner&0x7fffffff][0 if owner&0x80000000 else 1][axis]=decode(value)
  inserts=[x for x in n if x.get('function')=='adjacency-insert'];pairs=[x['stack']['words'][1:3] for x in inserts];assert all(x['stack']['words'][29]==2 for x in inserts)
  publication=next(x for x in n if x.get('function')=='broadphase-created-list');words=publication['created_pairs']['words'];published=[[(words[i]&0x3ffff)+1,(words[i+1]&0x3ffff)+1] for i in range(0,len(words),4)];assert [sorted(p) for p in published]==[sorted(p) for p in pairs]
  fixtures.append({'case':name,'repeat':rep,'bounds':inputbounds,'actual_Native_insert_pairs':pairs,'actual_Native_published_pairs':published,'actual_axes_all_exact_Mac':True,'actual_Native_primary_axis':2})
assert len(fixtures)==4
lines=[]
for f in fixtures:
 lines.append('6')
 for i in range(1,7):
  b=f['bounds'][i];lines.append(' '.join(map(str,[0 if i==1 else 1,int(i>=3)]+sum([[b[0][a],b[1][a]] for a in range(3)],[]))))
data='\n'.join(lines)+'\n';runs=[];env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))}
for opt in ['O0','O2']:
 exe=w/f'birth-{opt}';subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-I'+str(inc),str(w/'birth-probe.cpp'),'-o',str(exe)],check=True)
 def run(e):return [list(map(int,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode(),env=e).decode().splitlines()]
 new=run(dict(env,RRR3D_EXPERIMENT_NATIVE_INITIAL_MAX_SCAN_V102='1'));old=run(env)
 for row,f in zip(new,fixtures):
  actual=[row[i:i+3] for i in range(0,len(row),3)];expected=[sorted(pair)+[i+1] for i,pair in enumerate(f['actual_Native_published_pairs'])];assert sorted(actual)==sorted(expected),(actual,expected)
 assert old!=new;runs.append({'optimization':opt,'actual_Native_published_pair_ranks_exact':20,'old_forward_minimum_ranks_differ':True})
(root/'outputs/contact-birth-v102/same-input-birth-proof.json').write_text(json.dumps({'scope':'Actual original Windows endpoint arrays all3axes exact Mac; actual initial adjacency insertion/publication ranks reproduced from decoded same-input bounds by generic reverse-max scan.4fixtures2slopesx2reps; no scenario or frame dispatch.','runs':runs,'fixtures':fixtures,'goal_complete':False},indent=2)+'\n')
for state,text in [('before',before),('after',s)]:
 d=w/state/p.name;d.parent.mkdir();d.write_text(text)
(w/'source-manifest.json').write_text(json.dumps({'scope':'Default-off generic Native initial reverse-maximum endpoint scan only; no updates/recycling/sort/bounds/coefficients/solver changes.','files':[{'path':'src/Rock3dEngine/header/physics/NativeContactOrderV79.h','location':'adapter','before_sha256':hashlib.sha256(before.encode()).hexdigest(),'after_sha256':hashlib.sha256(s.encode()).hexdigest()}],'goal_complete':False},indent=2)+'\n');p.write_text(s);print('Actual endpoint bounds and20Native initial pair ranks O0/O2 exact; installed candidate',runs,flush=True)
