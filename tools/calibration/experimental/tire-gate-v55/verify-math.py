from pathlib import Path
import json,struct,subprocess,argparse,tempfile
here=Path(__file__).resolve().parent;parser=argparse.ArgumentParser();parser.add_argument('--out',type=Path,required=True);args=parser.parse_args();r=json.load(open(here/'native-inputs.json'));data=[len(r['curves'])]
for x in r['curves']:data+=x['parameters']+[x['slip'],x['result']]
data+=[len(r['gates'])]
for x,y in zip(r['gates'],r['scaled']):data+=x['pose']+x['normal']+x['lateral']+x['longitudinal']+[y['projection']]+x['point']+x['previous']+[x['skin'],x['axle'],x['update'],x['lateral_error'],x['longitudinal_error']]
data+=[len(r['scaled'])]
for x in r['scaled']:data+=[x[k] for k in ['normal_force','projection','dt','lateral_mu','long_mu','scalar','lateral_limit','long_limit']]
blob=struct.pack('<'+'I'*len(data),*data);results=[]
for opt in ['-O0','-O2']:
 temp=tempfile.TemporaryDirectory();exe=Path(temp.name)/'verify-tire';subprocess.run(['clang++','-std=c++17',opt,'-ffp-contract=off','-I',str(here),'-I',str(here),str(here/'verify-tire.cpp'),'-o',str(exe)],check=True);p=subprocess.run([str(exe)],input=blob,capture_output=True);print(opt,p.stdout.decode(),p.stderr.decode());p.check_returncode();results.append({'optimization':opt,'result':p.stdout.decode().strip()})
args.out.write_text(json.dumps({'actual_native_repeats':2,'unchanged_reference_states':600,'capture_points':7,'curves':56,'zero_steer_basis_anchor_projections':28,'normal_force_scales_and_limits':28,'results':results,'goal_complete':False},indent=2)+'\n')
