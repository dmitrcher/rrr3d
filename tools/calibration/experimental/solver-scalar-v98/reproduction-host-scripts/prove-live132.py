from pathlib import Path
import json,gzip,struct
root=Path.cwd();out=root/'outputs/solver-compose-v98';bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];reports=[]
for rep in range(2):
 n=list(map(json.loads,gzip.open(root/f'outputs/box-face-v97/native-solver-frame132/v97-solver-frame132-body-tilt-roll-{rep}-private.jsonl.gz','rt')))
 m=list(map(json.loads,open(out/f'mac-candidate132/body-tilt-roll-132-{rep}-physical.jsonl')));rows=[x['sample'] for x in m if x.get('event')=='physics.native-contact-row'];prepared=[x for x in n if x.get('function')=='solver-initialize-exit'];assert len(prepared)==len(rows)==10
 stage=next(x['sample'] for x in m if x.get('event')=='physics.native-material-stages')
 for a,b in zip(prepared,rows):
  body=a['native_body0']['words'];row=a['native_solver_row']['words'];assert body[:3]+body[4:7]==list(map(bits,b['velocity']+b['angular']));assert body[15:24]==list(map(bits,b['inverse_inertia']));assert body[11]==bits(b['inverse_mass']);assert row[4:7]==list(map(bits,b['lever_arm']));assert row[16:20]==list(map(bits,stage['solver_quaternion']));assert row[20]==bits(b['effective_mass']);assert row[24]==bits(b['scaled_bias'])
 reports.append({'repeat':rep,'uninterrupted_solver_body_checkpoints':10,'body_words_exact':60,'tensor_words_exact':90,'lever_words_exact':30,'solver_quaternion_words_exact':40,'inverse_mass_words_exact':10,'effective_mass_words_exact':10,'scaled_bias_words_exact':10})
(out/'live-132-proof.json').write_text(json.dumps({'scope':'V98 uninterrupted candidate same incoming body, ten contact-row checkpoints against original Windows132. No oracle states injected.','repeats':reports,'goal_complete':False},indent=2)+'\n');print('both repeats10 sequential body rows, masses/bias/quaternion/tensor/levers exactNative')
