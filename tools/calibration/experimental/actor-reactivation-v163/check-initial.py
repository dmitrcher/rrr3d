from pathlib import Path
import json,struct,subprocess
rows=[r for r in map(json.loads,open('outputs/cold-manager-v162/source-initial16/initial-order.jsonl')) if r.get('event')=='native-order-init']
def raw(k):return k^2147483648 if k&2147483648 else ~k&0xffffffff
bounds={}
for r in rows:
 axis=r['axis'];w=r['words']
 for i in range(0,len(w),2):
  owner=w[i+1]&0x7fffffff;b=0 if w[i+1]&2147483648 else 1;bounds.setdefault(owner,[[0]*3,[0]*3])[b][axis]=raw(w[i])
for plane in [False,True]:
 inp=[]
 for i,b in sorted(bounds.items()):
  if i==105 and plane:inp.append('0 0 '+' '.join(str(struct.unpack('<I',struct.pack('<f',v))[0]) for v in [-1e6,-1e6,-2000000.125,1e6,1e6,.125]))
  actor=0 if i<=96 else i-95 if i<105 else 1
  inp.append(f'{actor} {int(i>105)} '+' '.join(str(x) for row in b for x in row))
 proc=subprocess.run(['work/actor-reactivation-v163/check-initial'],input=str(len(inp))+'\n'+'\n'.join(inp),text=True,capture_output=True,check=True,env={'RRR3D_EXPERIMENT_NATIVE_INITIAL_MAX_SCAN_V102':'1'})
 pairs=[list(map(int,s.split())) for s in proc.stdout.splitlines()];print('plane',plane,'owned', [p for p in pairs if any(x>=105+plane for x in p)])
 if not plane:
  ref=json.loads(open('outputs/cold-manager-v162/source-track16/managers.jsonl').readline())['created'];print('whole source original match',pairs==ref,len(pairs),len(ref))
