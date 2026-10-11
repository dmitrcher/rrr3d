from pathlib import Path
import argparse,json,struct,subprocess,hashlib,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=Path,required=True);p.add_argument('--edge-probe',type=Path,required=True);a=p.parse_args();out=a.measurements
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
f=lambda x:struct.unpack('f',struct.pack('f',x))[0]
def floats(row,key):
 w=row[key]['words'];return list(struct.unpack(f'{len(w)}f',struct.pack(f'{len(w)}I',*w)))
baseline=compare.load(out/'reference-row-iterations4-run0-v30-windows.jsonl','windows-physx-d3d9');reports=[]
for m in range(3):
 for h in range(2):
  for v in range(2):
   name=f'isolate-03-contact-m{m}-h{h}-v{v}-f1';repeats=[]
   for repeat in range(2):
    path=out/f'reference-native-edge-{name}-geometry-run{repeat}-v34-windows.jsonl';rows=list(map(json.loads,path.open()));accepted=[r for r in rows if r.get('function')=='edge-crossing-accepted'][:4];assert len(accepted)==4
    inputs=[]
    for r in accepted:
     p3=floats(r,'edge_colliding_p3');frame=floats(r,'edge_frame');delta=frame[67:70];p4=[f(x+y) for x,y in zip(p3,delta)]
     inputs.append(sum([floats(r,k) for k in ['edge_delta','edge_reference_p1','edge_reference_p2','edge_reference_dir','edge_colliding_p3']],[])+p4)
    raw='\n'.join(' '.join(format(x,'.9g') for x in row) for row in inputs)+'\n';models=[json.loads(x) for x in subprocess.check_output([str(a.edge_probe.resolve())],input=raw,text=True).splitlines()];assert len(models)==4
    edges=[]
    for index,(r,model) in enumerate(zip(accepted,models)):
     for key,nativekey in [('plane_normal','edge_plane_normal'),('normalized_direction','edge_normalized_direction'),('clipped_point','edge_clipped_point')]:assert list(map(f,model[key]))==floats(r,nativekey),(name,index,key,model,floats(r,nativekey))
     assert f(model['plane_constant'])==floats(r,'edge_plane_constant')[0] and f(model['coefficient'])==floats(r,'edge_inverse_coefficient')[0] and model['accepted']
     edges.append({'index':index,'inputs':inputs[index],'plane_normal':list(map(f,model['plane_normal'])),'normalized_direction':list(map(f,model['normalized_direction'])),'plane_constant':f(model['plane_constant']),'coefficient':f(model['coefficient']),'clipped_point_before_world_transform':list(map(f,model['clipped_point'])),'exact':True})
    phys=out/f'reference-native-edge-{name}-physics-run{repeat}-v34-windows.jsonl';assert compare.load(phys,'windows-physx-d3d9')[name]==baseline[name];repeats.append(edges)
   assert repeats[0]==repeats[1];reports.append({'case':name,'defined_numeric_repeats_exact':True,'all120_native_states_unchanged':True,'edges':repeats[0]})
report={'scope':'Actual compiled ARM64 edge helper reconstructs normalized plane, direction, coefficient and clipped local point from observed input vertices/direction, not expected outputs. Four first accepted internal edges per case; raw scratch/padding and entire trace pointer identity excluded. This standalone check supplies recorded geometric inputs. Actual Mac independent contact generation is separately verified.','cases':12,'repeats':2,'internal_edges_exact':48,'helper_sha256':hashlib.sha256(a.edge_probe.read_bytes()).hexdigest(),'goal_complete':False,'reports':reports}
(out/'physics-v34-edge-model-audit.json').write_text(json.dumps(report,indent=2)+'\n');print('48/48 internal edge calculations exact, both repeats; all24 Native runs retain full120 baseline states.')
