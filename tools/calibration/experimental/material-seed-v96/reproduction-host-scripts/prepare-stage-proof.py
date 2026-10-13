from pathlib import Path
import json,subprocess,os,struct,gzip
root=Path.cwd(); work=root/'work/contact-mass-v96'; inc=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics')
source=(inc/'NativeMaterialFrameV36.h').read_text()
helper='''inline cf::Quaternion materialSeedV96(cf::Quaternion a,cf::Quaternion b) {
 const float x=a[0],y=a[1],z=a[2],w=a[3],X=b[0],Y=b[1],Z=b[2],W=b[3];
 return {{sub(add(add(mul(y,Z),mul(W,x)),mul(w,X)),mul(z,Y)),
          sub(add(add(mul(W,y),mul(w,Y)),mul(z,X)),mul(x,Z)),
          sub(add(add(mul(W,z),mul(w,Z)),mul(x,Y)),mul(y,X)),
          sub(sub(sub(mul(w,W),mul(x,X)),mul(y,Y)),mul(z,Z))}};
}
'''
assert source.count('const auto seed=framev36::multiply(inverse,r.errors.material)')==1
(work/'NativeMaterialFrameCandidateV96.h').write_text(source.replace('struct Result {',helper+'struct Result {').replace('const auto seed=framev36::multiply(inverse,r.errors.material)','const auto seed=framev36::materialSeedV96(inverse,r.errors.material)'))
(work/'stage-probe.cpp').write_text('''#include <cstdio>
#include <initializer_list>
#include "NativeMaterialFrameCandidateV96.h"
int main(){namespace cf=motorrock::contactframe;namespace f=motorrock::framev36;
float v[16];while(scanf("%f",v)==1){for(int i=1;i<16;++i)if(scanf("%f",v+i)!=1)return 2;
cf::Matrix m;for(int c=0;c<3;++c)for(int r=0;r<3;++r)m.a[r][c]=v[4+c*3+r];
auto result=f::compute(m,{{v[0],v[1],v[2],v[3]}},{{v[13],v[14],v[15]}},{{v[13],v[14],v[15]}},false);
for(auto q:{result.rawLocal,result.errors.raw,result.errors.material,result.seed,result.callback.local0,result.callback.local1,result.solverQuaternion})for(int i=0;i<4;++i)printf("%.9g ",q[i]);
for(int i=0;i<3;++i)printf("%.9g ",result.normal[i]);puts("");}}''')
mac=next(json.loads(l)['sample'] for l in (root/'outputs/contact-mass-v96/mac-readonly-frame10/body-tilt-roll-10-0-physical.jsonl').open() if json.loads(l).get('event')=='physics.native-material-stages')
vals=mac['physical_actor']+mac['api_actor']+mac['callback_plane']; data=' '.join(format(x,'.9g') for x in vals)+'\n'
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};env.update(json.loads((root/'work/frozen-v95/manifest.json').read_text())['environment'])
# The pure helper needs the frame flags normally set by the loaded profile.
for l in Path(env['RRR3D_CALIBRATION_PROFILE']).read_text().splitlines():
 if l.strip().startswith('RRR3D_') and '=' in l:
  k,v=l.split('=',1);env[k.strip()]=v.strip()
report={'scope':'Same-input diagnostic helper only; no live physics edit','mac_input':mac,'runs':{}}
f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
for opt in ['O0','O2']:
 exe=work/('stage-probe-'+opt)
 subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(inc),str(work/'stage-probe.cpp'),'-o',str(exe)],check=True)
 got=list(map(float,subprocess.check_output([str(exe)],input=data.encode(),env=env).split()))
 keys=['raw_local','raw_world','material_world','seed','callback_local0','callback_local1','solver_quaternion']
 stages={k:got[i*4:i*4+4] for i,k in enumerate(keys)};stages['normal']=got[28:31]
 report['runs'][opt]=stages;print(opt,stages)
(root/'outputs/contact-mass-v96/material-stage-same-input.json').write_text(json.dumps(report,indent=2)+'\n')
