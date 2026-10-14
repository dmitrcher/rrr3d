from pathlib import Path
import json,gzip,struct
r=Path.cwd();bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def load(p):return [json.loads(line,parse_int=lambda v:-0.0 if v=='-0' else int(v)) for line in (gzip.open(p,'rt') if str(p).endswith('.gz') else p.open())]
reports=[]
for rep in range(2):
 native=load(r/f'outputs/static-pair-order-v108/native-next-border61/v108-next-border61-border-grounded-{rep}-private.jsonl.gz');mac=load(r/f'outputs/contact-bias-v109/mac-border61/border-grounded-61-{rep}-physical.jsonl');prepared=[x for x in native if x.get('function')=='solver-initialize-exit'];rows=[x['sample'] for x in mac if x.get('event')=='physics.native-contact-row'];cache=next(x['sample'] for x in mac if x.get('event')=='physics.native-cache-input');stage=next(x['sample'] for x in mac if x.get('event')=='physics.native-material-stages');print('rep',rep,'counts',len(prepared),len(rows),'cache',cache['canonical_dynamic_slot']);checks=[]
 for i,(n,m) in enumerate(zip(prepared,rows)):
  slot=0 if n['native_body0']['words'][11] else 1;body=n[f'native_body{slot}']['words'];row=n['native_solver_row']['words'];check={'row':i,'Native_dynamic_slot':slot,'body_incoming_equal':body[:3]+body[4:7]==list(map(bits,m['velocity']+m['angular'])),'tensor_equal':body[15:24]==list(map(bits,m['inverse_inertia'])),'lever_equal':row[(4 if slot==0 else 8):(7 if slot==0 else 11)]==list(map(bits,m['lever_arm'])),'mass_equal':row[20]==bits(m['effective_mass']),'bias_equal':row[24]==bits(m['scaled_bias']),'Native_mass_word':row[20],'Mac_mass_word':bits(m['effective_mass']),'Native_bias_word':row[24],'Mac_bias_word':bits(m['scaled_bias'])};checks.append(check)
  if i<3:print(check)
 reports.append({'repeat':rep,'checks':checks,'cache':cache,'material':stage})
(r/'outputs/contact-bias-v109/live-border61-proof.json').write_text(json.dumps({'scope':'Next preserved largest-error border-grounded first61 body contact kernel checkpoints, actual original Windows and actual current Mac. This is inspection only, not a candidate fix or a parity claim.','checks':reports,'goal_complete':False},indent=2)+'\n')
