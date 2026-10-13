"""Derive observed node allocations/publications directly from read-only Native captures."""
from pathlib import Path
import gzip,json,struct,subprocess,tempfile
base=Path(__file__).resolve().parent
payloads=[]
for rep in range(2):
 records=[json.loads(s) for s in gzip.open(base/f'captures/native-endpoint-window/force-devildriver-engine3-lateral-45-{rep}-private.jsonl.gz','rt')]
 ticks=sorted({r['sample_tick_v71'] for r in records if r.get('function')=='broadphase-removed-list'})
 assert ticks==list(range(46))
 lines=[str(len(ticks))]
 for tick in ticks:
  rows=[r for r in records if r.get('sample_tick_v71')==tick]
  inserts=[r for r in rows if r.get('function')=='adjacency-insert'];lines.append(str(len(inserts)))
  for r in inserts:
   a,b=r['stack']['words'][1:3];words=r['pair_storage']['words'];head=words[16]&65535
   lines.append(f'{a} {b} {0 if head==65535 else head} {words[15]}')
  pubs=[r for r in rows if r.get('function')=='broadphase-created-api'];pairs=[]
  for r in pubs:
   bitmap=r['created_bitmap']['words'][:r['broadphase_api']['words'][25]]
   words=r['adjacency_nodes']['words'];shorts=struct.unpack('<'+'H'*(len(words)*2),struct.pack('<'+'I'*len(words),*words))
   for wordindex,word in enumerate(bitmap):
    for bit in range(32):
     if word&(1<<bit):
      idx=wordindex*32+bit;node,slot=divmod(idx,4);pairs.append((shorts[node*6+5],shorts[node*6+slot]))
  lines.append(str(len(pairs)));lines.extend(f'{a} {b}' for a,b in pairs)
  removals=[r for r in rows if r.get('function')=='broadphase-removed-list'];assert len(removals)==1
  r=removals[0];words=r['removed_pairs']['words'];assert len(words)==4*r['removed_pair_count']
  lines.append(str(r['removed_pair_count']))
  lines.extend(f'{(words[i]&0x3ffff)+1} {(words[i+1]&0x3ffff)+1}' for i in range(0,len(words),4))
 payload='\n'.join(lines)+'\n';assert payload==(base/f'proofs/captured-nodes-{rep}.txt').read_text();payloads.append(payload)
assert payloads[0]==payloads[1]
with tempfile.TemporaryDirectory(prefix='motorrock-v90-nodes-') as t:
 for opt in ['O0','O2']:
  exe=Path(t)/opt
  subprocess.run(['clang++','-std=c++17','-'+opt,str(base/'proofs/verify-nodes.cpp'),'-o',str(exe)],check=True)
  result=subprocess.run([str(exe)],input=payloads[0],text=True,capture_output=True,check=True)
  assert result.stdout.strip()=='54 Native allocator/publication checkpoints exact'
  control=subprocess.run([str(exe),'no-recycling'],input=payloads[0],text=True,capture_output=True)
  assert control.returncode and 'Native pre-insert allocator state mismatch' in control.stderr
  print(opt+': '+result.stdout.strip()+'; no-recycling control rejected')
print('Two captured 46-epoch operation streams exact. Mixed cancellation and scene geometry outside this proof.')
