from pathlib import Path
import json,gzip,subprocess,hashlib,sys
mode=sys.argv[1] if len(sys.argv)>1 else 'tilt';assert mode in ['base','offset','tilt','tilt-offset'];npath=Path('outputs/controller-steering-transform-v180'+('-'+mode if mode!='base' else ''));native=[x for x in map(json.loads,gzip.open(npath/'accepted-native0.jsonl.gz','rt')) if x['event']=='steering.pose'];out=Path('outputs/controller-steering-matrix-v181/transform-'+mode);out.mkdir(parents=True,exist_ok=True);fields=[]
for x in native:fields.append([x['case'],int(x['angle_hook_hit']),*x['position_before'],*x['rotation_before'],*x['matrix_before'],x['angle_word'] if x['angle_hook_hit'] else 0])
inputs=out/'inputs.words';inputs.write_text(''.join(' '.join(map(str,row))+'\n' for row in fields));exe=Path('work/controller-steering-matrix-v181/SourceTransformProbe').resolve();reports=[]
cz=lambda a:[0 if x==0x80000000 else x for x in a]
for rep in range(2):
 output=out/f'source{rep}.jsonl';subprocess.run([str(exe),str(inputs),str(output)],check=True);source=list(map(json.loads,output.open()));assert len(source)==90
 for variant in range(2):
  rows=[x for x in source if x['mode']==variant];bad=[];matrix=[]
  for n,s in zip(native,rows):
   if cz(s['matrix'])!=cz(n['matrix_before']):matrix.append(n['case'])
   if cz(s['position'])!=cz(n['position_call']) or cz(s['rotation'])!=cz(n['rotation_call']):bad.append(dict(case=n['case'],native_position=n['position_call'],source_position=s['position'],native_rotation=n['rotation_call'],source_rotation=s['rotation']))
  reports.append(dict(repeat=rep,variant='decoded SDK matrix' if variant else 'observed matrix as method INPUT',cases=45,exact=45-len(bad),differences=bad,matrix_decode_differences=matrix));print(mode,rep,variant,'exact',45-len(bad),'decoded matrix differences',len(matrix),bad[:1],flush=True)
assert (out/'source0.jsonl').read_bytes()==(out/'source1.jsonl').read_bytes();(out/'comparison.json').write_text(json.dumps(dict(scope='Pure transform boundary: measured original before-state and angular INPUT versus actual original WheelsProgress public pose setter result. Does not claim Source backend velocity-to-angle calculation or whole tilted-world physics integration.',runs=reports,source_probe_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),goal_complete=False),indent=2)+'\n')
