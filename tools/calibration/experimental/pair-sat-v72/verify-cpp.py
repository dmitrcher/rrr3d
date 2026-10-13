"""Verify optimized standalone C++ SAT and ordered geometry against original Windows captures."""
from pathlib import Path
import argparse,json,gzip,subprocess,tempfile,hashlib
parser=argparse.ArgumentParser();parser.add_argument('--header',type=Path);parser.add_argument('--out',type=Path,default=Path('native-box-cpp-proof-v72.json'));args=parser.parse_args();base=Path(__file__).resolve().parent
header=args.header or base/'after/adapter/src/Rock3dEngine/header/physics/SourceBoxManifold.h';header=header.resolve();assert header.is_file()
fixtures=json.loads((base/'sat-fixtures.json').read_text());results=[]
with tempfile.TemporaryDirectory(prefix='motorrock-native-sat-v72-') as tmp:
 for optimization in ['0','2']:
  result={'optimization':'O'+optimization,'fixtures':len(fixtures),'sat_exact':False,'raw_geometry_exact':0}
  for name in ['test-native-sat','test-native-geometry']:
   exe=Path(tmp)/(name+optimization)
   subprocess.run(['xcrun','clang++','-std=c++17','-O'+optimization,'-ffp-contract=off','-I',str(header.parent),str(base/(name+'.cpp')),'-o',str(exe)],check=True)
   lines=subprocess.run([str(exe)],capture_output=True,text=True,check=True).stdout.splitlines()
   if name=='test-native-sat':result['sat_exact']=True;result['sat_stdout']=lines
   else:
    assert len(lines)==len(fixtures)
    for fixture,line in zip(fixtures,lines):
     rows=list(map(json.loads,gzip.open(base/'fixtures'/fixture['name'],'rt')))
     transport=next(x for x in rows if x.get('function')=='point-transport-entry')
     expected=[w for i in range(transport['transport_input_point_size']) for w in transport[f'transport_input_point_{i}']['words'][:7]]
     actual=list(map(int,line.split()));assert actual[0]==fixture['words'][-1] and actual[1:]==expected,fixture['name'];result['raw_geometry_exact']+=1
  results.append(result);print('O'+optimization,len(fixtures),'SAT and ordered contact geometry batches raw-float32 exact')
args.out.write_text(json.dumps({'scope':'C++ O0/O2 independent Native float32 fixture verification: six axis overlaps, six projected distances, nine absolute rotation coefficients, selected face, every ordered contact point/normal/separation. Includes tilted cars; no fitted constants.','header_sha256':hashlib.sha256(header.read_bytes()).hexdigest(),'results':results,'goal_complete':False},indent=2)+'\n')
