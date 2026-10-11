import argparse,hashlib,json,pathlib,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--probe',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]));import verify_isolation
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
def comparable(case):
 c=dict(case);c['meta']=dict(case['meta']);profile=c['meta'].pop('profile');assert '\n' in profile;c['profile_contents']=profile.split('\n',1)[1];return c
windows=compare.load(root/'reference-long-contact-run0-v25-windows.jsonl','windows-physx-d3d9')
baseline=compare.load(root/'calibration-plane-ordinary-run0-v38-mac.jsonl','mac-jolt-metal')
audit={'scope':'Independent experimental Mac polygon depth; production remains default-off; complete goal not achieved','probe_sha256':sha(a.probe),'datasets':{},'isolation':{}}
for folder in ['long','step0','step1','step2']:
 d=root/folder;manifest=json.loads((d/'capture-manifest.json').read_text());assert manifest['status']=='measured' and manifest['probe_sha256']==audit['probe_sha256'];names=manifest['cases'];captures={}
 for mode in ['ordinary','control']:
  runs=[]
  diagnostics=[]
  for repeat in range(2):
   path=d/f'calibration-depth-{mode}-run{repeat}-v39-mac.jsonl';assert sha(path)==manifest['captures'][mode+str(repeat)]['sha256'];cases,diagnostic=verify_isolation.capture(path,'mac-jolt-metal','frame-v25-long-');assert set(cases)==set(names);runs.append(cases);diagnostics.append(diagnostic)
   if mode=='control':assert all(comparable(cases[n])==comparable(baseline[n]) for n in names)
   stats={n:{'first_difference':next((x['tick'] for x,y in zip(windows[n]['rows'],cases[n]['rows']) if x!=y),None),'max_position':max(compare.norm(x['position'],y['position']) for x,y in zip(windows[n]['rows'],cases[n]['rows'])),'max_velocity':max(compare.norm(x['velocity'],y['velocity']) for x,y in zip(windows[n]['rows'],cases[n]['rows']))} for n in names};assert stats==manifest['captures'][mode+str(repeat)]['cases'];captures[mode+str(repeat)]=stats
  assert runs[0]==runs[1]
  assert diagnostics[0]['semantic_sha256']==diagnostics[1]['semantic_sha256'] and diagnostics[0]['contact_semantic_sha256']==diagnostics[1]['contact_semantic_sha256']
 audit['datasets'][folder]={'repeatable':True,'disabled_control_equals_v38':True,'captures':captures}
new=audit['datasets']['long']['captures']['ordinary0'];old=audit['datasets']['long']['captures']['control0'];exact_new={n for n in new if new[n]['first_difference'] is None};exact_old={n for n in old if old[n]['first_difference'] is None};assert len(new)==72 and len(exact_new)==16 and len(exact_old)==6 and exact_old<=exact_new
summary={'exact':len(exact_new),'remaining':72-len(exact_new),'new_exact':sorted(exact_new-exact_old),'lost_exact':sorted(exact_old-exact_new),'position_improved':[n for n in new if new[n]['max_position']<old[n]['max_position']],'position_worsened':[n for n in new if new[n]['max_position']>old[n]['max_position']],'velocity_improved':[n for n in new if new[n]['max_velocity']<old[n]['max_velocity']],'velocity_worsened':[n for n in new if new[n]['max_velocity']>old[n]['max_velocity']],'max_position':max(c['max_position'] for c in new.values()),'max_velocity':max(c['max_velocity'] for c in new.values())};audit['long_summary']=summary
for stage in range(4):
 s=f'{stage:02}';report=verify_isolation.verify(root/f'reference-isolation-stage{s}-run0-v22-windows.jsonl',root/f'reference-isolation-stage{s}-run1-v22-windows.jsonl',root/f'isolation/isolation-stage{s}-run0-mac.jsonl',root/f'isolation/isolation-stage{s}-run1-mac.jsonl');assert report['exact_parity']['complete'] and report['repeatable'];audit['isolation'][s]=report
assert sum(r['exact_parity']['cases_checked'] for r in audit['isolation'].values())==189
isolation_manifest=json.loads((root/'isolation/isolation-run.json').read_text());assert isolation_manifest['contact_v39_depth'] and isolation_manifest['probe_sha256']==audit['probe_sha256'] and isolation_manifest['status']=='implemented_stages_passed'
for s,report in audit['isolation'].items():assert [c['sha256'] for c in report['captures']]==isolation_manifest['stages'][s]['capture_sha256']
native_traces=list(root.glob('reference-native-point-*-physics-run*-v37-windows.jsonl'));assert len(native_traces)==8
for path in native_traces:
 native=compare.load(path,'windows-physx-d3d9');assert len(native)==1 and all(c==windows[n] for n,c in native.items()),'External tracing changes original Windows physics'
audit['original_windows_traces_unchanged']=len(native_traces)
for step in [0,1,2]:
 result=json.loads((root/f'step{step}/solver-comparison.json').read_text());assert result['repeats']['0']==result['repeats']['1'];cases=result['repeats']['0'];assert len(cases)==4
 if step<2:assert all(not c['differences'] for c in cases.values()) and sum(c['rows'] for c in cases.values())==120
 else:assert all(not c['differences'] for n,c in cases.items() if n!='frame-v25-long-p7-x1-y0') and cases['frame-v25-long-p7-x1-y0']['differences'][0]['field']=='scaled_bias'
 audit[f'solver_step{step}']=cases
(root/'physics-v39-depth-audit.json').write_text(json.dumps(audit,indent=2)+'\n');print('189 isolation scenarios exact; 16/72 long scenarios exact, 56 remaining; two repeats and v38 disabled controls checked.');print('Position improved/worsened:',len(summary['position_improved']),len(summary['position_worsened']));print('Velocity improved/worsened:',len(summary['velocity_improved']),len(summary['velocity_worsened']))
