"""Validate the reconstructed SSE routine against observed original writes."""
import argparse,json,pathlib,struct,subprocess,sys
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--probe',type=pathlib.Path,required=True);a=p.parse_args()
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]));import compare
f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
f32=lambda v:struct.unpack('<f',struct.pack('<f',v))[0]
reference=compare.load(a.measurements/'reference-long-contact-run0-v25-windows.jsonl','windows-physx-d3d9');runs=[];audit={'scope':'Formula reconstruction from observed unmodified original PhysX solver-body writes; recorded inputs are diagnostic only','repeats':{}}
for repeat in range(2):
 name='frame-v25-long-p9-x2-y0';stem=f'reference-native-inertia-watch-{name}'
 physical=compare.load(a.measurements/f'{stem}-physics-run{repeat}-v37-windows.jsonl','windows-physx-d3d9');assert physical=={name:reference[name]},'Hardware tracing changed physical states'
 rows=[];config=[]
 for line in (a.measurements/f'{stem}-geometry-run{repeat}-v37-windows.jsonl').open():
  x=json.loads(line)
  if x.get('event')=='watch-config':config.append(x)
  if x.get('function')=='solver-row-watch' and x['offset']==0x7c8a2:
   body=list(map(f,x['inertia_watch_ecx']['words']));q=body[28:32];d=body[37:40];local=[d[0],0,0,0,d[1],0,0,0,d[2]];expected=list(map(f,x['inertia_watched_body']['words'][15:24]))
   result=subprocess.run([str(a.probe.resolve())],input=' '.join(map(str,q+local))+'\n',text=True,capture_output=True,check=True)
   actual=list(map(f32,map(float,result.stdout.split())));assert actual==expected,(repeat,actual,expected)
   rows.append({'q':q,'local_diagonal':d,'native_solver_tensor':expected})
 assert len(rows)==40 and config and all(x['success'] for x in config)
 runs.append(rows);audit['repeats'][str(repeat)]={'tensor_writes':len(rows),'entries_exact':9*len(rows),'physical_baseline_unchanged':True}
assert runs[0]==runs[1];audit['inputs_and_outputs_repeatable']=True
(a.measurements/'physics-v41-internal-inertia-audit.json').write_text(json.dumps(audit,indent=2)+'\n');print('80 original tensor writes / 720 entries reconstructed exactly; two physical runs unchanged.')
