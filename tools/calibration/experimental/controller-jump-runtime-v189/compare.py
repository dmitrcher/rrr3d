"""Provisional compatibility comparison; not native Windows acceptance."""
from pathlib import Path
import collections,gzip,hashlib,json
R=Path.cwd();O=R/'outputs/controller-jump-runtime-v189';fixture=json.loads((O/'cases.json').read_text());groups={x['case']:x['group'] for x in fixture['cases']}
def load(name):
 with gzip.open(O/name,'rt') as f:return [json.loads(s) for s in f if 'jump.sample' in s]
def canonical(x):
 if isinstance(x,list):return [canonical(y) for y in x]
 return 0 if type(x) is int and x==0x80000000 else x
keys=['spring_after_word','position_after_step','velocity_after_step','rotation_after_step','momentum_after_step','awake_after_step'];inputs=['velocity_before','rotation_before'];reports=[];previous=None
for rep in range(2):
 a=load(f'compatibility-run{rep}.jsonl.gz');b=load(f'source-run{rep}.jsonl.gz');assert len(a)==len(b)==fixture['count']
 incoming=[];bad=[];zero_only=collections.Counter();bygroup=collections.defaultdict(lambda:dict(cases=0,exact=0))
 for x,y in zip(a,b):
  assert x['case']==y['case'];c=x['case'];inc=[k for k in inputs if canonical(x[k])!=canonical(y[k])];fields=[k for k in keys if canonical(x[k])!=canonical(y[k])]
  if inc:incoming.append(dict(case=c,fields=inc))
  if fields:bad.append(dict(case=c,group=groups[c],fields=fields,compatibility={k:x[k] for k in fields},source={k:y[k] for k in fields},source_horizontal_speed_word=y['horizontal_speed_word'],source_pitch_word=y['applied_pitch_word']))
  zero_only.update(k for k in keys if x[k]!=y[k] and canonical(x[k])==canonical(y[k]));g=bygroup[groups[c]];g['cases']+=1;g['exact']+=not fields
 identity=([{k:canonical(x[k]) for k in keys} for x in a],[{k:canonical(y[k]) for k in keys} for y in b]);assert previous is None or previous==identity;previous=identity
 reports.append(dict(repeat=rep,cases=len(a),exact=len(a)-len(bad),initial_differences=incoming,first_differences=bad[:12],all_different_case_ids=[x['case'] for x in bad],field_counts=dict(collections.Counter(k for x in bad for k in x['fields'])),signed_zero_only_fields=dict(zero_only),groups=dict(bygroup)));print('repeat',rep,reports[-1]['exact'],'/',len(a),'incoming',len(incoming),dict(bygroup),flush=True)
report=dict(scope='Actual original JumpProgress+SDK under CrossOver versus actual Source GameCar+Jolt. Runtime qualification for arbitrary GameCar arithmetic is NOT established; differences are investigation leads, not automatic Windows parity failures. Synthetic scalar contact flags, identity initial orientation, four free wheels, no AI. One method delta then fixed integration1/60.',comparison='Exact uint32 words; existing physics contract treats signed zeros equal and reports them separately. No float conversion of integer words.',runs=reports,repeats_identical=True,vm_started=False,native_windows_gold=False,full_goal_complete=False)
(O/'comparison.json').write_text(json.dumps(report,indent=2)+'\n')
