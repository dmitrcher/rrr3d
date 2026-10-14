from pathlib import Path
import json,gzip,struct,subprocess
b=lambda f:struct.unpack('<I',struct.pack('<f',f))[0];v=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
report=[];inputs=[]
for track,tick in [(48,781),(16,787)]:
 r=list(map(json.loads,gzip.open(f'outputs/post-impulse-v132/native-anchor-fixed/track{track}-run0-private.jsonl.gz','rt')));gates=[x for x in r if x.get('function')=='tire-anchor-gate'];s=[x for x in map(json.loads,open(f'outputs/post-impulse-v132/source-anchor/track{track}-anchor.jsonl')) if x['step']==tick]
 for x in gates:
  w=x['wheel_contact']['words'];m=next(z for z in s if list(map(b,z['current']))==w[:3]);ready=next(z for z in r if z.get('function')=='tire-anchor-bias-ready' and z['ebx']==x['ebx']);expected=ready['prepare_frame']['words'][63:65];word_input=w[:3]+w[22:25]+w[6:9]+w[9:12]+x['wheel_skin']['words'][:1]+w[19:20]+[int(any(z.get('function')=='tire-anchor-update' and z['ebx']==x['ebx'] for z in r))]+expected;inputs.append(word_input)
  result=dict(track=track,tick=tick,wheel=m['wheel'],current_point_exact=True,native_previous=list(map(v,w[22:25])),source_previous=m['previous'],previous_exact=list(map(b,m['previous']))==w[22:25],native_errors=list(map(v,expected)),source_errors=[m['lateral_error'],m['longitudinal_error']],native_lateral=list(map(v,w[6:9])))
  report.append(result);print(result)
p=Path('work/post-impulse-v132');data=str(len(inputs))+'\n'+'\n'.join(' '.join(map(str,x)) for x in inputs)+'\n';(p/'native-anchor-inputs.txt').write_text(data)
inc='/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics'
for opt in ['O0','O2']:
 exe=p/f'verify-anchor-{opt}';subprocess.run(['clang++','-std=c++17','-'+opt,'-I'+inc,'work/tire-anchor-v91/verify-native-anchor-inputs.cpp','-o',str(exe)],check=True);z=subprocess.run([str(exe)],input=data,text=True,capture_output=True);print(opt,z.returncode,z.stdout,z.stderr)
(p/'anchor-input-proof.json').write_text(json.dumps(report,indent=2)+'\n')
