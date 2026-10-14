from pathlib import Path
import json,gzip,struct
r=Path.cwd();bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];swap=lambda v:[v[0],v[2],v[1]]
def load(p):return [json.loads(line,parse_int=lambda v:-0.0 if v=='-0' else int(v)) for line in (gzip.open(p,'rt') if str(p).endswith('.gz') else p.open())]
signed_zero_differences=[]
def eq(a,b):
    if isinstance(a,int):a=[a];b=[b]
    if a==b:return True
    if len(a)!=len(b):return False
    for index,(x,y) in enumerate(zip(a,b)):
        if x==y:continue
        if x&0x7fffffff==0 and y&0x7fffffff==0:
            signed_zero_differences.append({'comparison':len(signed_zero_differences),'component':index,'Mac_word':x,'Native_word':y})
        else:return False
    return True
proof=[]
for rep in range(2):
 n=[row for row in load(r/f'outputs/contact-bias-v109/native-wheel79/border-grounded-79-{rep}-private.jsonl.gz') if row.get('sample_tick_v71')==79];ni=[row for row in n if row.get('function')=='spring-solve-input'];nr=[row for row in n if row.get('function')=='spring-solve-result'];p=r/'outputs/contact-bias-v109/mac-final79';tag=f'border-grounded-79-{rep}';prep=load(p/(tag+'-prep.jsonl'));solver=load(p/(tag+'-solver.jsonl'));si=[row for row in solver if row['phase']=='spring-before'];sr=[row for row in solver if row['phase']=='spring-after'];tire=load(p/(tag+'-tire.jsonl'));ti=[row for row in tire if row['event'].endswith('-input')];tr=[row for row in tire if row['event'].endswith('-result')];assert len(ni)==len(nr)==45 and len(si)==len(sr)==15 and len(ti)==len(tr)==30;orders=[]
 for iteration in range(5):
  assert [row['wheel'] for row in si[iteration*3:iteration*3+3]]==[1,2,3]
  for index in range(3):
   m=si[iteration*3+index];n=ni[iteration*9+index];row=n['solve_row']['words'];pr=next(x for x in prep if x['wheel']==m['wheel']);assert bits(m['mass'])==row[11];assert list(map(bits,pr['world_axis']))==row[8:11];assert list(map(bits,pr['spring']['jacobian']))==row[12:15];assert list(map(bits,pr['spring']['inverse_jacobian']))==row[20:23];assert list(map(bits,m['velocity']))==n['solve_body0']['words'][:3];assert list(map(bits,m['angular']))==n['solve_body0']['words'][4:7];assert bits(sr[iteration*3+index]['lambda'])==nr[iteration*9+index]['solve_row']['words'][28]
  visits=[]
  for index in range(6):
   m=ti[iteration*6+index];n=ni[iteration*9+3+index];row=n['solve_row']['words'];match=[x for x in prep if list(map(bits,swap(x['world_point'])))==list(map(bits,m['position']))];assert len(match)==1;visits.append(match[0]['wheel']);assert bits(m['mass'])==row[11];assert eq(list(map(bits,[-v for v in swap(m['axis'])])),row[8:11]);assert list(map(bits,swap(m['r_cross_axis'])))==row[12:15];assert list(map(bits,swap(m['inv_i_r_cross_axis'])))==row[20:23];assert list(map(bits,swap(m['linear'])))==n['solve_body0']['words'][:3];assert list(map(bits,[-v for v in swap(m['angular'])]))==n['solve_body0']['words'][4:7];assert eq([bits(m['minimum']),bits(m['maximum'])],[row[30],row[29]]);assert bits(tr[iteration*6+index]['lambda'])==nr[iteration*9+3+index]['solve_row']['words'][28]
  orders.append(visits)
 proof.append({'repeat':rep,'physical_tick':79,'all_five_iterations_spring_order_exact':[1,2,3],'prepared_spring_properties_incoming_bodies_result_lambdas_exact':15,'tire_values_incoming_bodies_limits_result_lambdas_equal_except_signed_zero':30,'actual_tire_visits':orders})
(r/'outputs/contact-bias-v109/live-border79-proof.json').write_text(json.dumps({'scope':'Actual original Native and actual live Mac V109 at the formerly first divergent79. All five iterations, prepared geometry, incoming bodies, limits and lambdas match as float32 values. Signed-zero raw ABI differences are separately retained; raw bit equality is not claimed. Entire border-grounded600 histories independently Native-equivalent. Generic manager lifecycle drives order; no hypothetical row permutation or reference reset.','proof':proof,'raw_signed_zero_differences':signed_zero_differences,'raw_ABI_bit_equality_claimed':not signed_zero_differences,'goal_complete':False},indent=2)+'\n');print('Two repeats:15 springs exact;30 tire values equal except signed zeros:',len(signed_zero_differences))
