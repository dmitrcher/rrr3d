from pathlib import Path
import json,struct,subprocess,gzip
b=lambda f:struct.unpack('<I',struct.pack('<f',f))[0];v=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
inc='/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics'
subprocess.run(['clang++','-std=c++17','-O2','-ffp-contract=off','-I'+inc,'work/energy-sleep-v127/check-body-bounds.cpp','-o','work/energy-sleep-v127/check-body-bounds'],check=True)
mesh_input=json.load(open('work/energy-sleep-v127/initial-bounds-hypothesis.json'))['inputs']
mesh_bounds=[list(map(int,l.split())) for l in subprocess.check_output(['work/energy-sleep-v127/check-bounds'],input='\n'.join(' '.join(map(str,x)) for x in mesh_input)+'\n',text=True).splitlines()]
poses=list(map(json.loads,open('outputs/energy-sleep-v127/source/poses.jsonl')));before={x['step']:x for x in poses if x['phase']=='before-update'};after={x['step']-1:x for x in poses if x['phase']=='after-restore'}
samples=[x['sample'] for x in map(json.loads,open('outputs/energy-sleep-v127/source/physical.jsonl')) if x['event']=='solo.sample']
w=list(map(int,' '.join(open('outputs/real-map-v124/repeats/track0-run0.words').read().splitlines()[1:]).split()));half=w[8:11];offset=[b(v(w[11+i])-v(w[14+i])) for i in range(3)]
body_inputs=[list(map(b,before[t]['rotation']+before[t]['center']))+offset+half for t in range(80)]
body_bounds=[list(map(int,l.split())) for l in subprocess.check_output(['work/energy-sleep-v127/check-body-bounds'],input='\n'.join(' '.join(map(str,x)) for x in body_inputs)+'\n',text=True).splitlines()]
native=[x for x in map(json.loads,gzip.open('outputs/energy-sleep-v127/native-attempt2/run0-private.jsonl.gz','rt')) if x.get('function')=='energy-sleep-entry' and abs(v(x['energy_body']['words'][120])-.0005)<1e-8]
inputs=[];counts=[];fail=[]
for t in range(80):
 bb=list(map(v,body_bounds[t]));pairs=[i for i,bounds in enumerate(mesh_bounds) if all(bb[a]<=v(bounds[a+3]) and bb[a+3]>=v(bounds[a]) for a in range(3))];contacts=sum(x['contact'] for x in samples[t]['wheels']);count=len(pairs)+contacts;n=native[t]['energy_body']['words'];s=after[t];p=before[t]
 word_input=list(map(b,s['center']+p['center']+s['rotation']+p['rotation']+s['inverse_inertia']+[s['inverse_mass'],v(w[25]),1/60]))+[count]
 expected=n[45:48]+n[60:63]+n[48:52]+n[63:67]+n[121:124]+n[120:121]+n[95:96]+[b(1/60),sum(n[8:12])]
 assert len(word_input)==len(expected)==21
 differences=[i for i,(a,e) in enumerate(zip(word_input,expected)) if a!=e and (a&0x7fffffff or e&0x7fffffff)]
 if differences:fail.append(dict(tick=t,fields=differences))
 counts.append(dict(tick=t,source_mesh_overlaps=pairs,source_wheel_contacts=contacts,source_count=count,native_count=sum(n[8:12])));inputs.append(word_input)
Path('work/energy-sleep-v127/source-input-proof.json').write_text(json.dumps(dict(scope='Actual Source private poses/mass + Source fixture + source-only cooked mesh bounds + full chassis bounds/wheel contacts. Mesh-only body interaction candidate; decoration/compound/dynamic actor counting not yet certified.',frames=80,failures=fail,counts=counts,inputs=inputs,goal_complete=False),indent=2)+'\n');print('source input frames80 mismatches',len(fail),fail[:8])
