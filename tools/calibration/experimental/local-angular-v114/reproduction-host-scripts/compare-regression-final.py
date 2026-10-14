from pathlib import Path
import json,itertools,sys
root=Path.cwd();sys.path.insert(0,str(root/'work/sync-rrr3d/tools/calibration'));import compare
def cases(path):
 current=None
 with compare.open_capture(path) as stream:
  for line in stream:
   x=json.loads(line)
   if x['event']=='calibration.begin':current={'name':x['detail'],'meta':x['sample'],'rows':[],'descriptions':{}}
   elif x['event']=='calibration.sample':current['rows'].append(x['sample'])
   elif x['event'] in ['calibration.engine-description','calibration.peer-description']:current['descriptions'][x['event']]=x['sample']
   elif x['event']=='calibration.end':yield current;current=None
 assert current is None
changed=[];same=[]
for a,b in itertools.zip_longest(cases(root/'outputs/game-angular-v113/broad-final/candidate-run0.jsonl'),cases(root/'outputs/air-torque-v114/broad-final/candidate-run0.jsonl')):
 assert a and b and a['name']==b['name'];assert a['meta']==b['meta'] and a['descriptions']==b['descriptions']
 if a['rows']==b['rows']:same.append(a['name'])
 else:
  changed.append(a['name'])

assert len(same)+len(changed)==608
metrics=json.load(open(root/'outputs/physics-v114-validated-practical-error-audit.json'))['cases'];fields=['position_max','velocity_max','orientation_max_degrees','angular_momentum_max','wheel_contact_mismatches','wheel_suspension_max','wheel_normal_force_max'];summary={}
for field in fields:
 dif=[(r['name'],r['previous_v113'][field],r['current'][field]) for r in metrics if r['previous_v113'][field]!=r['current'][field]]
 summary[field]={'improved':[n for n,a,b in dif if b<a],'worsened':[n for n,a,b in dif if b>a],'same':608-len(dif)}

report={'scope':'Complete V113/V114 observable trajectories and unchanged engine/fixture metadata. Full trajectory magnitudes compared to the unchanged original Windows reference, not only end frames.','unchanged':same,'changed_trajectories':changed,'metadata_and_descriptions_preserved':True,'maxima':summary,'goal_complete':False}
(root/'outputs/physics-v114-validated-regression-audit.json').write_text(json.dumps(report,indent=2)+'\n');print('unchanged',len(same),'changed',len(changed),'worsened maxima',{f:v['worsened'] for f,v in summary.items()})
