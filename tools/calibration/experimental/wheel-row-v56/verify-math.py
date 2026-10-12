from pathlib import Path
import argparse,tempfile,subprocess,json
p=argparse.ArgumentParser();p.add_argument('--out',type=Path,required=True);args=p.parse_args();here=Path(__file__).resolve().parent;results=[];rows=json.loads((here/'expanded-native-inputs.json').read_text())['rows'];assert len(rows)==84
s=['#include "NativeWheelRowV56.h"','#include <cstdint>','#include <cstring>','#include <cstdio>','float f(uint32_t w){float x;std::memcpy(&x,&w,4);return x;}','uint32_t bits(float x){uint32_t w;std::memcpy(&w,&x,4);return w;}','int main(){unsigned checked=0;']
for i,x in enumerate(rows):
 r=x['row'];s+=['{ motorrock_native_wheel_row_v56::Matrix m{'+','.join(f'f({v}u)' for v in x['inertia'])+'}; motorrock_native_wheel_row_v56::Vector r{'+','.join(f'f({v}u)' for v in r[8:11])+'};auto v=motorrock_native_wheel_row_v56::multiply(m,r);']
 for j in range(3):s.append(f'if(bits(v[{j}])!={r[16+j]}u){{std::fprintf(stderr,"row {i} component {j}: %08x expected %08x\\n",bits(v[{j}]),{r[16+j]}u);return 1;}}++checked;')
 if r[15]==0:
  s.append('motorrock_native_wheel_row_v56::Vector n{'+','.join(f'f({v}u)' for v in r[:3])+'};float k=motorrock_native_wheel_row_v56::inverse_mass(n,r,v,f('+str(x['inverse_mass'])+'u));float mass=motorrock_native_wheel_row_v56::div(1,k);if(bits(mass)!='+str(r[7])+'u){std::fprintf(stderr,"mass row '+str(i)+': %08x expected %08x\\n",bits(mass),'+str(r[7])+'u);return 2;}++checked;')
 s.append('}')
s.append('std::printf("%u original inverse-jacobian/hard-mass float32 words exact\\n",checked);}')
with tempfile.TemporaryDirectory(prefix='motorrock-wheel-row-v56-math-') as tmp:
 source=Path(tmp)/'verify.cpp';source.write_text('\n'.join(s)+'\n');assert source.read_text()==(here/'verify-expanded.cpp').read_text()
 for opt in ['-O0','-O2']:
  exe=Path(tmp)/'verify';subprocess.run(['clang++','-std=c++17',opt,'-ffp-contract=off','-I',str(here),str(source),'-o',str(exe)],check=True);r=subprocess.run([str(exe)],text=True,capture_output=True,check=True);assert r.stdout.strip()=='308 original inverse-jacobian/hard-mass float32 words exact';results.append({'optimization':opt,'result':r.stdout.strip()});print(opt,r.stdout.strip(),flush=True)
args.out.parent.mkdir(parents=True,exist_ok=True);args.out.write_text(json.dumps({'prepared_rows':84,'inverse_jacobian_words':252,'hard_effective_masses':56,'results':results,'scope':'7 original capture points,2actual Native repetitions each; static other body. Generated C++ independently recreated from selected inputs and checked against recorded source. No full trajectory parity.','goal_complete':False},indent=2)+'\n')
