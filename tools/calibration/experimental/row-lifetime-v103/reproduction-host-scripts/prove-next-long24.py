from pathlib import Path
import json,gzip,struct
root=Path.cwd();o=root/'outputs/row-lifetime-v103';bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];reports=[]
swap=lambda v:[v[0],v[2],v[1]]
for rep in range(2):
 tag=f'slope-longitudinal-24-{rep}';n=list(map(json.loads,gzip.open(o/'native-next-long24'/f'{tag}-private.jsonl.gz','rt')));ni=[r for r in n if r.get('function')=='spring-solve-input'];nr=[r for r in n if r.get('function')=='spring-solve-result'];p=o/'mac-next-long24/slope-longitudinal';prep=list(map(json.loads,(p/f'{tag}-prep.jsonl').open()));ms=list(map(json.loads,(p/f'{tag}-solver.jsonl').open()));mi=[r for r in ms if r['phase']=='spring-before'];mr=[r for r in ms if r['phase']=='spring-after'];mt=list(map(json.loads,(p/f'{tag}-tire.jsonl').open()));tin=[r for r in mt if r['event'].endswith('-input')];tout=[r for r in mt if r['event'].endswith('-result')];assert len(ni)==len(nr)==60 and len(mi)==len(mr)==20 and len(tin)==len(tout)==40
 for i in range(4):
  r=ni[i]['solve_row']['words'];x=mi[i];pm=next(p for p in prep if p['wheel']==x['wheel']);assert bits(x['mass'])==r[11];assert list(map(bits,pm['world_axis']))==r[8:11];assert list(map(bits,pm['spring']['jacobian']))==r[12:15];assert list(map(bits,pm['spring']['inverse_jacobian']))==r[20:23];assert list(map(bits,x['velocity']))==ni[i]['solve_body0']['words'][:3] and list(map(bits,x['angular']))==ni[i]['solve_body0']['words'][4:7];assert bits(mr[i]['lambda'])==nr[i]['solve_row']['words'][28]
 macrows=[]
 for x in tin[:8]:
  match=[p for p in prep if list(map(bits,swap(p['world_point'])))==list(map(bits,x['position']))];assert len(match)==1;macrows.append((match[0]['wheel'],x))
 assert [w for w,x in macrows[::2]]==[3,1,0,2];native_order=[]
 for r in ni[4:12]:
  row=r['solve_row']['words'];match=[(w,x) for w,x in macrows if bits(x['mass'])==row[11]];assert len(match)==1;wheel,x=match[0];native_order.append(wheel)
  assert list(map(bits,[-v for v in swap(x['axis'])]))==row[8:11];assert list(map(bits,swap(x['r_cross_axis'])))==row[12:15];assert list(map(bits,swap(x['inv_i_r_cross_axis'])))==row[20:23]
 assert native_order==[3,3,2,2,1,1,0,0]
 for i in range(2):
  assert list(map(bits,swap(tin[i]['linear'])))==ni[4+i]['solve_body0']['words'][:3];assert list(map(bits,[-v for v in swap(tin[i]['angular'])]))==ni[4+i]['solve_body0']['words'][4:7];assert bits(tout[i]['lambda'])==nr[4+i]['solve_row']['words'][28]
 q=list(map(json.loads,gzip.open(o/'native-query-window'/f'slope-longitudinal-21-{rep}-private.jsonl.gz','rt')));handles={}
 for r in q:
  if r.get('function')=='wheel-query-entry' and r['query_mode']==1:
   handles.setdefault(r['sample_tick_v71'],{})[r['queried_wheel_tail']['words'][67]]=r['queried_wheel_tail']['words'][71:73]
 assert handles[19][0]==handles[20][0]==handles[21][0] and handles[19][1]==handles[20][1]==handles[21][1];indices={w:[h&0x3ffff for h in hs] for w,hs in handles[21].items()};assert sorted(indices,key=lambda w:max(indices[w]),reverse=True)==[3,2,1,0]
 reports.append({'repeat':rep,'physical_tick':24,'first_four_springs_all_prepared_properties_inputs_lambda_exact':True,'all_eight_tire_rows_mass_axis3_jacobian3_inverse_jacobian3_exact':True,'Mac_tire_wheels_bound_by_actual_world_contact_points':[w for w,x in macrows[::2]],'Native_tire_wheel_order_bound_by_unique_mass_and_geometry':native_order[::2],'first_two_tire_inputs_and_result_lambdas_exact':True,'actual_Native_tire_handle_indices_at21':indices,'front_tire_handles_retained19_20_21':True,'descending_cached_tire_object_indices_predict_actual_Native_order':True})
(o/'next-long24-tire-order-lead.json').write_text(json.dumps({'scope':'Original Windows physical24 and current V103 fresh600step repeats: four springs and first tire pair exact; all corresponding tire row properties exact; second tire wheel selected differs. Original tire handles persist while actor-wide spring/stop objects recreate. Existing V87 sorts tires using broadphase pair ranks instead of the measured cached tire-object lifetime. Need generic actual tire create/delete/allocator binding before V104 edit; no case/frame dispatch or coefficient fitting. No V104 runtime edit.','repeats':reports,'goal_complete':False},indent=2)+'\n');print('Two repeats: tire properties80words exact; Native cached tire order3,2,1,0 vs actual Mac3,1,0,2. No V104 edit.')
