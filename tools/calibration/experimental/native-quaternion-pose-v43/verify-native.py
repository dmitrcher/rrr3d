"""Reconstruct original COM/shape translations and normal-response mass."""
import argparse,json,pathlib,struct,subprocess,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--probe',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
f32=lambda v:struct.unpack('<f',struct.pack('<f',float(v)))[0]
reference=compare.load(root/'expanded-native/expanded-pose-run0-windows.jsonl','windows-physx-d3d9')
report={'scope':'Observed original geometry/cache-miss poses and normal-response mass; recorded inputs are diagnostic only','datasets':{}}
for i in [1,3,4,5]:
 name=f'pose-v42-frozen-{i}';params=reference[name]['meta']['fixture_parameters'];offset=[f32(f32(x)-f32(y)) for x,y in zip(params['shape_position'],params['center_of_mass'])];repeats=[]
 for repeat in range(2):
  stem=f'reference-native-point-{name}';physical=root/f'{stem}-physics-run{repeat}-v37-windows.jsonl';assert compare.load(physical,'windows-physx-d3d9')=={name:reference[name]},'Tracing changes original physical states'
  initial=None
  for line in physical.open():
   x=json.loads(line)
   if x.get('event')=='calibration.initial-rotation':initial=x['sample'];break
  assert initial is not None
  pose=None;first_center=None;inputs=[];expected=[];kinds=[];samples=[]
  path=root/f'{stem}-geometry-run{repeat}-v37-windows.jsonl'
  for line in path.open():
   x=json.loads(line)
   if x.get('function')=='geometry-box-matrix-ready':pose=list(map(f,x['geometry_box_pose']['words']))
   if x.get('function')=='point-transport-entry':
    assert pose is not None;center=list(map(f,x['transport_pose0']['words'][9:12]));first_center=first_center or center;values=pose[3:]+center+offset;inputs.append('0 '+' '.join(map(str,values)));expected.append(pose[:3]);kinds.append('translation');samples.append(values)
   if x.get('function')=='solver-initialize-exit':
    row=list(map(f,x['native_solver_row']['words']));body=list(map(f,x['native_body0']['words']));values=row[4:7]+row[16:20]+body[15:24]+[body[11]];inputs.append('2 '+' '.join(map(str,values)));expected.append([row[20]]);kinds.append('mass');samples.append(values)
  assert first_center is not None
  values=list(map(f32,initial['quaternion']+initial['position']+params['center_of_mass']));inputs.append('0 '+' '.join(map(str,values)));expected.append(first_center);kinds.append('initial_com');samples.append(values)
  result=subprocess.run([str(a.probe.resolve())],input='\n'.join(inputs)+'\n',text=True,capture_output=True,check=True);actual=[list(map(f32,line.split())) for line in result.stdout.splitlines()];assert actual==expected,(name,repeat,[(j,kinds[j],x,y) for j,(x,y) in enumerate(zip(actual,expected)) if x!=y][:3]);repeats.append((samples,expected))
  counts={k:kinds.count(k) for k in set(kinds)};assert counts['mass']==256
  report['datasets'][f'{name}-run{repeat}']={'physical_120_steps_unchanged':True,'exact_records':counts}
 assert repeats[0]==repeats[1],'Native diagnostic inputs or outputs differ between repeats'
report['totals']={k:sum(d['exact_records'][k] for d in report['datasets'].values()) for k in ['translation','mass','initial_com']};assert report['totals']=={'translation':896,'mass':2048,'initial_com':8};(root/'physics-v43-native-formula-audit.json').write_text(json.dumps(report,indent=2)+'\n');print('Original native exact:',report['totals'])
