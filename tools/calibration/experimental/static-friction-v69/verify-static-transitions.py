from pathlib import Path
import gzip, json, struct, hashlib

f=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
fs=lambda w:[struct.unpack('<f',struct.pack('<I',x))[0] for x in w]
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
add=lambda a,b:f(a+b)
sub=lambda a,b:f(a-b)
mul=lambda a,b:f(a*b)
div=lambda a,b:f(a/b)
def cross(a,b):
    return [sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))]
def dot(a,b):return add(add(mul(a[0],b[0]),mul(a[2],b[2])),mul(a[1],b[1]))
def axes(q):
    x,y,z,w=q
    xx=mul(mul(2,x),x);yy=mul(mul(2,y),y);zz=mul(mul(2,z),z)
    xy=mul(mul(2,x),y);xz=mul(mul(2,x),z);yz=mul(mul(2,y),z)
    xw=mul(mul(2,x),w);yw=mul(mul(2,y),w);zw=mul(mul(2,z),w)
    return [[sub(1,add(yy,zz)),add(xy,zw),sub(xz,yw)],
            [sub(xy,zw),sub(1,add(xx,zz)),add(yz,xw)],
            [add(xz,yw),sub(yz,xw),sub(1,add(xx,yy))]]

path=Path('outputs/triangle-native-followup-v60/native-border-grounded-62-0-private.jsonl.gz')
data=[json.loads(l) for l in gzip.open(path,'rt')]
history=[]; skipped=[]
for step, count in [(61,4),(62,6)]:
    entries=[r for r in data if r.get('simulation_step')==step and r.get('function')=='solver-iteration-entry']
    prepared=[r for r in data if r.get('simulation_step')==step and r.get('function')=='solver-initialize-exit']
    assert len(entries)==len(prepared)==count*5
    friction=[[0.]*3 for _ in range(count)]
    for j, entry in enumerate(entries):
        i=j%count
        row=fs(prepared[j]['native_solver_row']['words'])
        flip=fs(entry['native_body0']['words'])[11]==0.0
        body=fs(entry['native_body1' if flip else 'native_body0']['words'])
        v,w,I,M=body[:3],body[4:7],body[15:24],body[11]
        assert M>0
        r=row[8:11] if flip else row[4:7]
        N,T1,T2=axes(row[16:20])
        if flip:N=[-x for x in N]
        point=[add(a,k) for a,k in zip(v,cross(w,r))]
        old=row[27]
        delta=sub(mul(row[20],-dot(N,point)),row[24]);total=add(old,delta)
        if total<0:delta=-old;total=0.
        deltas=[]; legacy_deltas=[]; before_v=v.copy(); before_w=w.copy()
        for axis,K,bias,static_mu,dynamic_mu in [(T1,row[21],row[25],row[7],row[15]),(T2,row[22],row[26],row[11],row[23])]:
            previous=dot(axis,friction[i]);desired=sub(mul(K,-dot(axis,point)),bias);target=add(previous,desired)
            if total>0 and abs(target)>mul(static_mu,total):
                desired=sub(mul(div(mul(dynamic_mu,total),abs(target)),target),previous)
            legacy=sub(mul(K,-dot(axis,point)),bias)
            legacy_target=add(previous,legacy)
            if total>0 and abs(legacy_target)>mul(dynamic_mu,total):
                legacy=sub(mul(div(mul(dynamic_mu,total),abs(legacy_target)),legacy_target),previous)
            legacy_deltas.append(legacy)
            deltas.append(desired)
        df=[add(mul(a,deltas[0]),mul(t,deltas[1])) for a,t in zip(T1,T2)] if total>0 else [0.]*3
        friction[i]=[add(a,k) for a,k in zip(friction[i],df)]
        force=[add(a,mul(t,delta)) for a,t in zip(df,N)]
        legacy_df=[add(mul(a,legacy_deltas[0]),mul(t,legacy_deltas[1])) for a,t in zip(T1,T2)] if total>0 else [0.]*3
        legacy_force=[add(a,mul(t,delta)) for a,t in zip(legacy_df,N)]
        legacy_torque=cross(r,legacy_force)
        legacy_v=[add(a,mul(k,M)) for a,k in zip(before_v,legacy_force)]
        legacy_w=[add(before_w[k],add(add(mul(I[k],legacy_torque[0]),mul(I[3+k],legacy_torque[1])),mul(I[6+k],legacy_torque[2]))) for k in range(3)]
        torque=cross(r,force)
        v=[add(a,mul(k,M)) for a,k in zip(v,force)]
        w=[add(w[k],add(add(mul(I[k],torque[0]),mul(I[3+k],torque[1])),mul(I[6+k],torque[2]))) for k in range(3)]
        if i==count-1:
            skipped.append({'step':step,'iteration':j//count+1,'reason':'Next contact entry crosses wheel solver group; no contact-only comparison.'})
            continue
        nxt=entries[j+1]
        nxtflip=fs(nxt['native_body0']['words'])[11]==0.0
        expected=fs(nxt['native_body1' if nxtflip else 'native_body0']['words'])
        assert entry['native_body1' if flip else 'native_body0']['address']==nxt['native_body1' if nxtflip else 'native_body0']['address']
        calculated=list(map(bits,v+w)); wanted=list(map(bits,expected[:3]+expected[4:7]))
        history.append({'step':step,'iteration':j//count+1,'contact':i,'exact':calculated==wanted,
                        'computed_words':calculated,'native_words':wanted,'force':force,'legacy_force':legacy_force,'legacy_dynamic_only_exact':list(map(bits,legacy_v+legacy_w))==wanted})
report={'scope':'Every contact starts from its own saved Native body input. Friction state is retained per row. Only immediate within-group contact transitions are compared; wheel group boundaries excluded.',
        'source':str(path),'source_gzip_sha256':hashlib.sha256(path.read_bytes()).hexdigest(),
        'exact_transitions':sum(r['exact'] for r in history),'legacy_dynamic_only_exact_transitions':sum(r['legacy_dynamic_only_exact'] for r in history),'transitions':len(history),'history':history,'skipped':skipped,'goal_complete':False}
Path('outputs/physics-v69-identical-input-static-friction-proof.json').write_text(json.dumps(report,indent=2)+'\n')
print(report['exact_transitions'],'/',len(history),'separate static exact; dynamic-only',report['legacy_dynamic_only_exact_transitions'])
for r in history:
    if not r['exact']:print(r)
