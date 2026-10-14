from pathlib import Path
import json,gzip,struct,subprocess,os,math
p=Path(__file__).resolve().parent
word=lambda x:struct.unpack('<I',struct.pack('<f',x))[0]
source=list(map(json.loads,open('outputs/mesh-parts-v149/source-rows342/track0-physical.jsonl')))
native=list(map(json.loads,gzip.open('outputs/body-contact-v144/native-step342/track0-run0-private.jsonl.gz','rt')))
cache=next(x['sample'] for x in source if x['event']=='physics.native-cache-input')
transport=next(x for x in native if x.get('function')=='point-transport-entry')
point_checks=[]
for i in range(4):
 expected=transport[f'transport_input_point_{i}']['words']
 actual=list(map(word,cache['current'][i]))+[word(cache['separations'][i])]
 assert actual==expected[:3]+expected[6:7]
 point_checks.append(dict(point=i,point_and_separation_exact=True))
rows=[x['sample'] for x in source if x['event']=='physics.native-contact-row' and x['sample']['step']==342 and x['sample']['iteration']==0]
prepared=[x for x in native if x.get('function')=='solver-initialize-exit'][:4]
row_checks=[]
for i,(s,n) in enumerate(zip(rows,prepared)):
 w=n['native_solver_row']['words'];row_checks.append(dict(row=i,mass_exact=word(s['effective_mass'])==w[20],bias_exact=word(s['scaled_bias'])==w[24],lever_exact=list(map(word,s['lever_arm']))==w[4:7]))
e={k:v for k,v in os.environ.items() if not k.startswith(('NX_','RRR3D_'))};e.update(json.load(open('outputs/hybrid-ray-v143/final-broad/manifest.json'))['environment'])
r=subprocess.run([str(p/'verify-material')],input=(p/'material-input.words').read_text(),text=True,capture_output=True,env=e,check=True);actual=[list(map(int,x.split())) for x in r.stdout.splitlines()];expected=[prepared[i]['native_solver_row']['words'][16:20] for i in [0,2]];assert actual==expected
sample=lambda path,event:[x.get('sample',x) for x in map(json.loads,gzip.open(path,'rt') if path.endswith('.gz') else open(path)) if x['event']==event]
n=sample('outputs/game-control-wake-v133/native/track0-rep0.jsonl.gz','world.sample')[342];old=sample('outputs/hybrid-ray-v143/moving-maps/track0-on0.jsonl','solo.sample')[342];new=sample('outputs/mesh-parts-v149/moving-maps/track0-on0.jsonl','solo.sample')[342]
errors={k:dict(baseline_l2=math.sqrt(sum((a-b)**2 for a,b in zip(n[k],old[k]))),candidate_l2=math.sqrt(sum((a-b)**2 for a,b in zip(n[k],new[k])))) for k in ['velocity','angular_momentum','rotation']}
report=dict(scope='Actual Mac V149 moving replay at first body contact physical342. Geometry-only correction with native input preservation and unchanged public controls.',live_point_and_separation_checks=point_checks,prepared_rows=row_checks,independent_per_triangle_material_quaternions_exact=actual,remaining_cause='Current runtime supplies one material frame for all points; original rows0/1 and rows2/3 use different callback triangle normals. Independent computation with each triangle normal reproduces all 8 original solver-quaternion words.',first_divergence_still=342,errors=errors,goal_complete=False)
(p/'live342-proof.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
