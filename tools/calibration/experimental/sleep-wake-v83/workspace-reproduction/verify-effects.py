from pathlib import Path
import gzip,json,math
root=Path.cwd();folder=root/'outputs/sleep-wake-v83/focused'
def load(path):
 result={}
 for x in map(json.loads,path.open()):
  if x['event']=='calibration.sample':result.setdefault(x['detail'],[]).append(x['sample'])
 return result
runs={m:[load(folder/f'{m}-{i}.jsonl')for i in range(2)]for m in ['control','wake','dormant','combined']}
assert all(v[0]==v[1]for v in runs.values())
for name in ['sleep-settle','settle']:
 assert runs['wake'][0][name]==runs['combined'][0][name]
 if name=='settle':assert all(v[0][name]==runs['control'][0][name]for v in runs.values())
assert runs['wake'][0]['sleep-settle']==runs['control'][0]['settle']
for mode in runs:
 a=list(map(json.loads,(folder/f'{mode}-0-sleep.jsonl').open()));b=list(map(json.loads,(folder/f'{mode}-1-sleep.jsonl').open()));assert a==b
 asleep=[x for x in a if x['allow_sleep']and not x['active']]
 if mode in ['wake','combined']:assert not asleep and all(x['allow_sleep']for x in a if x['allow_sleep'])
 if mode=='dormant':assert asleep and all(x['velocity']==[0,0,0]and x['angular']==[0,0,0]for x in asleep)
report=json.load(open(folder/'report.json'));old=report['cases']['control']['sleep-settle'];now=report['cases']['combined']['sleep-settle'];assert now['position_max']<old['position_max']and now['velocity_max']<old['velocity_max']
r={'scope':'Factorial isolation: control/wake-only/dormant-restore-only/combined, two identical1200state runs each. Wake-only exactly restores the no-sleep-control trajectory. Dormant-only removes stale motion but leaves premature sleep, and by itself slightly worsens angular-momentum error; it is not a parity fix alone. Combined matches wake-only exactly. Sleeping remains enabled. No timer or threshold fitted.','control':old,'combined':now,'wake_only_matches_control_settle_complete_history':True,'combined_matches_wake_only':True,'dormant_only_clears_inactive_motion':True,'settle_all_modes_preserved':True,'all_modes_two_repeats_exact':True,'goal_complete':False};(root/'outputs/physics-v83-factorial-causal-audit.json').write_text(json.dumps(r,indent=2)+'\n');print('Isolated mechanisms proven; position',old['position_max'],'->',now['position_max'],'velocity',old['velocity_max'],'->',now['velocity_max'])
