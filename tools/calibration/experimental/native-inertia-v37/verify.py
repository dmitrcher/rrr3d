import argparse,hashlib,json,pathlib,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]));from verify_isolation import verify,capture
audit={'goal_complete':False,'experimental_only':True,'isolation':{},'tilted':{},'initial_anchor_hypothesis_correction':'The preliminary v36 local-anchor discrepancy used 9-digit JSON doubles without casting operands to float32. Corrected first p7 eight-point transform matches Windows; that preliminary discrepancy is not evidence of a physics defect.'}
manifest=json.loads((root/'isolation-run-v37.json').read_text());assert manifest['contact_v37_inertia'] and manifest['environment']['RRR3D_EXPERIMENT_NATIVE_CONTACT_INERTIA']=='1'
for stage in ('00','01','02','03'):
 refs=[root/f'reference-isolation-stage{stage}-run{r}-v22-windows.jsonl' for r in range(2)];mac=[root/f'isolation-stage{stage}-run{r}-mac.jsonl' for r in range(2)]
 result=verify(*refs,*mac);assert result['exact_parity']['complete'];assert [c['sha256'] for c in result['captures']]==manifest['stages'][stage]['capture_sha256']
 audit['isolation'][stage]={'cases':result['captures'][0]['cases'],'repeatable':True,'complete':True}
base=compare.load(root/'calibration-frame-ordinary-run0-v36-mac.jsonl','mac-jolt-metal');native=compare.load(root/'reference-long-contact-run0-v25-windows.jsonl','windows-physx-d3d9')
reports={};allcases={}
for mode in ('ordinary','control'):
 paths=[root/f'calibration-inertia-{mode}-run{r}-v37-mac.jsonl' for r in range(2)];measured=[capture(path,'mac-jolt-metal','frame-v25-long-') for path in paths]
 assert measured[0][1]['semantic_sha256']==measured[1][1]['semantic_sha256'];assert measured[0][1]['contact_semantic_sha256']==measured[1][1]['contact_semantic_sha256']
 assert set(measured[0][0])==set(native) and len(native)==72
 # Compare only physical snapshots here; capture-mac.py additionally checks all control metadata/profile contents.
 if mode=='control':assert {n:c['rows'] for n,c in measured[0][0].items()}=={n:base[n]['rows'] for n in base},'Disabled v37 changes saved v36 physical states'
 allcases[mode]=measured[0][0];reports[mode]=compare.compare(root/'reference-long-contact-run0-v25-windows.jsonl',paths[0])
 audit['tilted'][mode]={'cases':72,'repeatable':True,'remaining':len(reports[mode]['exact_parity']['cases_remaining']),'max_position':max(c['maximum']['position_error'] for c in reports[mode]['cases'].values()),'max_velocity':max(c['maximum']['velocity_error'] for c in reports[mode]['cases'].values())}
audit['tilted']['changed_cases_vs_v36']=[n for n in allcases['ordinary'] if allcases['ordinary'][n]['rows']!=allcases['control'][n]['rows']];audit['tilted']['control_states_unchanged_vs_v36']=True
for metric in ('position_error','velocity_error'):
 audit['tilted'][metric+'_increased']=[n for n,c in reports['ordinary']['cases'].items() if c['maximum'][metric]>reports['control']['cases'][n]['maximum'][metric]]
 audit['tilted'][metric+'_decreased']=[n for n,c in reports['ordinary']['cases'].items() if c['maximum'][metric]<reports['control']['cases'][n]['maximum'][metric]]
assert all(x['remaining']==66 for x in audit['tilted'].values() if isinstance(x,dict));audit['tilted']['new_exact']=sorted(set(reports['control']['exact_parity']['cases_remaining'])-set(reports['ordinary']['exact_parity']['cases_remaining']));audit['tilted']['lost_exact']=sorted(set(reports['ordinary']['exact_parity']['cases_remaining'])-set(reports['control']['exact_parity']['cases_remaining']));assert not audit['tilted']['new_exact'] and not audit['tilted']['lost_exact']
for name in ('frame-v25-long-p7-x1-y0','frame-v25-long-p5-x0-y0','frame-v25-long-p5-x1-y1','frame-v25-long-p4-x1-y0'):
 for repeat in range(2):
  assert compare.load(root/f'reference-native-point-{name}-physics-run{repeat}-v37-windows.jsonl','windows-physx-d3d9')[name]==native[name],'Native matrix trace changes physical state'
(root/'physics-v37-comparison-reverified.json').write_text(json.dumps(audit,indent=2)+'\n');print(json.dumps(audit,indent=2))
