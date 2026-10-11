import argparse,json,pathlib,struct,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
f=lambda ws:[struct.unpack('<f',struct.pack('<I',w))[0] for w in ws];round32=lambda vs:[struct.unpack('<f',struct.pack('<f',v))[0] for v in vs]
name='pose-v45-inverted-5';reference=compare.load(root/'inverted-native/expanded-pose-run0-windows.jsonl','windows-physx-d3d9')[name];report={'scope':'Read-only residual lead; not an accepted mass fix; source local inertia differs before global rotation','case':name,'repeats':{}}
for repeat in range(2):
 folder=root/'next-lead/native-inertia-focused50-v45';physical=compare.load(folder/f'reference-native-point-{name}-physics-run{repeat}-v37-windows.jsonl','windows-physx-d3d9');assert physical=={name:reference}
 native=[];matrix=None
 for l in (folder/f'reference-native-point-{name}-geometry-run{repeat}-v37-windows.jsonl').open():
  r=json.loads(l)
  if r.get('function')=='native-inertia-prepared' and r['simulation_step']==50:native.append(r)
  if r.get('function')=='geometry-box-matrix-ready' and r['simulation_step']==50:matrix=f(r['geometry_box_matrix']['words'])
 assert len(native)==1;body=f(native[0]['native_inertia_source']['words']);q=body[28:32];diagonal=body[37:40];tensor=f(native[0]['native_inertia_output']['words'][15:24])
 local=None;box=None;row=None
 for l in (root/f'next-lead/mac-inertia-focused50-v45/expanded-pose-ordinary-run{repeat}-mac.jsonl').open():
  r=json.loads(l)
  if r.get('event')=='physics.source-local-inertia':local=round32(r['sample']['local_inverse_inertia'])
  if r.get('event')=='physics.source-mesh-input' and box is None:box=round32(r['sample']['box'])
  if r.get('event')=='physics.native-contact-row' and row is None:row=r['sample']
 assert box==matrix and len(local)==9;mac_diagonal=[local[i] for i in [0,4,8]];differences=[{'axis':i,'windows':diagonal[i],'mac':mac_diagonal[i]} for i in range(3) if diagonal[i]!=mac_diagonal[i]];assert len(differences)==1 and differences[0]['axis']==2
 mac_tensor=round32(row['inverse_inertia']);assert sum(x!=y for x,y in zip(tensor,mac_tensor))==1 and tensor[8]!=mac_tensor[8]
 report['repeats'][str(repeat)]={'native_physical_states_unchanged_120_steps':True,'geometry_matrix_exact':True,'physical_quaternion':q,'native_local_diagonal':diagonal,'mac_local_diagonal':mac_diagonal,'local_coefficient_differences':differences,'native_solver_tensor':tensor,'mac_solver_tensor':mac_tensor}
assert report['repeats']['0']==report['repeats']['1'];(root/'physics-v45-next-lead-audit.json').write_text(json.dumps(report,indent=2)+'\n');print('New residual: geometry exact, source local inverse inertia Z differs one float step, before original solver rotation; both native traced runs retain all120 states.')
