from pathlib import Path
import gzip,json,struct,math
f=lambda u:struct.unpack('<f',struct.pack('<I',u))[0];s=lambda x:struct.unpack('<f',struct.pack('<f',x))[0];w=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];d=lambda pair:struct.unpack('<d',struct.pack('<II',*pair))[0]
p=Path('outputs/controller-stabilize-v185');ns={n['case']:n for n in map(json.loads,gzip.open(p/'accepted-native0.jsonl.gz','rt')) if n['event']=='stabilize.pose'};reps=[]
for case in [264,504,744,984,1224]:
 n=ns[case];q=list(map(f,n['rotation_before']));x,y,z,ww=q;mag=sum(s(v*v) for v in q);scale=2/mag;xs=x*scale;ys=y*scale;zs=z*scale
 mat=[s(1-(y*ys+z*zs)),s(x*ys-ww*zs),s(x*zs+ww*ys),0,s(x*ys+ww*zs),s(1-(x*xs+z*zs)),s(y*zs-ww*xs),0,s(x*zs-ww*ys),s(y*zs+ww*xs),s(1-(x*xs+y*ys)),0,0,0,0,1]
 angles=list(map(f,n['checkpoint4'][:3]));angles[0]=max(-f(n['roll_word']),min(f(n['roll_word']),angles[0]));angles[1]=max(-f(n['pitch_word']),min(f(n['pitch_word']),angles[1]));trigs=[math.cos(v*.5) for v in angles]+[math.sin(v*.5) for v in angles];native=[d(n['checkpoint7'][i:i+2]) for i in range(0,12,2)]
 print(case,'matrix exact',list(map(w,mat))==n['checkpoint6']);print('trigs',native);print('mac',trigs);print('float native',[v==s(v) for v in native]);print('diff',[(a-b) for a,b in zip(native,trigs)])
 reps.append(dict(case=case,matrix_exact=list(map(w,mat))==n['checkpoint6'],native_trig=native,mac_trig=trigs,native_trig_f32=[v==s(v) for v in native]))
(p/'euler-arithmetic-analysis.json').write_text(json.dumps(reps,indent=2)+'\n')
