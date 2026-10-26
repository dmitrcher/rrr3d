"""Read original PE/PDB identity and public JumpProgress symbol; no target execution."""
from pathlib import Path
import struct,hashlib,json,subprocess
root=Path.cwd();w=root/'work/controller-jump-v187';dll=root/'work/package/MotorRock-Windows-Debug/Rock3dGame.dll';pdb=dll.with_suffix('.pdb');b=dll.read_bytes();p=pdb.read_bytes();sha=lambda d:hashlib.sha256(d).hexdigest()
assert sha(b)=='4483e8f402edb5c4639609dcf3fd805386e80f1d25b1c4829a8236b40a6b8a15'
assert sha(p)=='818ba6d6386ef6c4c925d16b9edf2403f05f78385e36565df493c7173432770c'
u=lambda d,o:struct.unpack_from('<I',d,o)[0]
page=u(p,32);ds=u(p,44);blockmap=u(p,52);blocks=struct.unpack_from('<'+'I'*((ds+page-1)//page),p,blockmap*page);directory=b''.join(p[n*page:(n+1)*page] for n in blocks)[:ds];count=u(directory,0);sizes=struct.unpack_from('<'+'I'*count,directory,4);cursor=4+4*count;streams=[]
for size in sizes:
 n=0 if size==0xffffffff else (size+page-1)//page;pages=struct.unpack_from('<'+'I'*n,directory,cursor) if n else [];cursor+=4*n;streams.append(b''.join(p[x*page:(x+1)*page] for x in pages)[:size] if size!=0xffffffff else b'')
nt=u(b,0x3c);sections=struct.unpack_from('<H',b,nt+6)[0];opt=struct.unpack_from('<H',b,nt+20)[0];base=u(b,nt+52);ss=[]
for i in range(sections):
 o=nt+24+opt+40*i;ss.append(dict(name=b[o:o+8].rstrip(b'\0').decode(),virtual_size=u(b,o+8),rva=u(b,o+12),raw_size=u(b,o+16),offset=u(b,o+20)))
def fileoff(rva):
 s=next(s for s in ss if s['rva']<=rva<s['rva']+s['raw_size']);return s['offset']+rva-s['rva']
debugrva=u(b,nt+24+96+6*8);debugsize=u(b,nt+24+96+6*8+4);codeview=[]
for o in range(fileoff(debugrva),fileoff(debugrva)+debugsize,28):
 if u(b,o+12)==2:
  q=u(b,o+24);codeview.append(b[q:q+u(b,o+16)])
cv=next(x for x in codeview if x.startswith(b'RSDS'));assert cv[4:20]==streams[1][12:28] and u(cv,20)==u(streams[1],8)
name=b'?JumpProgress@GameCar@game@r3d@@AAEXM@Z';symbol=[]
for streamno,data in enumerate(streams):
 start=0
 while True:
  n=data.find(name,start)
  if n<0:break
  if n>=14:
   length,kind,flags,offset,segment=struct.unpack_from('<HHIIH',data,n-14)
   if kind==0x110e:symbol.append(dict(stream=streamno,kind=kind,flags=flags,offset=offset,segment=segment))
  start=n+1
assert len(symbol)==1,symbol;s=symbol[0];rva=ss[s['segment']-1]['rva']+s['offset'];address=base+rva
with (w/'original-jump.disasm.txt').open('w') as f:subprocess.run(['objdump','-d',f'--start-address=0x{address:x}',f'--stop-address=0x{address+0x188:x}',str(dll)],stdout=f,check=True)
r=dict(evidence_type='ordinary local PE/PDB parsing and LLVM disassembly; not REA Evidence or runtime observation',dll_sha256=sha(b),pdb_sha256=sha(p),codeview_guid=cv[4:20].hex(),pdb_age=u(cv,20),identity_matches=True,symbol=name.decode(),public_symbol=s,address=hex(address),rva=hex(rva),file_offset=fileoff(rva),method_bytes_sha256=sha(b[fileoff(rva):fileoff(rva)+0x188]),source_semantics=['spring timer decremented before contact gate','fixed Scene::cDefGravity with NX_ACCELERATION mode5 and wakeup=true','pitch needs speed>1, nonzero flyYTorque and post-decrement spring time==0'],limitations=['source semantics corroborated by bounded decoded instructions, not a live Windows call','global cDefGravity initialization/value is established from original Physx.cpp source, not a captured runtime memory read','Ghidra default analysis timed out after 300s; no function Evidence was returned; session closed'],vm_started=False)
(w/'windows-static-verification.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r,indent=2))
