import argparse,json,pathlib,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]));from verify_isolation import verify,capture
audit={'goal_complete':False,'experimental_only':True,'isolation':{},'factorial':{}}
manifest=json.loads((root/'isolation-run-v38.json').read_text());assert manifest['contact_v38_plane_normal']
for stage in ('00','01','02','03'):
 result=verify(*[root/f'reference-isolation-stage{stage}-run{r}-v22-windows.jsonl' for r in range(2)],*[root/f'isolation-stage{stage}-run{r}-mac.jsonl' for r in range(2)]);assert result['exact_parity']['complete'];assert [c['sha256'] for c in result['captures']]==manifest['stages'][stage]['capture_sha256'];audit['isolation'][stage]={'cases':result['captures'][0]['cases'],'repeatable':True,'complete':True}
base=compare.load(root/'calibration-inertia-ordinary-run0-v37-mac.jsonl','mac-jolt-metal');native=compare.load(root/'reference-long-contact-run0-v25-windows.jsonl','windows-physx-d3d9')
for candidate in ('plane-only','normal-only','combined'):
 reports={};states={}
 for mode in ('ordinary','control'):
  paths=[root/f'calibration-{candidate}-{mode}-run{r}-v38-mac.jsonl' for r in range(2)];measured=[capture(path,'mac-jolt-metal','frame-v25-long-') for path in paths]
  assert measured[0][1]['semantic_sha256']==measured[1][1]['semantic_sha256'];assert measured[0][1]['contact_semantic_sha256']==measured[1][1]['contact_semantic_sha256'];assert set(measured[0][0])==set(native) and len(native)==72
  if mode=='control':assert {n:c['rows'] for n,c in measured[0][0].items()}=={n:c['rows'] for n,c in base.items()},'Disabled changes prior physical states'
  states[mode]=measured[0][0];reports[mode]=compare.compare(root/'reference-long-contact-run0-v25-windows.jsonl',paths[0])
 result={'cases':72,'steps_per_case':120,'repeatable':True,'control_unchanged_vs_v37':True,'remaining':len(reports['ordinary']['exact_parity']['cases_remaining']),'max_position':max(c['maximum']['position_error'] for c in reports['ordinary']['cases'].values()),'max_velocity':max(c['maximum']['velocity_error'] for c in reports['ordinary']['cases'].values())}
 for metric in ('position_error','velocity_error'):
  for direction,comparison in [('increased',lambda x,y:x>y),('decreased',lambda x,y:x<y)]:result[metric+'_'+direction]=[n for n,c in reports['ordinary']['cases'].items() if comparison(c['maximum'][metric],reports['control']['cases'][n]['maximum'][metric])]
 result['new_exact']=sorted(set(reports['control']['exact_parity']['cases_remaining'])-set(reports['ordinary']['exact_parity']['cases_remaining']));result['lost_exact']=sorted(set(reports['ordinary']['exact_parity']['cases_remaining'])-set(reports['control']['exact_parity']['cases_remaining']));assert result['remaining']==66 and not result['new_exact'] and not result['lost_exact'];audit['factorial'][candidate]=result
first=compare.load(root/'calibration-plane-first-ordinary-run0-v38-mac.jsonl','mac-jolt-metal');assert len(first)==4
assert all(c['rows'][0]==native[n]['rows'][0] for n,c in first.items()),'First physical snapshot differs'
for kind in ('vertex','normal','point'):
 version='37' if kind=='point' else '38'
 for path in root.glob(f'reference-native-{kind}-*-physics-run*-v{version}-windows.jsonl'):
  got=compare.load(path,'windows-physx-d3d9');assert len(got)==1
  assert all(c==native[n] for n,c in got.items()),'External tracer changes original physical states'
(root/'physics-v38-comparison-reverified.json').write_text(json.dumps(audit,indent=2)+'\n')
for candidate,r in audit['factorial'].items():print(candidate,'remaining',r['remaining'],'pos increases/decreases',len(r['position_error_increased']),len(r['position_error_decreased']),'vel increases/decreases',len(r['velocity_error_increased']),len(r['velocity_error_decreased']))
