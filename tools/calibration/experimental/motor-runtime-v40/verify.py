import argparse,json,pathlib,struct
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);a=p.parse_args();d=a.measurements
f32=lambda v:struct.unpack('<f',struct.pack('<f',float(v)))[0]
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
value=lambda v:struct.unpack('<f',struct.pack('<I',v))[0]
load=lambda p:[json.loads(line) for line in p.read_text().splitlines()]
runs=[load(d/f'motor-runtime-windows-run{i}-v40.jsonl') for i in range(2)];assert runs[0]==runs[1] and len(runs[0])==144
mac=load(d/'motor-formula-mac-run0-v39.jsonl');windows=load(d/'motor-formula-windows-run0-v39.jsonl');assert len(mac)==len(windows)==72
out={'scope':'Original Windows Debug DLL CalcTorque in a standalone caller under explicitly selected x87 precision; actual running game precision and wheel boundary remain pending','original_dll_sha256':'4483e8f402edb5c4639609dcf3fd805386e80f1d25b1c4829a8236b40a6b8a15','repeatable':True,'cases':{}}
for precision in [53,24]:
 rows=[r for r in runs[0] if r['precision']==precision];assert len(rows)==72
 for r,m,w in zip(rows,mac,windows):
  assert all(r[k]==m[k]==w[k] for k in ['torque_index','sem_index','gear'])
  torque=[300,600,2000,5000][r['torque_index']];ratio=f32([1.5,2.66,1.78,1.30,1,.74][r['gear']]);diff=f32(3.42);sem=f32([.7,.9,1][r['sem_index']]);game=f32(1.15)
  extended=float(torque)*ratio*diff*sem*game
  sequential=f32(f32(f32(f32(torque*ratio)*diff)*sem)*game)
  assert r['source_bits']==bits(extended if precision==53 else sequential)
  assert r['source_bits']==(w if precision==53 else m)['source_bits']
 mismatches=[{'torque_index':r['torque_index'],'sem_index':r['sem_index'],'gear':r['gear'],'windows_bits':r['source_bits'],'mac_collapsed_bits':m['collapsed_bits'],'absolute_difference':abs(value(r['source_bits'])-value(m['collapsed_bits']))} for r,m in zip(rows,mac) if r['source_bits']!=m['collapsed_bits']]
 out['cases'][str(precision)]={'inputs':72,'mac_collapsed_differences':len(mismatches),'maximum_absolute_difference':max(x['absolute_difference'] for x in mismatches),'differences':mismatches}
assert out['cases']['53']['mac_collapsed_differences']==27 and out['cases']['24']['mac_collapsed_differences']==15
out['sem07_folded_bits']=bits(f32(f32(.7)*f32(1.15)));out['default_efficiency_0805_bits']=bits(f32(.805))
(d/'physics-v40-motor-runtime-audit.json').write_text(json.dumps(out,indent=2)+'\n');print({k:v for k,v in out.items() if k!='cases'});print({k:{n:v for n,v in r.items() if n!='differences'} for k,r in out['cases'].items()})
