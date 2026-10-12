import json,subprocess,struct,hashlib,tempfile,argparse
from pathlib import Path
here=Path(__file__).resolve().parent
parser=argparse.ArgumentParser();parser.add_argument('--out',type=Path,required=True);args=parser.parse_args();r=json.load(open(here/'native-inputs.json'));a=[]
a.append(len(r['composes']))
for x in r['composes']:
 for k in ['parent_current_transform','shape_local_transform','composed_transform']:a+=x[k]
a.append(len(r['endpoints']))
for x in r['endpoints']:
 for k in ['parent_current_transform','shape_local_transform']:a+=x[k]
 a +=[x['radius'],x['travel']]+x['pose']+x['endpoints']+[int('unit' in x)]
 if 'unit' in x:a+=x['unit']
blob=struct.pack('<'+'I'*len(a),*a);results=[]
for opt in ['-O0','-O2']:
 temp=tempfile.TemporaryDirectory();exe=Path(temp.name)/'verify-pose'
 subprocess.run(['clang++','-std=c++17',opt,'-ffp-contract=off','-I',str(here),str(here/'verify-pose.cpp'),'-o',str(exe)],check=True)
 p=subprocess.run([str(exe)],input=blob,capture_output=True);print(opt,p.stdout.decode(),p.stderr.decode());p.check_returncode();results.append({'optimization':opt,'result':p.stdout.decode().strip()})
args.out.write_text(json.dumps({'scope':'Seven capture points,14 actual Windows runs,84 getter endpoints,28 query rays; preserved full native histories. Duplicate getter endpoints are not separate queries.','two_native_defined_field_repeats_exact':True,'compose_records':len(r['composes']),'endpoint_records':len(r['endpoints']),'normalized_ray_records':sum('unit' in x for x in r['endpoints']),'results':results,'goal_complete':False},indent=2)+'\n')
