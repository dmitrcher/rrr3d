from pathlib import Path
import json,struct,gzip,subprocess
p=Path('work/sleep-inputs-v160');b=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];v=lambda u:struct.unpack('<f',struct.pack('<I',u))[0];base=Path('outputs/sleep-inputs-v160/source-managers')
r=list(map(json.loads,open(base/'track64-physical.jsonl')));states={x['sample']['tick']:x['sample'] for x in r if x['event']=='solo.sample'};drive={x['sample']['tick']:x['sample'] for x in r if x['event']=='solo.drive'};poses=list(map(json.loads,open(base/'track64-pose.jsonl')));before={x['step']:x for x in poses if x['phase']=='before-update'};after={x['step']-1:x for x in poses if x['phase']=='after-restore'};managers={x['step']:x for x in map(json.loads,open(base/'managers.jsonl'))};energy={x['step']:x for x in map(json.loads,open(base/'energy.jsonl'))};checks=[]
inc='/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d/src/Rock3dEngine/header/physics'
for opt in ['O0','O2']:
 exe=p/('check-continuous-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-ffp-contract=off','-I'+inc,str(p/'check-continuous.cpp'),'-o',str(exe)],check=True)
 for count_managers,wake_removed_managers in [(False,False),(True,False),(False,True),(True,True)]:
  inputs=[];ticks=[]
  for t,z in sorted(energy.items()):
   if t>655:continue
   a=after[t];prev=before[t];m=managers[t];wheels=sum(w['contact'] for w in states[t]['wheels']);count=z['interactions']-wheels+len(m['active_wheel_pairs']) if count_managers else z['interactions'];body=m['body_shapes'][0];active={tuple(sorted(x)) for x in m['active_wheel_pairs']}
   body_loss=any(body in pair for pair in m['removed']);wheel_loss=any(tuple(sorted(pair)) in active for pair in m['removed']);wake_after=body_loss or (wake_removed_managers and wheel_loss)
   inputs.append(list(map(b,a['center']+prev['center']+a['rotation']+prev['rotation']+a['inverse_inertia']+[a['inverse_mass'],.05,1/60]))+[count,int(drive[t]['wake_event']),int(wake_after)]);ticks.append(t)
  result=[list(map(int,l.split())) for l in subprocess.check_output([str(exe)],input='\n'.join(' '.join(map(str,x)) for x in inputs)+'\n',text=True).splitlines()]
  for rep in [0,1]:
   frames={}
   for x in map(json.loads,gzip.open(f'outputs/sleep-inputs-v160/native-energy656-v160/run{rep}-private.jsonl.gz','rt')):
    if 'energy_body' in x and abs(v(x['energy_body']['words'][120])-.0005)<1e-8:frames.setdefault(x['sample_tick_v71'],{})[x['function']]=x
   differences=[]
   for tick,out in zip(ticks,result):
    d=frames[tick];sw=d['energy-wake-store'];expected=sw['energy_body']['words'][97:103]+[sw['xmm_words'][0][0]]
    if 'energy-threshold-test' in d:expected +=[d['energy-threshold-test']['xmm_words'][0][0],d['energy-threshold-test']['xmm_words'][1][0]]
    else:expected +=[0,0]
    expected +=[bool(d['energy-sleep-exit']['energy_body']['words'][84]&0x100000)]
    fields=[i for i,(a,bits) in enumerate(zip(out,expected)) if a!=bits and (a&0x7fffffff or bits&0x7fffffff)]
    if fields:differences.append(dict(step=tick,fields=fields))
   item=dict(optimization=opt,repeat=rep,count_potential_managers=count_managers,wake_removed_wheel_managers=wake_removed_managers,frames=len(ticks),difference_count=len(differences),first_difference=differences[0] if differences else None);checks.append(item);print(item)
(p/'continuous-source-proof.json').write_text(json.dumps(dict(scope='Continuously evolving original energy helper driven only by Source poses,mass,geometry-derived manager counts/lifecycle and recorded Source command events; no Native cache/state reset injection.655 last same physical step; Native656 excluded as a consequence of prior Source sleep.',checks=checks,goal_complete=False),indent=2)+'\n')
