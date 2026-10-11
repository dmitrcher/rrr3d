import argparse,json,pathlib,struct,subprocess,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--probe',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0];f32=lambda v:struct.unpack('<f',struct.pack('<f',float(v)))[0]
name='frame-v25-long-p8-x1-y1';native=compare.load(root/'reference-long-contact-run0-v25-windows.jsonl','windows-physx-d3d9');runs=[];report={'scope':'Recorded original internal geometry preparation; recorded inputs are diagnostic only','repeats':{}}
for repeat in range(2):
 stem=f'reference-native-geometry-pose-{name}';physical=compare.load(root/f'{stem}-physics-run{repeat}-v37-windows.jsonl','windows-physx-d3d9');assert physical=={name:native[name]}
 rows=[]
 for line in (root/f'{stem}-geometry-run{repeat}-v37-windows.jsonl').open():
  x=json.loads(line)
  if x.get('function')=='geometry-box-matrix-ready':
   pose=list(map(f,x['geometry_box_pose']['words']));expected=list(map(f,x['geometry_box_matrix']['words']));result=subprocess.run([str(a.probe.resolve())],input=' '.join(map(str,pose))+'\n',text=True,capture_output=True,check=True);actual=list(map(f32,result.stdout.split()));assert actual==expected,(actual,expected);rows.append({'pose':pose,'matrix':expected})
 assert len(rows)==120;runs.append(rows);report['repeats'][str(repeat)]={'matrices_exact':120,'entries_exact':1440,'physical_immutable_baseline_unchanged':True}
assert runs[0]==runs[1];report['inputs_outputs_repeatable']=True;(root/'physics-v42-geometry-audit.json').write_text(json.dumps(report,indent=2)+'\n');print('240 original internal geometry matrices / 2880 entries exact, two repeats; physics unchanged.')
