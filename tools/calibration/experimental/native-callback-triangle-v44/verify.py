import argparse,hashlib,json,pathlib,struct,sys,subprocess
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--probe',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]));import verify_isolation
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
f32=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
def rounded(v):return [f32(x) for x in v]
def comparable(c):
 c=dict(c);c['meta']=dict(c['meta']);profile=c['meta'].pop('profile');assert '\n' in profile;c['profile_contents']=profile.split('\n',1)[1];return c
windows=compare.load(root/'reference-long-contact-run0-v25-windows.jsonl','windows-physx-d3d9');baseline=compare.load(root/'v43-long-baseline.jsonl','mac-jolt-metal')
audit={'scope':'Separate public triangle normal for experimental material callback; no force tuning or production promotion; full goal incomplete','probe_sha256':sha(a.probe),'datasets':{},'isolation':{}}
for folder in ['long','step0','step1','step2','p7-stages']:
 d=root/folder;manifest=json.loads((d/'capture-manifest.json').read_text());assert manifest['probe_sha256']==audit['probe_sha256'];modes={}
 for mode in ['ordinary','control']:
  runs=[];diags=[]
  for repeat in range(2):
   path=d/f'calibration-depth-{mode}-run{repeat}-v39-mac.jsonl';assert sha(path)==manifest['captures'][mode+str(repeat)]['sha256'];cases,diag=verify_isolation.capture(path,'mac-jolt-metal','frame-v25-long-');assert set(cases)==set(manifest['cases']);runs.append(cases);diags.append(diag)
   if mode=='control':assert all(comparable(c)==comparable(baseline[n]) for n,c in cases.items())
   stats={n:{'first_difference':next((x['tick'] for x,y in zip(windows[n]['rows'],c['rows']) if x!=y),None),'max_position':max(compare.norm(x['position'],y['position']) for x,y in zip(windows[n]['rows'],c['rows'])),'max_velocity':max(compare.norm(x['velocity'],y['velocity']) for x,y in zip(windows[n]['rows'],c['rows']))} for n,c in cases.items()};assert stats==manifest['captures'][mode+str(repeat)]['cases'];modes[mode+str(repeat)]=stats
  assert runs[0]==runs[1] and diags[0]['semantic_sha256']==diags[1]['semantic_sha256'] and diags[0]['contact_semantic_sha256']==diags[1]['contact_semantic_sha256']
 audit['datasets'][folder]={'disabled_control_equals_v43':True,'repeatable':True,'captures':modes}
new=audit['datasets']['long']['captures']['ordinary0'];old=audit['datasets']['long']['captures']['control0'];newexact={n for n in new if new[n]['first_difference'] is None};oldexact={n for n in old if old[n]['first_difference'] is None};assert len(new)==72 and len(newexact)==67 and len(oldexact)==44 and oldexact<=newexact
for field in ['max_position','max_velocity']:assert all(new[n][field]<=old[n][field] for n in new)
audit['long_summary']={'exact':67,'remaining':5,'new_exact':sorted(newexact-oldexact),'lost_exact':[],'residuals':{n:new[n] for n in sorted(set(new)-newexact)},'position_improved':sum(new[n]['max_position']<old[n]['max_position'] for n in new),'velocity_improved':sum(new[n]['max_velocity']<old[n]['max_velocity'] for n in new),'position_or_velocity_worsened':0}
for step in range(3):
 subprocess.run([sys.executable,str(pathlib.Path(__file__).with_name('compare-solver.py')),'--measurements',str(root/f'step{step}'),'--reference-dir',str(root),'--step',str(step)],check=True)
 s=json.loads((root/f'step{step}/solver-comparison.json').read_text())['repeats'];assert s['0']==s['1'] and len(s['0'])==6 and all(not c['differences'] for c in s['0'].values());audit[f'solver_step{step}']=s['0']
subprocess.run([sys.executable,str(pathlib.Path(__file__).with_name('compare-solver.py')),'--measurements',str(root/'p7-stages'),'--reference-dir',str(root),'--step','0'],check=True)
s=json.loads((root/'p7-stages/solver-comparison.json').read_text())['repeats'];assert s['0']==s['1'];assert sum(c['rows'] for c in s['0'].values())==20 and all(not c['differences'] for c in s['0'].values());audit['new_p7_solver_step0']=s['0']
for stage in range(4):
 s=f'{stage:02}';result=verify_isolation.verify(root/f'reference-isolation-stage{s}-run0-v22-windows.jsonl',root/f'reference-isolation-stage{s}-run1-v22-windows.jsonl',root/f'isolation/isolation-stage{s}-run0-mac.jsonl',root/f'isolation/isolation-stage{s}-run1-mac.jsonl');assert result['repeatable'] and result['exact_parity']['complete'];audit['isolation'][s]=result
assert sum(r['exact_parity']['cases_checked'] for r in audit['isolation'].values())==189
isom=json.loads((root/'isolation/isolation-run.json').read_text());assert isom['probe_sha256']==audit['probe_sha256'] and isom['contact_v44_callback_triangle_normal'] and isom['status']=='implemented_stages_passed'
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
audit['expanded']={'exact':6,'disabled_control_equals_v43':True,'repeatable':True}
p7='frame-v25-long-p7-x2-y1';stage_checks=[]
for repeat in range(2):
 native=root/f'reference-native-point-{p7}-physics-run{repeat}-v37-windows.jsonl';assert compare.load(native,'windows-physx-d3d9')[p7]==windows[p7]
 native_events={}
 for line in native.open():
  row=json.loads(line)
  if row.get('event') in ['calibration.body-contact','calibration.contact-quaternion'] and row['sample']['tick']==0:native_events.setdefault(row['event'],[]).append(row['sample'])
 stages=[json.loads(l)['sample'] for l in (root/f'p7-stages/calibration-depth-ordinary-run{repeat}-v39-mac.jsonl').open() if json.loads(l).get('event')=='physics.native-material-stages'];assert len(stages)==1
 stage=stages[0];after=rounded(stage['callback_after']);q0=rounded(stage['callback_local0']);q1=rounded(stage['callback_local1'])
 # Native callback telemetry keeps the previous tick until after simulation;
 # the first geometric pass has the point count verified by the private trace.
 point_count=audit['new_p7_solver_step0'][p7]['points']
 assert all(len(native_events[e])>=point_count for e in native_events)
 for n in native_events['calibration.body-contact'][:point_count]:assert after==rounded(n['after_axis0']+n['after_axis1']+n['after_axis2'])
 for n in native_events['calibration.contact-quaternion'][:point_count]:assert q0==rounded(n['local_quaternion0']) and q1==rounded(n['local_quaternion1'])
 stage_checks.append({'callback_axes_and_local_quaternions_exact':17,'native_callbacks_checked':point_count,'scope':'First geometry pass only; native public tick can include next simulation callbacks','callback_normal':stage['callback_plane'],'original_native_physical_states_unchanged':True})
assert stage_checks[0]==stage_checks[1];audit['original_callback_stage']=stage_checks
(root/'physics-v44-callback-triangle-audit.json').write_text(json.dumps(audit,indent=2)+'\n');print('67/72 long contacts exact, 5 remaining; 23 improved, none worsened. 189 isolated and 6 expanded scenarios exact in two repeats. 17 public callback components and 20 new solver rows exact.')
