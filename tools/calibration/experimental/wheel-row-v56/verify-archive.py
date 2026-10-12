from pathlib import Path
import argparse,zipfile,tempfile,hashlib,json,subprocess,sys,io,importlib.util
p=argparse.ArgumentParser();p.add_argument('--archive',type=Path,required=True);p.add_argument('--out',type=Path,required=True);args=p.parse_args();args.out.parent.mkdir(parents=True,exist_ok=True);sys.dont_write_bytecode=True
with zipfile.ZipFile(args.archive) as z,tempfile.TemporaryDirectory(prefix='motorrock-v56-verify-') as tmp:
 root=Path(tmp);manifest=json.loads(z.read('manifest.json'));assert len(set(z.namelist()))==len(z.namelist());assert set(z.namelist())=={a['path'] for a in manifest['files']}|{'manifest.json','README.txt'}
 for a in manifest['files']:
  h=hashlib.sha256();size=0
  with z.open(a['path']) as f:
   while b:=f.read(1024*1024):h.update(b);size+=len(b)
  assert size==a['bytes'] and h.hexdigest()==a['sha256'],a['path']
 print('All',len(manifest['files']),'archive members hash/size verified',flush=True)
 for name in z.namelist():
  if name.startswith('tools/') or name.startswith('source/wheel-row-v56/') and Path(name).name in ['verify-math.py','verify-expanded.cpp','NativeWheelRowV56.h','NativeSpringSplitV53.h','expanded-native-inputs.json']:
   assert '..' not in Path(name).parts;path=root/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(z.read(name))
 sys.path.insert(0,str(root/'tools'));import compare
 original_path=compare.Path
 class PackedPath:
  def __init__(self,name):self.name=name
  def open(self,mode='r',encoding=None):
   f=z.open(self.name);return f if 'b'in mode else io.TextIOWrapper(f,encoding=encoding or 'utf-8')
  def __str__(self):return self.name
 compare.Path=lambda x:x if isinstance(x,PackedPath) else original_path(x)
 s=importlib.util.spec_from_file_location('words',root/'tools/compare-float32.py');w=importlib.util.module_from_spec(s);s.loader.exec_module(w);count=0
 for scope in ['native24','long72','old6','isolation00','isolation01','isolation02','isolation03']:
  ref=PackedPath(f'measurements/references/{scope}-windows.jsonl')
  for mode in ['control','candidate']:
   a=PackedPath(f'measurements/gates/{scope}-{mode}-run0-mac.jsonl');b=PackedPath(f'measurements/gates/{scope}-{mode}-run1-mac.jsonl');assert compare.load(a,'mac-jolt-metal')==compare.load(b,'mac-jolt-metal');r=w.audit(ref,a);assert not r['cases_remaining'];count+=len(r['cases_exact']) if mode=='candidate' else 0
 assert count==291;print('291 retained gates, controls/candidates and both actual repeats verified',flush=True)
 # Bind the portable 84-row math input to both actual original traces,
 # and independently verify unchanged physical Windows histories.
 selected=[];raw_points=[('settle',20,'measurements/native-first-rows','')]
 raw_points += [(n,t,'measurements/native-expanded-rows',str(i)+'-') for i,(n,t) in enumerate(json.loads(z.read('measurements/native-expanded-rows/capture-manifest.json'))['cases'])]
 def reference(name):
  path='measurements/first-reference/windows.jsonl' if name not in ['slope-longitudinal','slope-lateral'] else 'measurements/slope-references/'+('longitudinal' if name=='slope-longitudinal' else 'lateral')+'-windows.jsonl'
  return compare.load(PackedPath(path),'windows-physx-d3d9')[name]
 for name,t,base,prefix in raw_points:
  gold=reference(name);payload=[]
  for rep in range(2):
   physical=compare.load(PackedPath(f'{base}/{prefix}physical-run{rep}.jsonl'),'windows-physx-d3d9')[name]['rows'];assert len(physical)==600 and all(w.physical(a)==w.physical(b) for a,b in zip(physical,gold['rows']))
   trace=[json.loads(l) for l in z.read(f'{base}/{prefix}private-run{rep}.jsonl').decode().splitlines()];jac=[x for x in trace if x.get('function')=='spring-row-jacobians'];ready=[x for x in trace if x.get('function')=='spring-row-ready'];assert len(jac)==len(ready)==12;rows=[]
   for j,(a,b) in enumerate(zip(jac,ready)):
    assert a['constraint_row']['address']==b['constraint_row']['address'];body=a['preparation_body0']['words'];rows.append({'case':name,'counter':t,'row_index':j,'inertia':body[15:24],'inverse_mass':body[11],'row':b['constraint_row']['words'][4:32]})
   payload.append(rows)
  assert payload[0]==payload[1];selected+=payload[0]
 assert selected==json.loads(z.read('source/wheel-row-v56/expanded-native-inputs.json'))['rows']
 for i,(name,t) in enumerate(json.loads(z.read('measurements/native-next-rows/capture-manifest.json'))['cases']):
  payload=[];gold=reference(name)
  for rep in range(2):
   rows=compare.load(PackedPath(f'measurements/native-next-rows/{i}-physical-run{rep}.jsonl'),'windows-physx-d3d9')[name]['rows'];assert len(rows)==600 and all(w.physical(a)==w.physical(b) for a,b in zip(rows,gold['rows']))
   trace=[json.loads(l) for l in z.read(f'measurements/native-next-rows/{i}-private-run{rep}.jsonl').decode().splitlines()];rr=[x['constraint_row']['words'][4:32] for x in trace if x.get('function')=='spring-row-ready'];assert len(rr)==12;payload.append(rr)
  assert payload[0]==payload[1] and any(x[3]&0x7fffffff for x in payload[0][4::2])
 print('84 defined rows bound to14original captures;6next-step captures and all600Windows states each verified',flush=True)
 math_path=args.out.with_name(args.out.stem+'-math.json');subprocess.run([sys.executable,str(root/'source/wheel-row-v56/verify-math.py'),'--out',str(math_path)],check=True)
 broad_path=args.out.with_name(args.out.stem+'-broad.json');subprocess.run([sys.executable,str(root/'tools/compare-broad.py'),'--archive',str(args.archive.resolve()),'--out',str(broad_path)],check=True);broad=json.loads(broad_path.read_text());assert broad['samples']==255864 and len(broad['exact'])==65 and len(broad['remaining'])==543 and broad['two_repeats_exact']
 args.out.write_text(json.dumps({'files_hash_verified':len(manifest['files']),'source_commit':manifest['source_commit'],'retained_original_gates':291,'actual_original_row_capture_points':7,'prepared_rows_bound_to_raw_traces':84,'next_step_original_capture_points':3,'portable_math_report':str(math_path),'portable_broad_report':str(broad_path),'broad_cases':608,'complete_exact_cases':65,'remaining_cases':543,'actual_mac_repeats_exact':True,'goal_complete':False},indent=2)+'\n')
print('Portable archive verified; all-discrepancies goal remains active',flush=True)
