from pathlib import Path
import json,itertools,sys
r=Path.cwd();sys.path.insert(0,str(r/'work/sync-rrr3d/tools/calibration'));import compare
def cases(path):
 current=None
 with compare.open_capture(path) as f:
  for line in f:
   x=json.loads(line)
   if x['event']=='calibration.begin':current={'name':x['detail'],'meta':x['sample'],'rows':[],'descriptions':{}}
   elif x['event']=='calibration.sample':current['rows'].append(x['sample'])
   elif x['event'] in ['calibration.engine-description','calibration.peer-description']:current['descriptions'][x['event']]=x['sample']
   elif x['event']=='calibration.end':yield current;current=None
 assert current is None
def except_length(row):
 row=dict(row);row['wheels']=[{k:v for k,v in x.items() if k!='suspension_length'} for x in row['wheels']];return row
changed=[];states=0;different=0
for a,b in itertools.zip_longest(cases(r/'outputs/spring-force-v115/broad/candidate-run0.jsonl'),cases(r/'outputs/wheel-length-v116/broad/candidate-run0.jsonl')):
 assert a and b and a['name']==b['name'] and a['meta']==b['meta'] and a['descriptions']==b['descriptions'] and len(a['rows'])==len(b['rows'])
 assert all(except_length(x)==except_length(y) for x,y in zip(a['rows'],b['rows'])),(a['name'],'Non-diagnostic observable changed')
 if a['rows']!=b['rows']:changed.append(a['name'])
 different+=sum(x!=y for x,y in zip(a['rows'],b['rows']));states+=len(a['rows'])
assert states==255864
report={'scope':'All608complete V115/V116 trajectories: every field except derived wheel suspension_length is decimal-exact unchanged, including commands, pose, velocity, momentum, contacts, forces, impulses and wheel speed. Fixture metadata and engine descriptions unchanged.','cases':608,'states':states,'all_other_observables_decimal_exact':True,'diagnostic_changed_trajectories':changed,'diagnostic_changed_samples':different,'goal_complete':False}
(r/'work/wheel-length-v116/diagnostic-only-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('All608 trajectories retain every other observable; changed diagnostic cases:',changed)
