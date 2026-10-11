import json,struct,subprocess,pathlib,hashlib,argparse
parser=argparse.ArgumentParser();parser.add_argument('--measurements',type=pathlib.Path,required=True);parser.add_argument('--probe',type=pathlib.Path,required=True);args=parser.parse_args();root=args.measurements.resolve()
def floats(words):return [struct.unpack('<f',struct.pack('<I',w))[0] for w in words]
def f32(v):return struct.unpack('<f',struct.pack('<f',v))[0]
names=['frame-v25-long-p7-x1-y0','frame-v25-long-p5-x0-y0','frame-v25-long-p5-x1-y1','frame-v25-long-p4-x1-y0']
audit={'scope':'First native contact preparation, diagnostic actual recorded inputs, not simulation parity','cases':{}}
for name in names:
 mac=next(json.loads(l)['sample'] for l in (root/f'calibration-tilted-{name}-run0-v36-mac.jsonl').open() if json.loads(l).get('event')=='physics.contact-manifold')
 rotation=[mac[f'source_box_axis{j}'][i] for i in range(3) for j in range(3)]
 for repeat in range(2):
  path=root/f'reference-native-tilted-{name}-geometry-run{repeat}-v36-windows.jsonl'
  records={}
  for line in path.open():
   r=json.loads(line)
   if 'function' in r:records.setdefault(r['function'],[]).append(r)
  size=records['geometry-dispatch-after'][0]['geometry_candidate_size']
  lines=[];expected=[]
  for i in range(size):
   ready=records['material-inputs-ready'][i];values=floats(ready['material_stack']['words'])
   actor=values[36:40];local=values[43:47];error=floats(ready['material_input_error']['words'])
   data=actor+local+mac['source_box_axis2']+error+rotation+mac['triangle_normal']
   assert len(data)==26;lines.append(' '.join(format(f32(v),'.9g') for v in data))
   frame=records['material-frame-adjusted'][i]
   matrix=floats(frame['material_stack']['words'][12:18])+floats(frame['material_stack']['words'][22:25])
   material=floats(records['material-quaternion-adjusted'][i]['material_stack']['words'][9:13])
   globalerror=floats(records['material-global-error'][i]['material_global_error']['words'])
   output=floats(records['material-error-ready'][i]['material_output_error']['words'])
   expected.append((matrix,material,globalerror,output,local))
  output=subprocess.check_output([str(args.probe.resolve())],input=('\n'.join(lines)+'\n').encode()).decode().splitlines()
  counts={k:0 for k in ('basis','material','global_error','output_error','derived_raw_local')};failures=[]
  for i,(line,expect) in enumerate(zip(output,expected)):
   values=[f32(float(v)) for v in line.split()]
   for label,actual,want in zip(counts,[values[4:13],values[13:17],values[28:31],values[31:34],values[34:38]],expect):
    if actual==want:counts[label]+=1
    else:failures.append({'point':i,'field':label,'actual':actual,'native':want})
  audit['cases'][name+f'-r{repeat}']={'points':size,'exact':counts,'failures':failures}
  print(name,repeat,counts,'failures',failures[:2],flush=True)
audit['model_header_sha256']=hashlib.sha256((pathlib.Path(__file__).resolve().parent/'src/Rock3dEngine/header/physics/NativeMaterialFrameV36.h').read_bytes()).hexdigest()
assert all(not c['failures'] for c in audit['cases'].values()),'Material frame model differs from native observations'
(root/'physics-v36-native-frame-model-audit-reverified.json').write_text(json.dumps(audit,indent=2)+'\n')
