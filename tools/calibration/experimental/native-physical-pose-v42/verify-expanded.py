import argparse,json,pathlib,sys,hashlib
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--probe',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]));import verify_isolation
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();reference=compare.load(root/'reference-long-contact-run0-v25-windows.jsonl','windows-physx-d3d9');wm=json.loads((root/'expanded-native/capture-manifest.json').read_text());mm=json.loads((root/'expanded-mac/capture-manifest.json').read_text());assert mm['probe_sha256']==sha(a.probe)
windows=[]
for i in range(2):
 path=root/f'expanded-native/original-long72-run{i}-windows.jsonl';assert sha(path)==wm['runs']['original'+str(i)]['sha256'];assert compare.load(path,'windows-physx-d3d9')==reference
 path=root/f'expanded-native/expanded-pose-run{i}-windows.jsonl';assert sha(path)==wm['runs']['expanded'+str(i)]['sha256'];windows.append(compare.load(path,'windows-physx-d3d9'))
assert windows[0]==windows[1];native=windows[0];assert set(native)=={f'pose-v42-frozen-{i}' for i in range(6)};report={'scope':'Six no-AI frozen contacts varying COM/shape origin; center restore ablation holds geometry/cache correction on','original72_unmodified_with_expanded_reference_probe':True,'windows_repeatable':True,'captures':{}}
for mode in ['ordinary','control']:
 runs=[];diagnostics=[]
 for i in range(2):
  path=root/f'expanded-mac/expanded-pose-{mode}-run{i}-mac.jsonl';info=mm['captures'][mode+str(i)];assert sha(path)==info['sha256'];cases,d=verify_isolation.capture(path,'mac-jolt-metal','pose-v42-frozen-');runs.append(cases);diagnostics.append(d);assert set(cases)==set(native)
  assert info['environment']['RRR3D_EXPERIMENT_NATIVE_CACHE_POSE']=='1' and info['environment']['RRR3D_EXPERIMENT_NATIVE_GEOMETRY_POSE']=='1';assert (info['environment'].get('RRR3D_EXPERIMENT_NATIVE_FROZEN_CM_RESTORE')=='1')==(mode=='ordinary')
  if mode=='control':assert cases==compare.load(root/f'expanded-before/expanded-pose-ordinary-run{i}-mac.jsonl','mac-jolt-metal'),'Disabled COM restore changed prior states'
  stats={n:{'first_difference':next((x['tick'] for x,y in zip(native[n]['rows'],cases[n]['rows']) if x!=y),None),'max_position':max(compare.norm(x['position'],y['position']) for x,y in zip(native[n]['rows'],cases[n]['rows'])),'max_velocity':max(compare.norm(x['velocity'],y['velocity']) for x,y in zip(native[n]['rows'],cases[n]['rows']))} for n in native};assert stats==info['cases'];report['captures'][mode+str(i)]=stats
  count=0
  for line in path.open():
   x=json.loads(line)
   if x.get('event')=='calibration.profile':assert all(x['sample'][k]==1 for k in ['body_friction_scale','brake_scale','damper_scale','gravity_scale','lateral_grip_scale','longitudinal_grip_scale','spring_scale','torque_scale']);count+=1
  assert count==6
 assert runs[0]==runs[1] and diagnostics[0]['semantic_sha256']==diagnostics[1]['semantic_sha256'] and diagnostics[0]['contact_semantic_sha256']==diagnostics[1]['contact_semantic_sha256']
new=report['captures']['ordinary0'];old=report['captures']['control0'];assert sum(v['first_difference'] is None for v in new.values())==2 and sum(v['first_difference'] is None for v in old.values())==1;report['remaining']=4;report['max_position_after']=max(v['max_position'] for v in new.values());report['max_position_before']=max(v['max_position'] for v in old.values());(root/'physics-v42-expanded-audit.json').write_text(json.dumps(report,indent=2)+'\n');print('Expanded contacts: 2/6 exact, 4 remaining; position max',report['max_position_before'],'->',report['max_position_after'],'; two repeats, disabled control checked.')
