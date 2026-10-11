from pathlib import Path
import json,sys,struct,hashlib,argparse
from collections import defaultdict
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
parser=argparse.ArgumentParser(description='Verify saved v30 flat-contact diagnostic captures.')
parser.add_argument('--measurements',type=Path,required=True)
parser.add_argument('--contracts',type=Path,required=True)
args=parser.parse_args()
out=args.measurements;contracts={c['case']:c for c in json.loads(args.contracts.read_text())}
f=lambda x:struct.unpack('f',struct.pack('f',x))[0]
fv=lambda x:[f(v) for v in x]
def cross(a,b):return [f(f(a[1]*b[2])-f(a[2]*b[1])),f(f(a[2]*b[0])-f(a[0]*b[2])),f(f(a[0]*b[1])-f(a[1]*b[0]))]
def read(p):
 rs=[];n=None
 for line in p.open():
  r=json.loads(line);compare.finite(r)
  if r['event']=='calibration.begin':n=r['detail']
  rs.append({'case':n,'event':r['event'],'detail':r['detail'],'sample':r.get('sample')})
 return rs
def sha(rs):return hashlib.sha256(json.dumps(rs,sort_keys=True,separators=(',',':')).encode()).hexdigest()
def repeated(prefix):
 ps=[out/(prefix.format(r=r)+'.jsonl') for r in range(2)];a,b=[read(p) for p in ps];assert a==b
 return a,{'files':[p.name for p in ps],'records':len(a),'semantic_sha256':sha(a),'repeat_identical':True}
wr,wa=repeated('reference-inverse-mass-flat-run{r}-v30-windows')
oldw=read(out/'reference-inverse-mass-flat-run0-v29-windows.jsonl')
def project_module(rs):
 result=[]
 for r in rs:
  if r['event']=='calibration.windows-module':r={**r,'sample':{'module':r['sample']['module']}}
  result.append(r)
 return result
assert project_module([r for r in wr if r['event']!='calibration.local-points'])==project_module(oldw)
assert compare.load(out/wa['files'][0],'windows-physx-d3d9')==compare.load(out/'reference-inverse-mass-flat-run0-v29-windows.jsonl','windows-physx-d3d9')
native=defaultdict(lambda:defaultdict(list));inv={}
for r in wr:
 n=r['case'];s=r['sample']
 if r['event']=='calibration.inverse-mass':inv[n]=s
 if n in contracts and r['event'] in ['calibration.local-points','calibration.body-points','calibration.point-force'] and s['tick']==contracts[n]['first_tick']:native[n][r['event']].append(s)
proof={}
for n,events in native.items():
 ls=events['calibration.local-points'];ws=events['calibration.body-points'];assert len(ls)==len(ws)==16
 assert [p['index'] for p in ls]==list(range(16))
 for local,world in zip(ls,ws):
  assert local['actor0_dynamic'] and not local['actor1_dynamic'] and local['phase']=='pre-modify'
  assert [local['point_frame0_axis'+str(i)] for i in range(3)]==[[1,0,0],[0,1,0],[0,0,1]]
  r=[f(f(x)-f(y)) for x,y in zip(world['point0'],local['point_frame0_position'])]
  assert r==fv(local['local_position0'])
  assert fv(local['local_position1'])==fv(world['point1'])
 proof[n]={'raw_local_arm_exactly_recovered_from_world':True,'local_points_first_contact':16,'actor_frame_identity':True,'first_local_arm':ls[0]['local_position0']}
newkeys=['r_cross_normal0','r_cross_normal1','inverse_inertia_r_cross_normal0','inverse_inertia_r_cross_normal1']
def legacy_projection(rs):
 result=[]
 for r in rs:
  if r['event']=='physics.source-normal-row':
   s={k:v for k,v in r['sample'].items() if k not in newkeys}
   for field in ['angular0','angular1']:s[field]=[-v for v in s[field]]
   r={**r,'sample':s}
  result.append(r)
 return result
mr,ma=repeated('calibration-inverse-normal-rows-flat-run{r}-v30-mac')
assert legacy_projection(mr)==read(out/'calibration-inverse-normal-rows-flat-run0-v29-mac.jsonl')
assert compare.load(out/ma['files'][0],'mac-jolt-metal')==compare.load(out/'calibration-inverse-normal-rows-flat-run0-v29-mac.jsonl','mac-jolt-metal')
captures={}
for g in ['points','both']:
 for mask in [0,2]:
  rs,a=repeated('calibration-row-arithmetic-'+g+'-mask'+str(mask)+'-run{r}-v30-mac')
  assert legacy_projection(rs)==read(out/f'calibration-row-arithmetic-{g}-mask{mask}-run0-v29-mac.jsonl')
  cases=compare.load(out/a['files'][0],'mac-jolt-metal');assert cases==compare.load(out/f'calibration-row-arithmetic-{g}-mask{mask}-run0-v29-mac.jsonl','mac-jolt-metal')
  rows=defaultdict(list)
  for r in rs:
   n=r['case'];s=r['sample']
   if r['event']=='physics.source-normal-row' and s['step']==contracts[n]['first_tick']:rows[n].append(s)
  results={}
  for n,rr in rows.items():
   assert len(rr)==16
   locals_=native[n]['calibration.local-points'][-8:];normal=fv(native[n]['calibration.point-force'][0]['normal'])
   expected_cross=[];expected_response=[]
   for p in locals_:
    rv=fv(p['local_position0']);c=cross(rv,normal);expected_cross.append(c)
    matrix=[fv(inv[n]['inverse_inertia_axis'+str(i)]) for i in range(3)]
    v=[f(f(f(matrix[0][axis]*c[0])+f(matrix[1][axis]*c[1]))+f(matrix[2][axis]*c[2])) for axis in range(3)];expected_response.append(v)
   ec=[];ei=[]
   for r in rr:
    i=r['index'];ec.append(fv(r['r_cross_normal1'])==expected_cross[i]);ei.append(fv(r['inverse_inertia_r_cross_normal1'])==expected_response[i])
    assert fv(r['r_cross_normal0'])==[0,0,0] and fv(r['inverse_inertia_r_cross_normal0'])==[0,0,0]
   results[n]={'prepared_rows':len(rr),'cross_matches_scalar_f32_native_input_model':sum(ec),'inverse_response_matches_scalar_f32_native_input_model':sum(ei),
               'expected_cross':expected_cross,'expected_inverse_response':expected_response,'actual_first_rows':rr[:8]}
  a['cases']=results;captures[g+'-mask'+str(mask)]=a
  print(g,mask,'cross',sum(c['cross_matches_scalar_f32_native_input_model'] for c in results.values()),'invresponse',sum(c['inverse_response_matches_scalar_f32_native_input_model'] for c in results.values()),'of192',flush=True)
report={'schema':1,'scope':'Raw Native callback local points and actual Mac prepared Jacobians. The expected Native Jacobians are reconstructed by a scalar float32 model, not directly read from the private SDK solver.',
 'native':wa,'ordinary_mac':ma,'native_added_diagnostics_preserve_all_prior_events_except_module_path':True,'mac_legacy_row_projection_exact':True,
 'angular_diagnostic_correction':'v29 row angular fields used polar axis permutation. v30 uses axial det(P)*P; physical angular state and solve unchanged.',
 'local_arms':proof,'replay_jacobians':captures,'physical_states_unchanged':True,'goal_complete':False}
(out/'physics-v30-jacobian-input-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print('Local-arm precision-loss hypothesis excluded in all12 first contacts; all physical states and diagnostic repeats exact.')
