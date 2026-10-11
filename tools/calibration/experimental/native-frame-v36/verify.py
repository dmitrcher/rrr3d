"""Revalidate v36 isolation, full tilted runs, repeats, and disabled control."""
import argparse,hashlib,json,pathlib,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]));from verify_isolation import verify,capture
audit={'goal_complete':False,'experimental_only':True,'isolation':{},'tilted':{}}
manifest=json.loads((root/'isolation-run-v36.json').read_text())
for stage in ('00','01','02','03'):
 refs=[root/f'reference-isolation-stage{stage}-run{r}-v22-windows.jsonl' for r in range(2)]
 mac=[root/f'isolation-stage{stage}-run{r}-mac.jsonl' for r in range(2)]
 result=verify(*refs,*mac);assert result['exact_parity']['complete']
 assert [c['sha256'] for c in result['captures']]==manifest['stages'][stage]['capture_sha256']
 audit['isolation'][stage]={'cases':result['captures'][0]['cases'],'repeatable':True,'complete':True}
base=compare.load(root/'calibration-contact-long-neon-material-run0-v35-mac.jsonl','mac-jolt-metal')
native=compare.load(root/'reference-long-contact-run0-v25-windows.jsonl','windows-physx-d3d9')
reports={}
for mode in ('ordinary','control'):
 paths=[root/f'calibration-frame-{mode}-run{r}-v36-mac.jsonl' for r in range(2)]
 measured=[capture(path,'mac-jolt-metal','frame-v25-long-') for path in paths]
 assert measured[0][1]['semantic_sha256']==measured[1][1]['semantic_sha256']
 assert measured[0][1]['contact_semantic_sha256']==measured[1][1]['contact_semantic_sha256']
 assert set(measured[0][0])==set(native) and len(native)==72
 if mode=='control':assert measured[0][0]==base,'Disabled experiment changes v35 physical state'
 reports[mode]=compare.compare(root/'reference-long-contact-run0-v25-windows.jsonl',paths[0])
 audit['tilted'][mode]={'cases':72,'repeatable':True,'remaining':len(reports[mode]['exact_parity']['cases_remaining']),'max_position':max(c['maximum']['position_error'] for c in reports[mode]['cases'].values()),'max_velocity':max(c['maximum']['velocity_error'] for c in reports[mode]['cases'].values())}
on=reports['ordinary']['cases'];off=reports['control']['cases']
audit['tilted']['new_exact']=[n for n in on if n not in reports['ordinary']['exact_parity']['cases_remaining'] and n in reports['control']['exact_parity']['cases_remaining']]
audit['tilted']['lost_exact']=[n for n in off if n not in reports['control']['exact_parity']['cases_remaining'] and n in reports['ordinary']['exact_parity']['cases_remaining']]
for metric in ('position_error','velocity_error'):
 audit['tilted'][metric+'_increased']=[n for n in on if on[n]['maximum'][metric]>off[n]['maximum'][metric]]
assert audit['tilted']['ordinary']['remaining']==66 and audit['tilted']['control']['remaining']==70
assert len(audit['tilted']['new_exact'])==4 and not audit['tilted']['lost_exact']
assert all(len(audit['tilted'][f'{m}_error_increased'])==23 for m in ('position','velocity'))
for name in ('frame-v25-long-p7-x1-y0','frame-v25-long-p5-x0-y0','frame-v25-long-p5-x1-y1','frame-v25-long-p4-x1-y0'):
 for repeat in range(2):
  path=root/f'reference-native-tilted-{name}-physics-run{repeat}-v36-windows.jsonl'
  assert compare.load(path,'windows-physx-d3d9')[name]==native[name],'Native trace changes physical state'
(root/'physics-v36-comparison-reverified.json').write_text(json.dumps(audit,indent=2)+'\n')
print(json.dumps(audit,ensure_ascii=False,indent=2))
