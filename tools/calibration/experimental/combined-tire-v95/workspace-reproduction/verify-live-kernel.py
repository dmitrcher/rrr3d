from pathlib import Path
import gzip,json,struct
root=Path.cwd();out=root/'outputs/combined-tire-v95';bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];value=lambda x:struct.unpack('<f',struct.pack('<I',x))[0];checks=[]
for rep in range(2):
 native=list(map(json.loads,gzip.open(root/f'outputs/wheel-skins-v94/native-next-brake245/force-marauder-wheel1-brake-245-{rep}-private.jsonl.gz','rt')));applied=[r for r in native if r.get('function')=='spring-solve-applied'];prepared=[r for r in native if r.get('function')=='spring-row-ready']
 tag=out/f'mac-candidate245/marauder-wheel1-brake-245-{rep}';solver=list(map(json.loads,open(str(tag)+'-solver.jsonl')));tire=list(map(json.loads,open(str(tag)+'-tire.jsonl')));results=[r for r in tire if r['event'].endswith('result')];inputs=[r for r in tire if r['event'].endswith('input')];mapped=[]
 convert=lambda r:[r['linear'][0],r['linear'][2],r['linear'][1],-r['angular'][0],-r['angular'][2],-r['angular'][1]]
 for iteration in range(1,6):
  springs=[r for r in solver if r['iteration']==iteration and r['phase']in('ordered-stop-after','spring-after')];assert len(springs)==4;mapped.extend([(r['velocity']+r['angular'],r['lambda'])for r in springs]);mapped.extend([(convert(r),r['lambda'])for r in results[(iteration-1)*8:iteration*8]])
 assert len(mapped)==len(applied)==60
 for index,((body,lam),n)in enumerate(zip(mapped,applied)):
  assert list(map(bits,body))==[n['solve_body0']['words'][k]for k in(0,1,2,4,5,6)],(rep,index,'body');assert lam==value(n['solve_row']['words'][28])or bits(lam)==n['solve_row']['words'][28],(rep,index,'lambda')
 for n,m in zip(prepared[4:],inputs[:8]):
  assert bits(m['maximum'])==n['constraint_row']['words'][29];assert bits(m['mass'])==n['constraint_row']['words'][11];assert bits(m['geometry_bias'])==n['constraint_row']['words'][7]
 checks.append({'repeat':rep,'continuous_body_checkpoints_exact':60,'body_float32_words_exact':360,'numeric_lambdas_exact':60,'all_eight_tire_bounds_and_masses_and_geometry_biases_exact':True})
report={'scope':'Physical245 Marauder moving-brake; four suspension rows and eight tire rows over five uninterrupted iterations. Corrected combined tire ellipse closes first clipped longitudinal divergence, with no reset/oracle or parameter fitting. Decimal private signed-zero lambda bit parity not claimed. Full608 regression acceptance pending.','checks':checks,'goal_complete':False};(out/'candidate-live-kernel-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Both repeats: all60 continuous body checkpoints/360floatwords,60numeric lambdas and8tire bounds exactNative')
