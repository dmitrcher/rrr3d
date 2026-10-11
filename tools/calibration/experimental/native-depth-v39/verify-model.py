import argparse,json,pathlib,struct,subprocess,os
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--helper',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
f32=lambda v:struct.unpack('<f',struct.pack('<f',v))[0]
results=[]
for repeat in range(2):
 records=[]
 for line in (root/f'reference-native-vertex-frame-v25-long-p5-x0-y0-geometry-run{repeat}-v38-windows.jsonl').open():
  row=json.loads(line)
  if row.get('function')=='vertex1-depth-ready' and row['hit'] in [1,9]:records.append(row)
 assert len(records)==2
 for row in records:
  values=[f(w) for w in row['transform1to0']['words']]+[f(w) for w in row['plane0']['words'][:3]]+[f(w) for w in row['polygon_vertex']['words']]+[f(w) for w in row['polygon_target_coordinate']['words']]
  expected=[f(w) for w in row['polygon_flattened_transform']['words'][9:12]]+[f(row['xmm_words'][0][0])];expected.append(f32(expected[3]-values[-1]));outputs={}
  for mode in ['enabled','disabled']:
   env=dict(os.environ);env.pop('RRR3D_EXPERIMENT_NATIVE_POLYGON_DEPTH',None)
   if mode=='enabled':env['RRR3D_EXPERIMENT_NATIVE_POLYGON_DEPTH']='1'
   measured=subprocess.check_output([str(a.helper.resolve())],env=env,input=(' '.join(map(str,values))+'\n').encode()).decode().split();outputs[mode]=[f32(float(v)) for v in measured]
  assert outputs['enabled']==expected
  if row['hit']==9:assert outputs['disabled'][4]==-0.2873401641845703 and outputs['disabled']!=expected
  results.append({'repeat':repeat,'native_hit':row['hit'],'inputs':values,'windows':expected,'candidate':outputs['enabled'],'disabled':outputs['disabled']})
assert [{k:v for k,v in r.items() if k!='repeat'} for r in results[:2]]==[{k:v for k,v in r.items() if k!='repeat'} for r in results[2:]]
(root/'physics-v39-depth-model.json').write_text(json.dumps({'scope':'Recorded-input formula validation, separate from independent Mac simulation','rows':results},indent=2)+'\n');print('4 projection reconstructions exact; second-step disabled mismatch reproduced in 2 repeats.')
