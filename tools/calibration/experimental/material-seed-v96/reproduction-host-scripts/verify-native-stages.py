from pathlib import Path
import json,gzip,struct,subprocess,hashlib
root=Path.cwd(); work=root/'work/contact-mass-v96'; inc=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics')
bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
f32=lambda x:f(bits(x))
add=lambda a,b:f32(a+b);sub=lambda a,b:f32(a-b);mul=lambda a,b:f32(a*b)
def cross(a,b):return [sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))]
(work/'seed-probe.cpp').write_text('''#include <cstdio>
#include "NativeMaterialFrameCandidateV96.h"
int main(){float v[8];while(scanf("%f",v)==1){for(int i=1;i<8;++i)if(scanf("%f",v+i)!=1)return 2;
using namespace motorrock::framev36;auto q=materialSeedV96({{-v[0],-v[1],-v[2],v[3]}},{{v[4],v[5],v[6],v[7]}});for(int i=0;i<4;++i)printf("%.9g ",q[i]);puts("");}}''')
fixtures=[];bound=[]
for rep in range(2):
 p=next((root/'outputs/combined-tire-v95/native-next-tilt-contact10').glob('*-'+str(rep)+'-private.jsonl.gz'))
 records=[json.loads(l) for l in gzip.open(p,'rt')];records=[r for r in records if r.get('sample_tick_v71')==10]
 rows={name:[r for r in records if r.get('function')==name] for name in ['material-inputs-ready','material-quaternion-adjusted','material-error-ready','set-frame0-entry','solver-initialize-exit']}
 for index,(m,e) in enumerate(zip(rows['material-quaternion-adjusted'],rows['material-error-ready'])):
  mw=list(map(f,m['material_stack']['words']));ew=list(map(f,e['material_stack']['words']));actor=mw[31:35];material=[-mw[9],-mw[10],-mw[11],mw[12]]
  assert list(map(bits,actor))==list(map(bits,ew[31:35]));assert list(map(bits,material))==e['material_stack']['words'][16:20]
  fixtures.append({'repeat':rep,'index':index,'actor':actor,'material':material,'expected_seed':ew[38:42]})
 stage=json.loads((root/'outputs/contact-mass-v96/material-stage-same-input.json').read_text())
 mac=stage['mac_input'];new=stage['runs']['O2'];actual_actor=list(map(f,rows['material-inputs-ready'][0]['material_stack']['words'][36:40]))
 assert list(map(bits,actual_actor))==list(map(bits,mac['physical_actor']))
 assert list(map(bits,fixtures[-8]['material']))==list(map(bits,mac['material_world']))
 seed_set=rows['set-frame0-entry'][0]['setter_input']['words'][3:7];callback_set=rows['set-frame0-entry'][4]['setter_input']['words'][3:7]
 assert list(map(bits,new['seed']))==seed_set
 assert list(map(bits,new['callback_local0']))==callback_set
 for i,row in enumerate(rows['solver-initialize-exit']):
  words=row['native_solver_row']['words'];values=list(map(f,words));body=list(map(f,row['native_body0']['words']))
  assert list(map(bits,new['solver_quaternion']))==words[16:20]
  # Existing scalar effective-mass arithmetic, unchanged. Only the normal changes.
  n=list(map(f32,new['normal']));lever=values[4:7];tensor=body[15:24];v=cross(lever,n)
  response=[add(add(mul(tensor[j],v[0]),mul(tensor[j+3],v[1])),mul(tensor[j+6],v[2])) for j in range(3)]
  c=cross(response,lever);projection=add(add(mul(n[2],c[2]),mul(n[0],c[0])),mul(n[1],c[1]))
  mass=f32(1/add(body[11],projection))
  assert bits(mass)==words[20],(rep,i,mass,values[20])
 bound.append({'repeat':rep,'actor_material_input_words_exact':8,'seed_words_exact':4,'callback_words_exact':4,'solver_rows_quaternion_and_mass_exact':len(rows['solver-initialize-exit']),'old_seed_mismatch':bits(mac['seed'][0])!=seed_set[0]})
data='\n'.join(' '.join(format(x,'.9g') for x in r['actor']+r['material']) for r in fixtures)+'\n'
proofs=[]
for opt in ['O0','O2']:
 exe=work/('seed-probe-'+opt);subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(inc),str(work/'seed-probe.cpp'),'-o',str(exe)],check=True)
 got=[list(map(float,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode()).decode().splitlines()]
 assert len(got)==len(fixtures)==16
 assert all(list(map(bits,a))==list(map(bits,r['expected_seed'])) for a,r in zip(got,fixtures))
 proofs.append({'optimization':opt,'actual_native_fixtures':16,'exact_words':64});print(opt,'16 actual Native seed fixtures exact,64 words')
report={'scope':'Bound actual native material inputs, seed, callback, solver quaternion and unchanged effective-mass arithmetic. Pure diagnostic; no oracle state injection.','bound_repeats':bound,'seed_arithmetic':proofs,'fixtures':fixtures,'goal_complete':False}
(root/'outputs/contact-mass-v96/native-stage-causal-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('both native20row quaternions and masses exactly recovered with corrected upstream seed')
