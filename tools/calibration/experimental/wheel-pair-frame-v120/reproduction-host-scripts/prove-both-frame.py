from pathlib import Path
import json,gzip,struct,subprocess
r=Path.cwd();w=r/'work/pair-pole-v120';bits=lambda f:struct.unpack('<I',struct.pack('<f',f))[0];fixtures=[]
for tick in [31,32]:
 for rep in [0,1]:
  n=list(map(json.loads,gzip.open(r/f'outputs/pair-pole-v120/native-kernel/kernel-pair-stock-pole-1-{tick}-{rep}-private.jsonl.gz','rt')));s=list(map(json.loads,gzip.open(r/f'outputs/pair-pole-v120/mac-telemetry/pair-stock-pole-1-{tick}-{rep}-prep.jsonl.gz','rt')));assert len(s)==4
  wheels=[x for x in n if x.get('function')=='wheel-spring-prepare'];by_point={tuple(x['wheel_backend']['words'][116:119]):x for x in wheels};entries={x['ecx']:x for x in n if x.get('function')=='spring-row-entry'};by_slot={entries[x['esi']]['spring_object']['words'][7]:x for x in n if x.get('function')=='spring-row-ready'}
  for x in s:
   wheel=by_point[tuple(map(bits,x['world_point']))];slot=wheel['wheel_tail']['words'][69]&0x3ffff;prepared=by_slot[slot];entry=entries[prepared['esi']];row=prepared['constraint_row']['words'];a=entry['body0']['words'];b=entry['body1']['words'];assert list(map(bits,x['quaternion']+x['center']))==a[28:32]+a[25:28];assert list(map(bits,x['contact_quaternion_v120']+x['contact_center_v120']))==b[28:32]+b[25:28]
   incoming=list(map(bits,x['quaternion']+x['world_point']+x['center']+x['world_axis']+x['contact_quaternion_v120']+x['world_point']+x['contact_center_v120']+x['world_axis']));fixtures.append({'tick':tick,'repeat':rep,'Source_wheel':x['wheel'],'spring_slot':slot,'both_actual_incoming_quaternions_centers_and_world_point_words_exact':True,'input':incoming,'expected':row[4:7]+row[12:15]+row[8:11]+row[16:19],'old_Source_Jacobians':list(map(bits,x['spring']['jacobian']+x['contact_spring_jacobian_v120']))})
for opt in ['O0','O2']:
 exe=w/('verify-both-frame-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/build/macos-arm64-m10/_deps/rrr3d_jolt-src/Jolt/Physics/Constraints/ConstraintPart',str(w/'verify-both-frame.cpp'),'-o',str(exe)],check=True);actual=[list(map(int,l.split())) for l in subprocess.check_output([str(exe)],input='\n'.join(' '.join(map(str,x['input'])) for x in fixtures)+'\n',text=True).splitlines()]
 for x,a in zip(fixtures,actual):assert a==x['expected'],(opt,x['tick'],x['Source_wheel'],a,x['expected'])
 print(opt,'all16 actual Native rows both axes and Jacobians exact; actual bothbody inputs match')
(w/'both-frame-proof.json').write_text(json.dumps({'scope':'All actual Source vs actual immutable Native incoming body poses/centers and wheel world points raw exact, actual manager slot binding. Existing actor frame helper reproduces both independently prepared axes and Jacobians on all16 contact rows at31/32,2reps,O0/O2. No runtime correction yet.','fixtures':fixtures,'goal_complete':False},indent=2)+'\n')
