import argparse,collections,json,pathlib,struct
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
f32=lambda v:struct.unpack('<f',struct.pack('<f',float(v)))[0]
floats=lambda words:[struct.unpack('<f',struct.pack('<I',w))[0] for w in words]
result={'scope':'Actual Mac cache physical poses, all stored points/normals and original raw frames','runs':{}}
for repeat in range(2):
 summary={}
 for step in range(3):
  current=None;records={}
  for line in (root/f'step{step}/calibration-depth-ordinary-run{repeat}-v39-mac.jsonl').open():
   r=json.loads(line,parse_float=f32)
   if r.get('event')=='calibration.begin':current=r['detail']
   if r.get('event')=='physics.native-cache-input':records[current]=r['sample']
  cases={}
  for name,mac in records.items():
   groups=[];group=None
   for line in (root/f'reference-native-point-{name}-geometry-run{repeat}-v37-windows.jsonl').open():
    r=json.loads(line);fn=r.get('function')
    if fn=='geometry-dispatch-after':
     if group is not None:groups.append(group)
     group=collections.defaultdict(list)
    elif group is not None:group[fn].append(r)
    if len(groups)>step:break
   native=groups[step]['set-error-before-write'][0]
   mismatch=[];normals=[];anchors=[]
   for i in range(len(mac['errors'])):
    raw=floats(native[f'pre_raw_point_{i}']['words'])
    if mac['frame']!=raw[18:22]:mismatch.append({'point':i,'field':'frame','mac':mac['frame'],'windows':raw[18:22]})
    if mac['errors'][i][1:]!=raw[16:18]:mismatch.append({'point':i,'field':'transverse','mac':mac['errors'][i][1:],'windows':raw[16:18]})
    stored=[raw[3:6],raw[6:9],raw[9:12],raw[12:15]]
    for j in range(4):
     if mac['after'][i][j]!=stored[j]:(anchors if j<2 else normals).append({'point':i,'field':j,'mac':mac['after'][i][j],'windows':stored[j]})
   cases[name]={'points':len(mac['errors']),'frame_transverse_differences':mismatch,'local_anchor_differences':anchors,'local_normal_differences':normals}
  summary[str(step)]=cases
 result['runs'][str(repeat)]=summary
assert result['runs']['0']==result['runs']['1'],'Cache diagnostics are not repeatable'
for cases in result['runs']['0'].values():
 assert len(cases)==6
 assert all(not c['frame_transverse_differences'] and not c['local_anchor_differences'] and not c['local_normal_differences'] for c in cases.values()),'Actual cache persistence differs'
# The originally observed p7 normal defect is now closed as well.
case=result['runs']['0']['2']['frame-v25-long-p7-x1-y0'];assert case['points']==8 and not case['frame_transverse_differences'] and not case['local_anchor_differences']
(root/'physics-v45-cache-input-audit.json').write_text(json.dumps(result,indent=2)+'\n')
for step,cases in result['runs']['0'].items():
 for name,c in cases.items():print(step,name,'frame/transverse differences',len(c['frame_transverse_differences']),'local anchor differences',len(c['local_anchor_differences']),'local normal differences',len(c['local_normal_differences']))
