from pathlib import Path
import json,gzip,struct
root=Path.cwd();bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];fixtures=json.loads((root/'outputs/contact-birth-v102/same-input-birth-proof.json').read_text())['fixtures'];reports=[]
for rep in range(2):
 tag=f'slope-lateral-19-{rep}';order=list(map(json.loads,(root/f'outputs/contact-birth-v102/mac-order-candidate/{tag}-order.jsonl').open()));actual=next(x['ranks'] for x in order if x.get('step')==19);f=next(f for f in fixtures if f['case']=='slope-lateral' and f['repeat']==rep);expected={max(pair):i+1 for i,pair in enumerate(f['actual_Native_published_pairs'])};assert actual==[expected[3],0,expected[5],0]
 mac=list(map(json.loads,(root/f'outputs/contact-birth-v102/mac-spring19/slope-lateral/{tag}-solver.jsonl').open()));before=[x for x in mac if x['phase']=='spring-before'];after=[x for x in mac if x['phase']=='spring-after'];assert len(before)==len(after)==10
 native=list(map(json.loads,gzip.open(root/f'outputs/tire-angle-v100/native-next-spring19/{tag}-private.jsonl.gz','rt')));inputs=[x for x in native if x.get('function')=='spring-solve-input'];results=[x for x in native if x.get('function')=='spring-solve-result'];assert len(inputs)==len(results)==30
 # The Native six-row batch starts with its two springs, followed by tire rows.
 matched=[]
 for iteration in range(5):
  for j in range(2):
   n=inputs[6*iteration+j];r=results[6*iteration+j];m=before[2*iteration+j];a=after[2*iteration+j];body=n['solve_body0']['words'];row=n['solve_row']['words'];assert bits(m['mass'])==row[11];assert list(map(bits,m['velocity']))==body[:3]
   angular=list(map(bits,m['angular']));assert all(x==y or {x,y}=={0,2147483648} for x,y in zip(angular,body[4:7]))
   assert bits(a['lambda'])==r['solve_row']['words'][28]
   matched.append({'iteration':iteration+1,'wheel':m['wheel'],'incoming_velocity3_words_exact':True,'incoming_angular3_words_equal_except_signed_zero':True,'effective_mass_word_exact':True,'actual_Native_result_lambda_word_exact':True})
 assert [x['wheel'] for x in before[:2]]==[0,2]
 reports.append({'repeat':rep,'physical_tick':19,'actual_contact_ranks_exact':actual,'actual_first_spring_order':[0,2],'uninterrupted_spring_checkpoints':matched})
(root/'outputs/contact-birth-v102/live-birth-proof.json').write_text(json.dumps({'scope':'Fresh V102 candidate: actual Native initial contact ranks and live side-slope first19 spring sequence/checkpoints match. Five iterations/two springs per repeat. Native result breakpoint proves row lambda only, not post-update body. Full physical600step histories independently exact originalWindows.','repeats':reports,'goal_complete':False},indent=2)+'\n');print('Live Native contact ranks/order and10 uninterrupted spring input/mass/result-lambda checkpoints perrepeat exact')
