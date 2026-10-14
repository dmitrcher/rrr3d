from pathlib import Path
import json,gzip,struct
root=Path.cwd();out=root/'outputs/wheel-objects-v104';bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];reports=[]
for rep in range(2):
 tag=f'slope-longitudinal-21-{rep}';n=list(map(json.loads,gzip.open(root/'outputs/row-lifetime-v103/native-query-window'/f'{tag}-private.jsonl.gz','rt')));ni=[r for r in n if r.get('function')=='spring-solve-input' and r['sample_tick_v71']==21];nr=[r for r in n if r.get('function')=='spring-solve-result' and r['sample_tick_v71']==21];m=list(map(json.loads,(out/'mac-live-long21/slope-longitudinal'/f'{tag}-solver.jsonl').open()));mi=[r for r in m if r['phase']=='spring-before'];mr=[r for r in m if r['phase']=='spring-after'];assert len(ni)==len(nr)==60 and len(mi)==len(mr)==20
 assert [r['wheel'] for r in mi[:4]]==[3,1,0,2];checkpoints=[]
 for i in range(20):
  j=i//4*12+i%4;native=ni[j];actual=mi[i];assert bits(actual['mass'])==native['solve_row']['words'][11];assert list(map(bits,actual['velocity']))==native['solve_body0']['words'][:3];assert list(map(bits,actual['angular']))==native['solve_body0']['words'][4:7];assert bits(mr[i]['lambda'])==nr[j]['solve_row']['words'][28]
  checkpoints.append({'iteration':i//4+1,'wheel':actual['wheel'],'incoming_velocity3_angular3_mass1_result_lambda1_float32_words_exact':True})
 reports.append({'repeat':rep,'physical_tick':21,'actual_spring_order':[3,1,0,2],'uninterrupted_checkpoints':checkpoints})
(out/'live-long21-proof.json').write_text(json.dumps({'scope':'Current V104 SDK actor-wide vertical-row recreation: 20 uninterrupted live spring checkpoints per repeat match actual original Native at first affected physical21. Original600step Golden-exact histories and Source600step focusedV104 unchanged repeats independently verified. Native result row proves lambda before body application, incoming later rows prove continuing state. Whole longitudinal still first24, not certified complete.','repeats':reports,'goal_complete':False},indent=2)+'\n');print('Both repeats: all20 uninterrupted live spring input velocity/angular/mass and Native resultlambda checkpoints exact.')
