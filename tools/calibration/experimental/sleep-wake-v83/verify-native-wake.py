from pathlib import Path
import json,gzip,hashlib
here=Path(__file__).resolve().parent;folder=here/'native-wake-captures';manifest=json.load(open(folder/'manifest.json'));windows={}
for r in map(json.loads,gzip.open(here/'captures/windows-reference.jsonl.gz','rt')):
 if r['event']=='calibration.sample':windows.setdefault(r['detail'],[]).append(r['sample'])
assert len(manifest['captures'])==8;groups={}
for p in sorted(folder.glob('*-private.jsonl.gz')):
 raw=gzip.open(p,'rb').read();a=[r for r in map(json.loads,raw.decode('utf-8-sig').splitlines())if'function'in r];assert [r['function']for r in a]==['add-force-entry','wake-counter-getter','wake-counter-loaded','wake-counter-setter','wake-counter-set','add-force-exit'];entry,get,loaded,setter,after,exit=a
 assert entry['input_force']['words']==[0,0,0]and entry['stack']['words'][2:4]==[5,1]
 base=get['stack']['words'][0]-0x127c7f;assert get['wake_vtable']['words'][12]-base==0x1af050 and setter['wake_setter_rva']==0x1b0da0
 before=entry['backend_actor']['words'][103];finish=after['wake_backend']['words'][103];assert before==loaded['wake_backend']['words'][103]==1053049923 and finish==exit['backend_actor']['words'][103]==setter['stack']['words'][0]==1053609164
 assert not(entry['backend_actor']['words'][84]&(1<<20))and not(after['wake_backend']['words'][84]&(1<<20))
 groups.setdefault(entry['sample_tick_v71'],[]).append((before,finish));m=next(x for x in manifest['captures']if x['sample_tick']==entry['sample_tick_v71']and x['repeat']==int(p.name.split('-')[-2]));assert hashlib.sha256(raw).hexdigest()==m['private_logical_sha256']and m['physical_golden_exact']
 physical=p.with_name(p.name.replace('-private.jsonl.gz','-physical.jsonl.gz'));data=[x['sample']for x in map(json.loads,gzip.open(physical,'rt'))if x['event']=='calibration.sample'];assert data==windows['sleep-settle']
assert set(groups)=={47,48,49,100}and all(len(g)==2 and g[0]==g[1]for g in groups.values())
print('Eight immutable Windows traces: zero acceleration force refreshes wake counter while awake; all4800physical states equal retained Windows reference. Getter target derived from vtable; early eax diagnostic ignored.')
