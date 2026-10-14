from pathlib import Path
import json,gzip,struct
r=Path.cwd();bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];value=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
def load(p):return [json.loads(line,parse_int=lambda v:-0.0 if v=='-0' else int(v)) for line in (gzip.open(p,'rt') if str(p).endswith('.gz') else p.open())]
proof=[];leads=[]
for rep in range(2):
 native=load(r/f'outputs/static-box-query-v107/native-next-curb224/curb-side-230-{rep}-private.jsonl.gz');p=r/'outputs/static-box-query-v107/mac-curb-checkpoints'
 for tick in [13,224]:
  tag=f'curb-side-{tick}-{rep}';prep=load(p/(tag+'-prep.jsonl'));solver=load(p/(tag+'-solver.jsonl'));mi=[x for x in solver if x['phase']=='spring-before'];mr=[x for x in solver if x['phase']=='spring-after'];ni=[x for x in native if x.get('sample_tick_v71')==tick and x.get('function')=='spring-solve-input'];nr=[x for x in native if x.get('sample_tick_v71')==tick and x.get('function')=='spring-solve-result'];count=len(mi)//5
  if tick==13:
   raw_angular_zero_sign_differences=[]
   for iteration in range(5):
    for index in range(count):
     mac=mi[iteration*count+index];pr=next(x for x in prep if x['wheel']==mac['wheel']);n=ni[iteration*count*3+index];row=n['solve_row']['words'];assert bits(mac['mass'])==row[11];assert list(map(bits,pr['world_axis']))==row[8:11];assert list(map(bits,pr['spring']['jacobian']))==row[12:15];assert list(map(bits,pr['spring']['inverse_jacobian']))==row[20:23];assert list(map(bits,mac['velocity']))==n['solve_body0']['words'][:3];a=list(map(bits,mac['angular']));b=n['solve_body0']['words'][4:7]
     assert all(x==y or value(x)==value(y)==0 for x,y in zip(a,b))
     if a!=b:raw_angular_zero_sign_differences.append({'iteration':iteration+1,'wheel':mac['wheel'],'Mac':a,'Native':b})
     assert bits(mr[iteration*count+index]['lambda'])==nr[iteration*count*3+index]['solve_row']['words'][28]
   proof.append({'repeat':rep,'physical_tick':13,'spring_rows':len(mi),'all_prepared_masses_axes_jacobians_inverse_jacobians_exact_Native':True,'all_incoming_linear_velocities_and_result_lambdas_exact_Native':True,'incoming_angular_zero_sign_only':raw_angular_zero_sign_differences})
  else:
   native_order=[]
   for n in ni[:count]:
    row=n['solve_row']['words'];match=[x for x in prep if x['spring']['active'] and bits(x['spring']['mass'])==row[11] and list(map(bits,x['spring']['jacobian']))==row[12:15] and list(map(bits,x['spring']['inverse_jacobian']))==row[20:23]];assert len(match)==1,(row,match);native_order.append(match[0]['wheel'])
   mac_order=[x['wheel'] for x in mi[:count]];assert native_order==[0,1,3] and mac_order==[3,1,0];assert list(map(bits,mi[0]['velocity']))==ni[0]['solve_body0']['words'][:3] and list(map(bits,mi[0]['angular']))==ni[0]['solve_body0']['words'][4:7]
   leads.append({'repeat':rep,'physical_tick':224,'all_three_corresponding_prepared_spring_properties_exact':True,'initial_incoming_body_linear_and_angular_exact':True,'Native_spring_order_bound_by_unique_mass_and_exact_geometry':native_order,'actual_Mac_spring_order':mac_order})
(r/'outputs/static-box-query-v107/live-curb13-proof.json').write_text(json.dumps({'scope':'Actual original Native and actual live V107 corrected curb13 spring rows: five iterations, geometry and resulting lambdas exact, incoming angular signed zero differences explicit. No hypothetical row permutation. Whole physical state first differs224.','proof':proof,'goal_complete':False},indent=2)+'\n')
(r/'outputs/static-box-query-v107/next-curb224-spring-order-lead.json').write_text(json.dumps({'scope':'Actual first224 after V107 fixes early static-box geometry. All corresponding spring properties and first body exact; traversal differs. Native0,1,3 vs Mac3,1,0. Bind actual contact callback queue/broadphase ranks and recreation; do not fit geometry or coefficients, do not apply fixture/frame sorting. Existing Source complete static-ground rank domain excludes decorations.','proof':leads,'goal_complete':False},indent=2)+'\n');print('Two repeats:10 curb13 spring rows exact prepared properties/results; next224 all three corresponding properties/body exact but actual traversal differs.')
