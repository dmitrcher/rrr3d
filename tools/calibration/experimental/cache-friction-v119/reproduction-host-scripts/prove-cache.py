from pathlib import Path
import json,gzip,subprocess,struct,os
r=Path.cwd();bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0];fixtures=[]
for tick in [165,166]:
 for rep in [0,1]:
  s=list(map(json.loads,gzip.open(r/f'outputs/body-pitch-v119/mac-contact/body-tilt-pitch-{tick}-{rep}-physical.jsonl.gz','rt')));cache=next(x['sample'] for x in s if x['event']=='physics.native-cache-input');frames=next(x['sample'] for x in s if x['event']=='physics.native-material-stages');n=list(map(json.loads,gzip.open(r/f'outputs/body-pitch-v119/native-contact/v119-body-tilt-pitch-{tick}-{rep}-private.jsonl.gz','rt')));converter=next(x for x in n if x.get('function')=='converted-create-entry');inputerrors=[x['material_input_error']['words'] for x in n if x.get('function')=='material-inputs-ready'];outputerrors=[x['material_output_error']['words'][0] for x in n if x.get('function')=='material-error-ready'];assert len(inputerrors)==len(outputerrors)==4
  for i in range(2):
   raw=converter[f'converter_raw_{i}']['words'];point=cache['before'][i];assert list(map(bits,point[0]+point[1]))==raw[3:6]+raw[6:9]
   incoming=list(map(bits,cache['actor']+cache['ground']+point[0]+point[1]+cache['frame']))+[raw[15]]+list(map(bits,frames['raw_world']+frames['material_world']));assert len(incoming)==43
   fixtures.append({'tick':tick,'repeat':rep,'point':i,'input':incoming,'expected':inputerrors[i]+[outputerrors[i]]})
for opt in ['O0','O2']:
 exe=r/f'work/body-pitch-v119/verify-cache-{opt}';subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics','work/body-pitch-v119/verify-cache.cpp','-o',str(exe)],check=True);env=dict(os.environ,RRR3D_EXPERIMENT_NATIVE_CACHE_WORLD_POINT_V119='1');data='\n'.join(' '.join(map(str,x['input'])) for x in fixtures)+'\n';out=subprocess.check_output([str(exe)],input=data,text=True,env=env)
 for x,l in zip(fixtures,out.splitlines()):assert list(map(int,l.split()))==x['expected'],(opt,x['tick'],x['point'],l,x['expected'])
 print(opt,'all8 actual Native transverse and material error outputs exact')
(r/'work/body-pitch-v119/cache-proof.json').write_text(json.dumps({'scope':'Actual Source pose/cache points/frame inputs, actual Native raw cache transverse and material error outputs; original prepared geometry/cache positions bound word-for-word. Production header O0/O2, two Native repeats, steps165/166. No runtime input injection.','fixtures':fixtures,'goal_complete':False},indent=2)+'\n')
