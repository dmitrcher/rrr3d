from pathlib import Path
import json,gzip,struct
root=Path.cwd();bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];swap=lambda v:[v[0],v[2],v[1]]
def load(p):
 if not p.exists():p=Path(str(p)+'.gz')
 return [json.loads(line,parse_int=lambda value: -0.0 if value=='-0' else int(value)) for line in (gzip.open(p,'rt') if str(p).endswith('.gz') else p.open())]
repeats=[]
for rep in range(2):
 n=[r for r in load(root/f'outputs/tire-lifetime-v106/native-manager-reset/bump-crossing-80-{rep}-private.jsonl.gz') if r.get('sample_tick_v71')==57];ni=[r for r in n if r.get('function')=='spring-solve-input'];nr=[r for r in n if r.get('function')=='spring-solve-result'];p=root/'outputs/tire-lifetime-v106/mac-bump57-after-force-reset/bump-crossing';tag=f'bump-crossing-57-{rep}';prep=load(p/(tag+'-prep.jsonl'));solver=load(p/(tag+'-solver.jsonl'));ms=[r for r in solver if r['phase']=='spring-before'];mr=[r for r in solver if r['phase']=='spring-after'];tires=load(p/(tag+'-tire.jsonl'));mi=[r for r in tires if r['event'].endswith('-input')];mt=[r for r in tires if r['event'].endswith('-result')];assert len(ni)==len(nr)==60 and len(ms)==len(mr)==20 and len(mi)==len(mt)==40
 orders=[]
 for iteration in range(5):
  for i in range(4):
   native=ni[iteration*12+i];r=native['solve_row']['words'];mac=ms[iteration*4+i];assert bits(mac['mass'])==r[11];assert list(map(bits,mac['velocity']))==native['solve_body0']['words'][:3];assert list(map(bits,mac['angular']))==native['solve_body0']['words'][4:7];assert bits(mr[iteration*4+i]['lambda'])==nr[iteration*12+i]['solve_row']['words'][28]
  visits=[]
  for i in range(8):
   native=ni[iteration*12+4+i];r=native['solve_row']['words'];mac=mi[iteration*8+i];match=[p for p in prep if list(map(bits,swap(p['world_point'])))==list(map(bits,mac['position']))];assert len(match)==1;visits.append(match[0]['wheel'])
   assert bits(mac['mass'])==r[11];assert list(map(bits,[-v for v in swap(mac['axis'])]))==r[8:11];assert list(map(bits,swap(mac['r_cross_axis'])))==r[12:15];assert list(map(bits,swap(mac['inv_i_r_cross_axis'])))==r[20:23]
   assert list(map(bits,swap(mac['linear'])))==native['solve_body0']['words'][:3];assert list(map(bits,[-v for v in swap(mac['angular'])]))==native['solve_body0']['words'][4:7]
   assert bits(mac['minimum'])==r[30] and bits(mac['maximum'])==r[29],(i,mac['minimum'],mac['maximum'],r[29:31])
   assert bits(mt[iteration*8+i]['lambda'])==nr[iteration*12+4+i]['solve_row']['words'][28]
  assert visits==[1,1,0,0,3,3,2,2];orders.append(visits)
 repeats.append({'repeat':rep,'physical_tick':57,'spring_rows_all_inputs_mass_result_lambda_exact':20,'tire_rows_all_mass_axis_jacobian_inverse_jacobian_incoming_velocity_angular_limits_result_lambda_exact':40,'actual_tire_visits':orders,'new_front_tire_limits_zero_exact_Native':True})
report={'scope':'Actual original Windows and actual live Mac private spring/tire solve inputs and results at bump57; all five iterations, not a hypothetical permutation. Object removal resets previous spring force and independent tire creation age. Wheel anchor retained. Full physical Windows comparison is separately validated over600steps.','repeats':repeats,'goal_complete':False};(root/'outputs/tire-lifetime-v106/live-bump57-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Two actual live repeats:20 spring +40 tire rows all incoming bodies/properties/limits/results exact original Windows.')
