from pathlib import Path
import json,gzip,subprocess
r=Path.cwd();w=r/'work/static-pair-order-v108';active=Path('/Users/dmitrijcerednicenko/Documents/Codex/2026-07-22/files-mentioned-by-the-user-rrr3d/outputs/rrr3d');reports=[];fixtures=[]
for rep in range(2):
 rows=list(map(json.loads,gzip.open(r/f'outputs/static-pair-order-v108/native-curb-pair-window224/endpoint-curb-side-224-{rep}-private.jsonl.gz','rt')));events={t:{'created':[],'removed':[]} for t in range(225)}
 for row in rows:
  if row.get('function') in ['broadphase-created-list','broadphase-removed-list']:
   kind='created' if row['function']=='broadphase-created-list' else 'removed';words=row[kind+'_pairs']['words']
   for i in range(0,len(words),4):
    pair=[(x&0x3ffff)+1 for x in words[i:i+2]]
    if any(3<=x<=6 for x in pair):events[row['sample_tick_v71']][kind].append(pair)
 observed={}
 for tick in [19,203,220,224]:
  path=r/f'outputs/static-pair-order-v108/native-manager-slots/creation-curb-side-{tick}-{rep}-private.jsonl.gz' if tick!=224 else r/f'outputs/static-box-query-v107/native-next-queue224/creation-curb-side-224-{rep}-private.jsonl.gz';data=list(map(json.loads,gzip.open(path,'rt')));mapping={x['enqueue_wheel']['address']+4:(x['enqueue_wheel']['words'][2]&0x3ffff)+1 for x in data if x.get('function')=='wheel-enqueue'};items=[]
  for row in data:
   if row.get('function')=='wheel-contact-manager':
    words=row['contact_manager']['words'];wheel=mapping.get(words[9],mapping.get(words[10]));assert wheel;support=7 if tick==19 and wheel in [3,5] or tick==203 and wheel==5 else 1;items.append({'pair':[wheel,support],'actual_Native_wheel_processor_slot':words[5]})
  observed[tick]=items
 inputs='225\n';expected=[]
 for tick in range(225):
  e=events[tick];fields=[tick,len(e['created'])]+[v for pair in e['created'] for v in pair]+[len(e['removed'])]+[v for pair in e['removed'] for v in pair]+[len(observed.get(tick,[]))]+[v for item in observed.get(tick,[]) for v in item['pair']];inputs+=' '.join(map(str,fields))+'\n';expected.append([tick]+[item['actual_Native_wheel_processor_slot']+1 for item in observed.get(tick,[])])
 for opt in ['O0','O2']:
  exe=w/('manager-list-'+opt);subprocess.run(['clang++','-std=c++17','-'+opt,'-I'+str(active/'src/Rock3dEngine/header/physics'),str(w/'manager-list-proof.cpp'),'-o',str(exe)],check=True);actual=[list(map(int,line.split())) for line in subprocess.check_output([str(exe)],input=inputs,text=True).splitlines()];assert actual==expected,(rep,opt,[(a,b) for a,b in zip(actual,expected) if a!=b]);reports.append({'repeat':rep,'optimization':opt,'all_observed_actual_Native_processor_slots_exact':True,'observed':observed})
 fixtures.append({'input':inputs,'expected':expected})
(w/'manager-list-fixtures.json').write_text(json.dumps({'fixtures':fixtures},indent=2)+'\n');(r/'outputs/static-pair-order-v108').mkdir(exist_ok=True);(r/'outputs/static-pair-order-v108/native-manager-list-proof.json').write_text(json.dumps({'scope':'Actual original Native wheel-only contact-manager processor slots:19,203,220,224. Replay actual original broadphase pair births/removals through production generic helper; body managers correctly excluded. Removal deferred one step, last wheel-manager slot replaces vacancy. Source geometry inference separately checked. No frame/case code in production helper.','checks':reports,'goal_complete':False},indent=2)+'\n');print('Two Native repeats x O0/O2: every observed wheel processor slot exact with generic deferred last-slot replacement.')
