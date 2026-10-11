from pathlib import Path
import argparse,json,struct,math,hashlib,subprocess,os
import compare_geometry
p=argparse.ArgumentParser();p.add_argument('--measurements',type=Path,required=True);p.add_argument('--geometry-probe',type=Path,required=True);a=p.parse_args();out=a.measurements;source=Path(__file__).resolve().parent
fixture_path=source/'expected-sdk-points.json';input_path=source/'native-input.txt';fixture=json.loads(fixture_path.read_text());data=input_path.read_bytes();reports={};lines=[]
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_EXPERIMENT','RRR3D_CONTACT'))};env['RRR3D_EXPERIMENT_UPPER_FACE_WINDING']=env['RRR3D_EXPERIMENT_NEGATIVE_X_FACE_ORDER']='1'
for mode in [0,1]:
 captures=[out/f'calibration-native-edge-geometry-mode{mode}-run{r}-v34-mac.txt' for r in range(2)];assert captures[0].read_bytes()==captures[1].read_bytes();env['RRR3D_EXPERIMENT_NATIVE_MESH_EDGE']=str(mode)
 for repeat in range(2):assert subprocess.check_output([str(a.geometry_probe.resolve())],input=data,env=env)==captures[repeat].read_bytes()
 report=compare_geometry.compare(captures[0],fixture);assert report['cases']==64 and report['points']==306 and report['nonexact_points']==(183 if mode==0 else 70)
 reports[str(mode)]={'semantic_numeric_repeats_exact':True,'report':report};lines.append(captures[0].read_text().splitlines())
f=lambda x:struct.unpack('f',struct.pack('f',float(x)))[0]
stats={'new_exact_points':0,'exact_points_regressed':0,'nonexact_distance_increased':0,'nonexact_distance_decreased':0,'same_distance_points':0};details=[]
for case,aa,bb in zip(fixture['cases'],*lines):
 aa=aa.split();bb=bb.split();assert aa[0]==bb[0]
 for i,point in enumerate(case['points']):
  j=1+8*i;expected=list(map(f,point['point']));ds=[math.dist(list(map(f,row[j:j+3])),expected) for row in [aa,bb]]
  if ds[0]>0 and ds[1]==0:stats['new_exact_points']+=1
  if ds[0]==0 and ds[1]>0:stats['exact_points_regressed']+=1
  if ds[1]>ds[0]:stats['nonexact_distance_increased']+=1;details.append({'case':case['name'],'point':i,'before_distance':ds[0],'after_distance':ds[1]})
  elif ds[1]<ds[0]:stats['nonexact_distance_decreased']+=1
  else:stats['same_distance_points']+=1
assert stats=={'new_exact_points':115,'exact_points_regressed':2,'nonexact_distance_increased':7,'nonexact_distance_decreased':123,'same_distance_points':176}
result={'scope':'Broader standalone face-contact arithmetic only, recorded Native initial pose/mesh inputs and recorded SDK points, not full physical replay. v26 separation is reconstructed diagnostic.','reference_fixture_sha256':hashlib.sha256(fixture_path.read_bytes()).hexdigest(),'input_sha256':hashlib.sha256(data).hexdigest(),'helper_sha256':hashlib.sha256(a.geometry_probe.read_bytes()).hexdigest(),'goal_complete':False,'reports':reports,'point_change_statistics':stats,'distance_increase_details':details}
(out/'physics-v34-geometry64-audit.json').write_text(json.dumps(result,indent=2)+'\n');print('64 recorded-input geometries repeated:183 ->70 nonexact points. Two exact points regress;7 distances increase. General geometry remains unaccepted.')
