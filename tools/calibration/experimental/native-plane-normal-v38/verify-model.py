import argparse,json,pathlib,struct,subprocess
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--plane-probe',type=pathlib.Path,required=True);p.add_argument('--normal-probe',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
fw=lambda w:list(struct.unpack('<'+str(len(w))+'f',struct.pack('<'+str(len(w))+'I',*w)));f=lambda v:struct.unpack('<f',struct.pack('<f',v))[0]
def helper(exe,rows):
 raw='\n'.join(' '.join(format(v,'.9g') for v in row) for row in rows)+'\n';got=subprocess.run([str(exe.resolve())],input=raw,text=True,capture_output=True,check=True);return [list(map(f,map(float,line.split()))) for line in got.stdout.splitlines()]
audit={'scope':'Recorded original Windows plane and kernel normal calculations; independent Mac triangle inputs and normals checked separately. Diagnostic helpers are not simulation replay.','plane_cases':{},'normal_cases':{},'goal_complete':False}
for repeat in range(2):
 geometry={};normal={};current=None
 for line in (root/f'calibration-plane-seven-ordinary-run{repeat}-v38-mac.jsonl').open():
  r=json.loads(line)
  if r.get('event')=='calibration.begin':current=r['detail']
  if r.get('event')=='physics.source-mesh-input':geometry.setdefault(current,[]).append(r['sample'])
  if r.get('event')=='physics.native-contact-row':normal.setdefault(current,[]).append(r['sample'])
 assert len(geometry)==7
 for name,mac in geometry.items():
  native=[r for r in map(json.loads,(root/f'reference-native-vertex-{name}-geometry-run{repeat}-v38-windows.jsonl').open()) if r.get('function')=='polygon-input-entry'][:2]
  assert len(mac)==len(native)==2
  actual=helper(a.plane_probe,[[f(v) for p in m['vertices'] for v in p] for m in mac])
  expected=[fw(n['plane0']['words']) for n in native]
  assert actual==expected and [list(map(f,m['plane'])) for m in mac]==expected,(name,'Triangle plane differs')
  audit['plane_cases'][name+f'-run{repeat}']={'triangles':2,'plane_formula_exact':True,'independent_mac_plane_exact':True,'inputs':[m['vertices'] for m in mac],'planes':expected}
 for name in ['frame-v25-long-p7-x1-y0','frame-v25-long-p5-x0-y0','frame-v25-long-p5-x1-y1','frame-v25-long-p4-x1-y0']:
  native=[r for r in map(json.loads,(root/f'reference-native-normal-{name}-geometry-run{repeat}-v38-windows.jsonl').open()) if r.get('function')=='kernel-normal-ready'];assert len(native)==64
  actual=helper(a.normal_probe,[fw(n['kernel_world_quaternion']['words']) for n in native]);expected=[fw(n['kernel_normal_words']) for n in native];assert actual==expected,(name,'Original kernel decode differs')
  mac=normal[name];assert len(mac) in (20,40)
  assert [list(map(f,m['normal'])) for m in mac]==expected[:len(mac)],(name,'Independent Mac normals differ')
  audit['normal_cases'][name+f'-run{repeat}']={'recorded_kernel_normals':len(native),'formula_exact':True,'independent_mac_first_rows_exact':len(mac)}
(root/'physics-v38-model-reverified.json').write_text(json.dumps(audit,indent=2)+'\n');print('28 triangle planes and 512 native kernel normals exact; independent Mac planes and first 240 normal rows exact across repeats.')
