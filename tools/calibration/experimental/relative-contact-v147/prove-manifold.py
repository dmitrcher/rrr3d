from pathlib import Path
import subprocess,json,os,gzip,itertools
p=Path(__file__).resolve().parent
include=p/'include'
env={k:v for k,v in os.environ.items() if not k.startswith(('RRR3D_','NX_'))}
env.update(json.loads((p/'environment.json').read_text()))
input_words=(p/'manifold-inputs.txt').read_text()
raw=list(map(json.loads,gzip.open(p/'evidence/native/track0-run0-private.jsonl.gz','rt')))
transport=next(x for x in raw if x.get('function')=='point-transport-entry')
expected=[transport[f'transport_input_point_{i}']['words'][:7] for i in range(4)]
checks=[]
for optimization in ['-O0','-O2']:
 for relative,shared,vertex in itertools.product([0,1],repeat=3):
  exe=p/f'manifold-{optimization}-{relative}{shared}{vertex}'
  subprocess.run(['clang++','-std=c++17',optimization,'-ffp-contract=off',f'-DMR_V147_RELATIVE={relative}',f'-DMR_V147_SHARED={shared}',f'-DMR_V147_VERTEX_PLANE={vertex}','-I'+str(p/'include'),'-I'+str(include),str(p/'manifold-relative.cpp'),'-o',str(exe)],check=True)
  r=subprocess.run([str(exe),str(p/'source-poses.words')],input=input_words,text=True,env=env,capture_output=True,check=True)
  lines=r.stdout.splitlines();assert lines[0]=='4'
  actual=[list(map(int,line.split())) for line in lines[1:]]
  differences=[dict(contact=i,field=j,actual=a,expected=e) for i,(es,as_) in enumerate(zip(expected,actual)) for j,(e,a) in enumerate(zip(es,as_)) if e!=a]
  item=dict(optimization=optimization,relative_quaternion_transforms=bool(relative),shared_plane=bool(shared),recompute_plane_distance_at_current_vertex=bool(vertex),exact_words=28-len(differences),differences=differences)
  checks.append(item)
  if relative and shared and vertex:assert not differences
  print(optimization,relative,shared,vertex,item['exact_words'],'/28',flush=True)
report=dict(scope='Offline one contact group at map0 physical342. Source box position/quaternion equal original input words; raw mesh vertices from Source. The known first-triangle patch grouping is supplied explicitly for causal isolation and is not a general runtime grouping algorithm.',checks=checks,all_enabled_exact_both_optimization_levels=True,goal_complete=False)
(p/'manifold-proof.json').write_text(json.dumps(report,indent=2)+'\n')
