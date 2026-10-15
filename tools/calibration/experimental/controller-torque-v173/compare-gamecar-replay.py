from pathlib import Path
import json,gzip,struct,hashlib
root=Path.cwd();p=root/'outputs/controller-torque-v173/gamecar-event-replay'
def rows(path,event=None):
 a=list(map(json.loads,path.open()));return [r['sample'] for r in a if r['event']==event] if event else [(r['event'],r['sample']) for r in a if r['event'] in ['solo.drive','solo.sample']]
a=rows(p/'track0-off.jsonl');b=rows(p/'track0-on0.jsonl');c=rows(p/'track0-on1.jsonl');assert b==c
assert a==rows(root/'outputs/controller-wake-v171/gamecar-event-replay/track0-on0.jsonl')
for mode in ['off','on0','on1']:assert (p/f'track0-{mode}.words').read_bytes()==(p/'track0-on0.words').read_bytes()
for event in ['solo.drive','solo.sample']:
 x=rows(p/'track0-off.jsonl',event);y=rows(p/'track0-on0.jsonl',event);assert len(x)==len(y)==900
 if event=='solo.sample':assert [(r['input'],r['delta_velocity']) for r in x]==[(r['input'],r['delta_velocity']) for r in y]
for mode in ['off','on0','on1']:
 d=rows(p/f'track0-{mode}.jsonl','solo.drive');assert [r['tick'] for r in d if r['wake_event']]==[120,360,420,480,600]
# Check a measured full-input command before any gear transition, against the original DLL.
native=[r for r in map(json.loads,gzip.open(root/'outputs/controller-torque-v173/native/run0.jsonl.gz','rt')) if r['event']=='motor.sample' and r['precision']==24]
word=lambda f:struct.unpack('<I',struct.pack('<f',f))[0]
n=next(r for r in native if r['maximum_torque_word']==word(1000.) and r['gear']==1)
x=rows(p/'track0-off.jsonl','solo.drive')[120];y=rows(p/'track0-on0.jsonl','solo.drive')[120]
assert x['gear']==y['gear']==1 and y['rpm']==0 and word(y['motor_torque'])==n['result_word'] and word(x['motor_torque'])!=n['result_word']
first={event:next((i for i,(x,y) in enumerate(zip(rows(p/'track0-off.jsonl',event),rows(p/'track0-on0.jsonl',event))) if x!=y),None) for event in ['solo.drive','solo.sample']}
report=dict(scope='Actual Mac importer/Player/GameCar/session bridge. Native DLL verifies full-throttle gear1 torque at step120 for the imported base1000 motor. Two corrected900-step runs identical; no full Windows trajectory comparison claimed.',first_change=first,step120=dict(native_word=n['result_word'],off_word=word(x['motor_torque']),on_word=word(y['motor_torque'])),default_off_identical_to_v171=True,corrected_runs_identical=True,scene_inputs_impulses_identical=True,wake_events_preserved=True,goal_complete=False)
(p/'comparison.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
