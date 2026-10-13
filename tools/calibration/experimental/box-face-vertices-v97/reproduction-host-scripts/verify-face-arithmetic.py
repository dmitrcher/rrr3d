from pathlib import Path
import subprocess,json,struct,hashlib
root=Path.cwd();w=root/'work/box-face-v97';fixtures=json.load(open(root/'outputs/box-face-v97/native-face-vertex-fixtures.json'))['fixtures'];bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
data='\n'.join(' '.join(map(str,r['half']+[r['axis'],r['sign']])) for r in fixtures)+'\n';checks=[]
for opt in ['O0','O2']:
 exe=w/('face-probe-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off',str(w/'face-probe.cpp'),'-o',str(exe)],check=True);a=[list(map(float,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode()).decode().splitlines()];assert len(a)==len(fixtures)==18
 assert all(list(map(bits,v))==list(map(bits,sum(r['selected'],[]))) for v,r in zip(a,fixtures));checks.append({'optimization':opt,'fixtures':len(fixtures),'native_corner_words_exact':len(fixtures)*12});print(opt,'18 Native vertex fixtures216words exact')
p=root/'work/editor-package/MotorRock-Windows-MapEditor/PhysXCore.dll';d=p.read_bytes();pe=struct.unpack_from('<I',d,0x3c)[0];n=struct.unpack_from('<H',d,pe+6)[0];op=struct.unpack_from('<H',d,pe+20)[0];rva=0x312774;actual=None
for i in range(n):
 s=pe+24+op+i*40;size,va,rs,ptr=struct.unpack_from('<IIII',d,s+8)
 if va<=rva<va+max(size,rs):actual=d[ptr+rva-va:ptr+rva-va+24]
assert actual==bytes(json.load(open(root/'outputs/box-face-v97/native-face-vertex-fixtures.json'))['native_face_table'])
(root/'outputs/box-face-v97/face-arithmetic-proof.json').write_text(json.dumps({'scope':'Actual Native12polygon calls plus six literal table rows bound to complete actual eight-corner array; no live parity claim yet','runs':checks,'disk_table_matches_all_runtime_captures':True,'original_dll_sha256':hashlib.sha256(d).hexdigest(),'table_rva':rva,'goal_complete':False},indent=2)+'\n')
