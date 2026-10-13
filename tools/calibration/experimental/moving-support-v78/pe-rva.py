from pathlib import Path
import struct,hashlib
path=Path('work/calibration-package/MotorRock-Calibration-Tools/Windows/PhysXCore.dll');data=path.read_bytes();assert hashlib.sha256(data).hexdigest()=='c03f0e20e99a6ca8d72c8627cf3b0108036a734123f75c652c2b78d3043dbdbb'
pe=struct.unpack_from('<I',data,0x3c)[0];count=struct.unpack_from('<H',data,pe+6)[0];opt=struct.unpack_from('<H',data,pe+20)[0];section=pe+24+opt
sections=[]
for i in range(count):
 at=section+i*40;name=data[at:at+8].rstrip(b'\0');vs,va,size,offset=struct.unpack_from('<4I',data,at+8);sections.append((va,max(vs,size),offset,name))
def read(rva,size):
 for va,limit,offset,name in sections:
  if va<=rva and rva+size<=va+limit:return data[offset+rva-va:offset+rva-va+size]
 raise ValueError(hex(rva))
if __name__=='__main__':
 import sys
 for arg in sys.argv[1:]:
  rva=int(arg,16);print(hex(rva),[hex(v-0x10000000)if 0x10000000<=v<0x10400000 else hex(v)for v in struct.unpack('<24I',read(rva,96))])
