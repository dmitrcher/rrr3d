from pathlib import Path
import itertools,json,sys,copy,struct
root=Path.cwd();sys.path.insert(0,str(root/'work/sync-rrr3d/tools/calibration'));import compare
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def cases(path):
 with compare.open_capture(path) as f:
  current=None
  for row in map(json.loads,f):
   if row['event']=='calibration.begin':current={'name':row['detail'],'meta':row['sample'],'rows':[]}
   elif row['event']=='calibration.sample':current['rows'].append(row['sample'])
   elif row['event']=='calibration.end':yield current;current=None
 assert current is None
paths=[root/'outputs/calibration-v14-source-windows.jsonl',root/'outputs/moving-support-v80/broad-query/candidate-run0.jsonl',root/'outputs/longitudinal-slip-v82/broad/candidate-run0.jsonl']
reports=[]
for a,b,c in itertools.zip_longest(*(cases(p)for p in paths)):
 assert a and b and c and a['name']==b['name']==c['name']
 assert len(a['rows'])==len(b['rows'])==len(c['rows'])
 mismatch_old=mismatch_new=changed=0;max_old=max_new=0
 for x,y,z in zip(a['rows'],b['rows'],c['rows']):
  yr=copy.deepcopy(y);zr=copy.deepcopy(z)
  for wx,wy,wz in zip(x['wheels'],yr['wheels'],zr['wheels']):
   old=wy.pop('engine_longitudinal_slip');new=wz.pop('engine_longitudinal_slip');ref=wx['engine_longitudinal_slip']
   mismatch_old+=bits(old)!=bits(ref);mismatch_new+=bits(new)!=bits(ref);changed+=bits(old)!=bits(new)
   max_old=max(max_old,abs(old-ref));max_new=max(max_new,abs(new-ref))
  assert yr==zr,(a['name'],x['tick'],'A quantity other than longitudinal slip changed')
 reports.append({'name':a['name'],'samples':len(a['rows']),'changed_slip_words':changed,'old_mismatch_words':mismatch_old,'new_mismatch_words':mismatch_new,'old_max_error':max_old,'new_max_error':max_new})
assert len(reports)==608
r={'scope':'Every recorded quantity other than engine_longitudinal_slip is unchanged versus V80 in all608 cases; exact raw float32 slip words compared to immutable Windows reference. No tolerance.','all_other_observables_unchanged':True,'cases':reports,'old_mismatch_words':sum(x['old_mismatch_words']for x in reports),'new_mismatch_words':sum(x['new_mismatch_words']for x in reports),'changed_cases':[x['name']for x in reports if x['changed_slip_words']],'maxima_improved':[x['name']for x in reports if x['new_max_error']<x['old_max_error']],'maxima_worsened':[x['name']for x in reports if x['new_max_error']>x['old_max_error']],'global_slip_maxima_no_regression':not any(x['new_max_error']>x['old_max_error']for x in reports),'scope_limit':'Source formula proven on identical Native inputs; existing upstream differences can worsen reported slip maxima. All other observables are unchanged.','goal_complete':False}
(root/'outputs/physics-v82-slip-only-change-audit.json').write_text(json.dumps(r,indent=2)+'\n')
print('Slip mismatches',r['old_mismatch_words'],'->',r['new_mismatch_words'],'changed cases',len(r['changed_cases']),'worsened slip maxima',len(r['maxima_worsened']))
assert r['all_other_observables_unchanged']
# A correct Source formula does not repair divergent inputs. Report all slip regressions explicitly.
print('Source formula verified independently; full-trajectory slip parity remains open.')
