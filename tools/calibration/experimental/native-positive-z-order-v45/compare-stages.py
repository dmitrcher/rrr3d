import argparse,json,pathlib,struct
p=argparse.ArgumentParser();p.add_argument('--mac',type=pathlib.Path,required=True);p.add_argument('--native',type=pathlib.Path,required=True);p.add_argument('--out',type=pathlib.Path,required=True);a=p.parse_args();f32=lambda x:struct.unpack('<f',struct.pack('<f',x))[0];decode=lambda w:[struct.unpack('<f',struct.pack('<I',x))[0] for x in w];reports=[]
for repeat in range(2):
 native=[];beforeDivide=None;afterPlane=None
 for line in (a.native/f'reference-native-point-frame-v25-long-p11-x1-y0-geometry-run{repeat}-v37-windows.jsonl').open():
  r=json.loads(line);fn=r.get('function')
  if fn=='geometry-dispatch-after':break
  if fn=='edge-crossing-before-divide':beforeDivide=r
  if fn=='edge-crossing-after-plane':afterPlane=r
  if fn=='edge-crossing-accepted':native.append((r,beforeDivide,afterPlane))
 mac=[json.loads(l)['sample'] for l in (a.mac/f'calibration-depth-ordinary-run{repeat}-v39-mac.jsonl').open() if json.loads(l).get('event')=='physics.native-edge-stages' and json.loads(l)['sample']['accepted']];assert len(mac)==len(native)==4;edges=[]
 for index,((n,before,after),m) in enumerate(zip(native,mac)):
  expected={k:decode(n[v]['words']) for k,v in [('delta','edge_delta'),('p1','edge_reference_p1'),('p2','edge_reference_p2'),('direction','edge_reference_dir'),('p3','edge_colliding_p3'),('plane_normal','edge_plane_normal'),('normalized_direction','edge_normalized_direction'),('after','edge_clipped_point')]}
  expected.update({k:decode(n[v]['words'])[0] for k,v in [('plane_constant','edge_plane_constant'),('coefficient','edge_inverse_coefficient'),('denominator','edge_crossing_denominator')]})
  expected.update({'distance':decode(n['xmm_words'][0])[0],'d3':decode(before['xmm_words'][0])[0],'ratio':decode(after['xmm_words'][0])[0],'before':[decode(after['xmm_words'][j])[0] for j in [3,2,1]]})
  dif=[]
  for k,e in expected.items():
   actual=list(map(f32,m[k])) if isinstance(m[k],list) else f32(m[k]);
   if actual!=e:dif.append({'field':k,'mac':actual,'windows':e})
  edges.append({'index':index,'fields_checked':len(expected),'differences':dif})
 reports.append(edges)
assert reports[0]==reports[1];report={'scope':'Actual independent upper local-Z face traversal and accepted edge inputs/intermediates against original private Windows calls; first simulation pass','accepted_edges':4,'repeats':2,'edges':reports[0]};a.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2));assert all(not e['differences'] for e in reports[0])
