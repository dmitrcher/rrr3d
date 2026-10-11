import argparse,hashlib,json,pathlib,struct,sys,subprocess
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--probe',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]));import verify_isolation
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
f32=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
def rounded(v):return [f32(x) for x in v]
def comparable(c):
 c=dict(c);c['meta']=dict(c['meta']);profile=c['meta'].pop('profile');assert '\n' in profile;c['profile_contents']=profile.split('\n',1)[1];return c
windows=compare.load(root/'reference-long-contact-run0-v25-windows.jsonl','windows-physx-d3d9');baseline=compare.load(root/'v44-long-baseline.jsonl','mac-jolt-metal')
audit={'scope':'Original upper local-Z polygon traversal; old long contact suite closed, one new offset-inverted residual and broader physics remain','probe_sha256':sha(a.probe),'datasets':{},'isolation':{}}
for folder in ['long','step0','step1','step2','p11-stages']:
 d=root/folder;manifest=json.loads((d/'capture-manifest.json').read_text());assert manifest['probe_sha256']==audit['probe_sha256'];modes={}
 for mode in ['ordinary','control']:
  runs=[];diags=[]
  for repeat in range(2):
   path=d/f'calibration-depth-{mode}-run{repeat}-v39-mac.jsonl';assert sha(path)==manifest['captures'][mode+str(repeat)]['sha256'];cases,diag=verify_isolation.capture(path,'mac-jolt-metal','frame-v25-long-');assert set(cases)==set(manifest['cases']);runs.append(cases);diags.append(diag)
   if mode=='control':assert all(comparable(c)==comparable(baseline[n]) for n,c in cases.items())
   stats={n:{'first_difference':next((x['tick'] for x,y in zip(windows[n]['rows'],c['rows']) if x!=y),None),'max_position':max(compare.norm(x['position'],y['position']) for x,y in zip(windows[n]['rows'],c['rows'])),'max_velocity':max(compare.norm(x['velocity'],y['velocity']) for x,y in zip(windows[n]['rows'],c['rows']))} for n,c in cases.items()};assert stats==manifest['captures'][mode+str(repeat)]['cases'];modes[mode+str(repeat)]=stats
  assert runs[0]==runs[1] and diags[0]['semantic_sha256']==diags[1]['semantic_sha256'] and diags[0]['contact_semantic_sha256']==diags[1]['contact_semantic_sha256']
 audit['datasets'][folder]={'disabled_control_equals_v44':True,'repeatable':True,'captures':modes}
new=audit['datasets']['long']['captures']['ordinary0'];old=audit['datasets']['long']['captures']['control0'];newexact={n for n in new if new[n]['first_difference'] is None};oldexact={n for n in old if old[n]['first_difference'] is None};assert len(new)==72 and len(newexact)==72 and len(oldexact)==67 and oldexact<=newexact
for field in ['max_position','max_velocity']:assert all(new[n][field]<=old[n][field] for n in new)
audit['long_summary']={'exact':72,'remaining':0,'new_exact':sorted(newexact-oldexact),'lost_exact':[],'residuals':{n:new[n] for n in sorted(set(new)-newexact)},'position_improved':sum(new[n]['max_position']<old[n]['max_position'] for n in new),'velocity_improved':sum(new[n]['max_velocity']<old[n]['max_velocity'] for n in new),'position_or_velocity_worsened':0}
for step in range(3):
 subprocess.run([sys.executable,str(pathlib.Path(__file__).with_name('compare-solver.py')),'--measurements',str(root/f'step{step}'),'--reference-dir',str(root),'--step',str(step)],check=True)
 s=json.loads((root/f'step{step}/solver-comparison.json').read_text())['repeats'];assert s['0']==s['1'] and len(s['0'])==6 and all(not c['differences'] for c in s['0'].values());audit[f'solver_step{step}']=s['0']
subprocess.run([sys.executable,str(pathlib.Path(__file__).with_name('compare-solver.py')),'--measurements',str(root/'p11-stages'),'--reference-dir',str(root),'--step','0'],check=True)
s=json.loads((root/'p11-stages/solver-comparison.json').read_text())['repeats'];assert s['0']==s['1'];assert sum(c['rows'] for c in s['0'].values())==40 and all(not c['differences'] for c in s['0'].values());audit['new_p11_solver_step0']=s['0']
for stage in range(4):
 s=f'{stage:02}';result=verify_isolation.verify(root/f'reference-isolation-stage{s}-run0-v22-windows.jsonl',root/f'reference-isolation-stage{s}-run1-v22-windows.jsonl',root/f'isolation/isolation-stage{s}-run0-mac.jsonl',root/f'isolation/isolation-stage{s}-run1-mac.jsonl');assert result['repeatable'] and result['exact_parity']['complete'];audit['isolation'][s]=result
assert sum(r['exact_parity']['cases_checked'] for r in audit['isolation'].values())==189
isom=json.loads((root/'isolation/isolation-run.json').read_text());assert isom['probe_sha256']==audit['probe_sha256'] and isom['contact_v45_positive_z_face_order'] and isom['status']=='implemented_stages_passed'
for stage in range(4):assert [c['sha256'] for c in audit['isolation'][f'{stage:02}']['captures']]==isom['stages'][f'{stage:02}']['capture_sha256']
expanded_native=compare.load(root/'expanded-native/expanded-pose-run0-windows.jsonl','windows-physx-d3d9');expanded_old=compare.load(root/'expanded-before/expanded-pose-ordinary-run0-mac.jsonl','mac-jolt-metal');assert len(expanded_native)==6
expanded_manifest=json.loads((root/'expanded/capture-manifest.json').read_text());assert expanded_manifest['probe_sha256']==audit['probe_sha256']
for mode in ['ordinary','control']:
 runs=[]
 for repeat in range(2):
  path=root/f'expanded/expanded-pose-{mode}-run{repeat}-mac.jsonl';assert sha(path)==expanded_manifest['captures'][mode+str(repeat)]['sha256'];cases,diag=verify_isolation.capture(path,'mac-jolt-metal','pose-v42-frozen-');runs.append((cases,diag));assert set(cases)==set(expanded_native)
  assert all(cases[n]['rows']==expanded_native[n]['rows'] for n in cases)
  if mode=='control':assert all(comparable(cases[n])==comparable(expanded_old[n]) for n in cases)
 assert runs[0][0]==runs[1][0] and runs[0][1]['semantic_sha256']==runs[1][1]['semantic_sha256'] and runs[0][1]['contact_semantic_sha256']==runs[1][1]['contact_semantic_sha256']
audit['expanded']={'exact':6,'disabled_control_equals_v44':True,'repeatable':True}
subprocess.run([sys.executable,str(pathlib.Path(__file__).with_name('compare-stages.py')),'--mac',str(root/'p11-stages'),'--native',str(root),'--out',str(root/'physics-v45-edge-stage-audit.json')],check=True)
audit['edge_stages']=json.loads((root/'physics-v45-edge-stage-audit.json').read_text())
native_manifest=json.loads((root/'inverted-native/capture-manifest.json').read_text());native_runs=[]
for repeat in range(2):
 path=root/f'inverted-native/original-long72-run{repeat}-windows.jsonl';assert sha(path)==native_manifest['runs']['original'+str(repeat)]['sha256'];assert compare.load(path,'windows-physx-d3d9')==windows
 path=root/f'inverted-native/expanded-pose-run{repeat}-windows.jsonl';assert sha(path)==native_manifest['runs']['expanded'+str(repeat)]['sha256'];cases,diag=verify_isolation.capture(path,'windows-physx-d3d9','pose-v45-inverted-');native_runs.append((cases,diag))
assert native_runs[0][0]==native_runs[1][0] and native_runs[0][1]['semantic_sha256']==native_runs[1][1]['semantic_sha256'] and native_runs[0][1]['contact_semantic_sha256']==native_runs[1][1]['contact_semantic_sha256']
inverted_native=native_runs[0][0];manifest=json.loads((root/'inverted/capture-manifest.json').read_text());assert manifest['probe_sha256']==audit['probe_sha256'];inverted={}
for mode in ['ordinary','control']:
 runs=[];stats_runs=[]
 for repeat in range(2):
  path=root/f'inverted/expanded-pose-{mode}-run{repeat}-mac.jsonl';assert sha(path)==manifest['captures'][mode+str(repeat)]['sha256'];cases,diag=verify_isolation.capture(path,'mac-jolt-metal','pose-v45-inverted-');runs.append((cases,diag));assert set(cases)==set(inverted_native)
  stats={n:{'first_difference':next((x['tick'] for x,y in zip(inverted_native[n]['rows'],c['rows']) if x!=y),None),'max_position':max(compare.norm(x['position'],y['position']) for x,y in zip(inverted_native[n]['rows'],c['rows'])),'max_velocity':max(compare.norm(x['velocity'],y['velocity']) for x,y in zip(inverted_native[n]['rows'],c['rows']))} for n,c in cases.items()};assert stats==manifest['captures'][mode+str(repeat)]['cases'];stats_runs.append(stats)
 assert runs[0][0]==runs[1][0] and runs[0][1]['semantic_sha256']==runs[1][1]['semantic_sha256'] and runs[0][1]['contact_semantic_sha256']==runs[1][1]['contact_semantic_sha256'];assert stats_runs[0]==stats_runs[1];inverted[mode]=stats_runs[0]
assert sum(s['first_difference'] is None for s in inverted['ordinary'].values())==5 and all(s['first_difference'] is not None for s in inverted['control'].values())
assert inverted['ordinary']['pose-v45-inverted-5']['first_difference']==50
assert all(inverted['ordinary'][n]['max_position']<=inverted['control'][n]['max_position'] and inverted['ordinary'][n]['max_velocity']<=inverted['control'][n]['max_velocity'] for n in inverted_native)
audit['inverted']={'exact':5,'remaining':1,'captures':inverted,'repeatable':True,'original_windows72_states_unchanged':True}
(root/'physics-v45-positive-z-order-audit.json').write_text(json.dumps(audit,indent=2)+'\n');print('72/72 original tilted contacts exact, 5 new inverted offset contacts exact and 1 residual. 189 isolated and 6 old expanded cases remain exact, two repeats. 4 edge crossings / 15 fields, 40 new and 475 old solver rows exact.')
