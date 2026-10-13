from pathlib import Path
import json,gzip,struct,subprocess,os
root=Path.cwd();w=root/'work/tire-angle-v100';native=root/'outputs/tire-angle-v100/native-angle';manifest=json.loads((native/'manifest.json').read_text());bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];flt=lambda w:struct.unpack('<f',struct.pack('<I',w))[0];fixtures=[];binding=[]
for cap in manifest['captures']:
 name,tick,rep=cap['case'],cap['physical_tick'],cap['repeat'];tag=f'{name}-{tick}-{rep}';r=list(map(json.loads,gzip.open(native/f'{tag}-private.jsonl.gz','rt')));a=[x for x in r if x.get('function')=='tire-angle-argument-ready'];b=[x for x in r if x.get('function')=='tire-angle-stored'];mac=list(map(json.loads,open(root/f'outputs/tire-angle-v100/mac-readonly/{tag}-angle.jsonl')));assert len(a)==len(b);matches=[]
 for i,(n,s) in enumerate(zip(a,b)):
  f=n['prepare_frame']['words'];point=n['wheel_contact']['words'][:3];v=f[13:16]+n['wheel_contact']['words'][6:9];m=next((x for x in mac if x['words'][9:12]==point),None)
  if m:matches.append({'wheel':m['wheel'],'vectors_exact':m['words'][:6]==v,'argument_exact':m['words'][6]==f[6]})
  fixtures.append({'case':name,'tick':tick,'repeat':rep,'call':i,'actual_native_input_words':v,'actual_native_cosine_word':f[6],'actual_native_angle_word':s['prepare_frame']['words'][26]})
 binding.append({'case':name,'tick':tick,'repeat':rep,'Native_calls':len(a),'exact_world_point_matches':len(matches),'matched_inputs':matches})
assert len(manifest['captures'])==16 and len(fixtures)==52
data='\n'.join(' '.join(format(flt(v),'.9g') for v in f['actual_native_input_words']) for f in fixtures)+'\n';env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))};inc=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics');source=(w/'angle-probe.cpp').read_text().replace('NativeTireCandidateV100.h','NativeTireV55.h');(w/'production-angle-probe.cpp').write_text(source);reports=[]
for opt in ['O0','O2']:
 exe=w/f'production-angle-{opt}';subprocess.run(['xcrun','clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+str(inc),str(w/'production-angle-probe.cpp'),'-o',str(exe)],check=True)
 def run(e):return [list(map(float,l.split())) for l in subprocess.check_output([str(exe)],input=data.encode(),env=e).decode().splitlines()]
 new=run(dict(env,RRR3D_EXPERIMENT_NATIVE_TIRE_ANGLE_V100='1'));old=run(env);assert len(new)==len(old)==len(fixtures)
 nd=[(f['case'],f['tick'],f['call']) for a,f in zip(new,fixtures) if bits(a[0])!=f['actual_native_angle_word']];cd=[(f['case'],f['tick'],f['call']) for a,f in zip(new,fixtures) if bits(a[1])!=f['actual_native_cosine_word']];od=[(f['case'],f['tick'],f['call']) for a,f in zip(old,fixtures) if bits(a[0])!=f['actual_native_angle_word']];assert not nd and not cd,(nd,cd)
 reports.append({'optimization':opt,'actual_Native_angles_exact':len(fixtures),'actual_Native_arguments_exact':len(fixtures),'default_float_angle_mismatches':od})
report={'scope':'Production header on all52 independently captured original Windows asin arguments/results across8 selections and2 repeats. 9600original states Golden exact. Actual Mac no-mutation traces preserve9600acceptedV99states; exact same live input binding explicitly restricted to listed matches.','arithmetic':reports,'fixtures':fixtures,'live_mac_input_binding':binding,'goal_complete':False};(root/'outputs/tire-angle-v100/expanded-same-input-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Production52Native angles and arguments O0/O2 exact; old mismatches',len(reports[0]['default_float_angle_mismatches']),flush=True)
