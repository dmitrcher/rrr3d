from pathlib import Path
import json,gzip,struct,subprocess
r=Path.cwd();w=r/'work/steer-transition-v118';bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
fixtures=[]
for tick in [239,240]:
 for rep in range(2):
  native=list(map(json.loads,gzip.open(r/f'outputs/steer-transition-v118/native/kernel-steer-left-{tick}-{rep}-private.jsonl.gz','rt')));source=list(map(json.loads,gzip.open(r/f'outputs/steer-transition-v118/mac-telemetry/steer-left-{tick}-{rep}-prep.jsonl.gz','rt')))
  wheels=[x for x in native if x.get('function')=='wheel-spring-prepare'];assert len(wheels)==4
  slots={x['wheel_tail']['words'][69]&0x3ffff:i for i,x in enumerate(wheels)};assert len(slots)==4
  objects={x['ecx']:x['spring_object']['words'][7] for x in native if x.get('function')=='spring-row-entry'}
  for x in native:
   if x.get('function')!='spring-row-ready' or objects[x['esi']] not in slots:continue
   wheel=slots[objects[x['esi']]];row=x['constraint_row']['words'];s=next(y for y in source if y['wheel']==wheel)
   inputs=list(map(bits,s['quaternion']+s['world_point']+s['center']+s['world_axis']))
   fixtures.append({'tick':tick,'repeat':rep,'wheel':wheel,'steer_angle':s['steer_angle'],'input':inputs,'expected_native_jacobian':row[12:15],'old_source_jacobian':list(map(bits,s['spring']['jacobian']))})
assert len(fixtures)==16
include='/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/_deps/rrr3d_jolt-src/Jolt/Physics/Constraints/ConstraintPart'
checks=[]
for opt in ['O0','O2']:
 exe=w/('verify-frame-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-I'+include,str(w/'verify-frame.cpp'),'-o',str(exe)],check=True)
 text='\n'.join(' '.join(map(str,x['input'])) for x in fixtures)+'\n';results=[list(map(int,line.split())) for line in subprocess.check_output([str(exe)],input=text,text=True).splitlines()]
 assert results==[x['expected_native_jacobian'] for x in fixtures],[(x['tick'],x['wheel'],actual,x['expected_native_jacobian']) for x,actual in zip(fixtures,results) if actual!=x['expected_native_jacobian']]
 checks.append({'optimization':opt,'actual_native_jacobians_exact':len(results)})
(w/'frame-proof.json').write_text(json.dumps({'scope':'Existing original frame arithmetic on actual Source pose/contact point/normal inputs reproduces every Native prepared spring Jacobian across zero/nonzero steering transition. Native actual handle slots bind wheel identity. No arbitrary permutation or fitted values.','fixtures':fixtures,'checks':checks,'old_nonzero_steer_row_mismatches':sum(x['old_source_jacobian']!=x['expected_native_jacobian'] for x in fixtures),'goal_complete':False},indent=2)+'\n');print(checks)
