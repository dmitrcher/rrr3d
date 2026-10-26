import struct,json,sys
from pathlib import Path
class MSI:
 def __init__(self,path):
  self.b=Path(path).read_bytes();b=self.b;self.ss=1<<struct.unpack_from('<H',b,30)[0];self.ms=1<<struct.unpack_from('<H',b,32)[0]
  self.fat=[];fatsectors=list(struct.unpack_from('<109I',b,76));nxt=struct.unpack_from('<I',b,68)[0]
  while nxt<0xfffffffa:
   z=struct.unpack('<%dI'%(self.ss//4),self.sector(nxt));fatsectors+=z[:-1];nxt=z[-1]
  for i in fatsectors:
   if i<0xfffffffa:self.fat+=struct.unpack('<%dI'%(self.ss//4),self.sector(i))
  directory=self.chain(struct.unpack_from('<I',b,48)[0]);self.entries={};root=None
  for i in range(0,len(directory),128):
   d=directory[i:i+128];l=struct.unpack_from('<H',d,64)[0];name=d[:max(0,l-2)].decode('utf-16le');kind=d[66];start=struct.unpack_from('<I',d,116)[0];size=struct.unpack_from('<Q',d,120)[0]
   if kind==5:root=(start,size)
   if kind==2:self.entries[self.decode(name)]=(start,size)
  self.mini=self.chain(root[0])[:root[1]];self.minifat=[]
  mf=self.chain(struct.unpack_from('<I',b,60)[0]);self.minifat=struct.unpack('<%dI'%(len(mf)//4),mf) if mf else []
  pool=self.stream('_StringPool');data=self.stream('_StringData');cp,flag=struct.unpack_from('<HH',pool);self.refwidth=3 if flag&0x8000 else 2;self.strings=[''];off=0
  for i in range(4,len(pool),4):
   size,refs=struct.unpack_from('<HH',pool,i);self.strings.append(data[off:off+size].decode('cp'+str(cp or 1252),errors='replace'));off+=size
 def sector(self,n):return self.b[(n+1)*self.ss:(n+2)*self.ss]
 def chain(self,n):
  out=b'';seen=set()
  while n<0xfffffffa:
   if n in seen:raise ValueError('chain cycle')
   seen.add(n);out+=self.sector(n);n=self.fat[n]
  return out
 @staticmethod
 def decode(s):
  chars='0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz._';out=''
  for c in s:
   n=ord(c)
   if n==0x4840:continue
   if 0x3800<=n<0x4800:n-=0x3800;out+=chars[n&63]+chars[n>>6]
   elif 0x4800<=n<0x4840:out+=chars[n-0x4800]
   else:out+=c
  return out
 def stream(self,name):
  n,size=self.entries[name]
  if size>=4096:return self.chain(n)[:size]
  out=b''
  while n<0xfffffffa:out+=self.mini[n*self.ms:(n+1)*self.ms];n=self.minifat[n]
  return out[:size]
 def table(self,name,types):
  b=self.stream(name);widths=[self.refwidth if t=='s' else t for t in types];N=len(b)//sum(widths);cols=[];off=0
  for t,w in zip(types,widths):
   col=[]
   for i in range(N):
    v=int.from_bytes(b[off+i*w:off+(i+1)*w],'little');col.append(self.strings[v] if t=='s' else v-(1<<(w*8-1)) if v else None)
   off+=N*w;cols.append(col)
  return list(map(list,zip(*cols)))
if __name__=='__main__':
 m=MSI(sys.argv[1]);cols=m.table('_Columns',['s',2,'s',2]);tables={}
 for n in ['Directory','Component','File']:
  schema=sorted([c for c in cols if c[0]==n],key=lambda c:c[1]);types=['s' if c[3]&0x800 else c[3]&0xff for c in schema];tables[n]=m.table(n,types)
 print(json.dumps(tables,indent=2))
