from pathlib import Path
import gzip,json,struct,math,hashlib
root=Path.cwd();work=root/'work/spring-force-v115';out=root/'outputs/spring-force-v115'
f32=lambda v:struct.unpack('<f',struct.pack('<f',v))[0]
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
value=lambda v:struct.unpack('<f',struct.pack('<I',v))[0]
def rows(p):return list(map(json.loads,gzip.open(p,'rt')))
def st0(r):
 b=bytes(r['x87_register_bytes'][:10]);s=int.from_bytes(b[:8],'little');se=int.from_bytes(b[8:],'little');e=se&32767
 assert e not in [0,32767] or s==0
 return (-1 if se&32768 else 1)*math.ldexp(float(s),e-16383-63) if e else 0.0
fixtures=[]
for name,tick in {'spring-soft':46,'travel-short':43,'mass-heavy':43}.items():
 mac=rows(out/f'mac-force/{name}-{tick-1}-0-solver.jsonl.gz')
 repeat=[]
 for rep in range(2):
  native=rows(out/f'native-force/force-{name}-{tick}-{rep}-private.jsonl.gz')
  callback={r['stack']['words'][1]+64:r['callback_input_force']['words'][0] for r in native if r.get('function')=='callback-entry'}
  wheel_order=[r['esi'] for r in native if r.get('function')=='force-first-input'];assert len(wheel_order)==4 and set(wheel_order)==set(callback)
  current=[]
  for wheel,address in enumerate(wheel_order):
   by={r['function']:r for r in native if r.get('esi')==address and r.get('function','').startswith('force-')}
   first=by['force-first-input'];second=by['force-second-input'];assert first['x87_control']==second['x87_control']==0x37f
   spring=st0(first);stop=st0(second);inverse=value(first['force_frame']['words'][2]);assert bits(inverse)==second['force_frame']['words'][2]
   source_spring=next(r['lambda'] for r in reversed(mac) if r.get('wheel')==wheel and r.get('phase')=='spring-after')
   source_stop=next(r['lambda'] for r in reversed(mac) if r.get('wheel')==wheel and r.get('phase')=='ordered-stop-after')
   assert bits(spring)==bits(source_spring) and bits(stop)==bits(source_stop),(name,wheel,'solver inputs differ')
   first_force=by['force-first-stored']['force_wheel_tail']['words'][0];force=by['force-second-stored']['force_wheel_tail']['words'][0]
   assert bits(f32(spring*inverse))==first_force
   assert bits(f32(stop*inverse+value(first_force)))==force==callback[address]
   old=bits(f32(f32(spring+stop)*inverse))
   current.append({'case':name,'physical_tick':tick,'wheel':wheel,'spring':bits(spring),'stop':bits(stop),'inverse_step':bits(inverse),'first_stored_force':first_force,'force':force,'old_sum_first_force':old,'source_solver_inputs_exact':True,'native_callback_address_link_exact':True})
  repeat.append(current)
 assert repeat[0]==repeat[1];fixtures+=repeat[0]
report={'scope':'Original Windows force update RVA148883/148890/1488bd/1488ca; x87 CW37f; spring product stored to float before stop product is added and stored. Actor/wheel addresses linked directly to callback, no row permutation. All observed spring/stop impulses exactly match Source V114 preceding solve. All ten complete600-state Native histories and ten complete600-state Mac histories unchanged. Three cases have force differences; travel-long and radius-small instead first differ in reported suspension length and are excluded from the force proof.','fixtures':fixtures,'repeats_exact':True,'old_sum_first_mismatches':sum(r['old_sum_first_force']!=r['force'] for r in fixtures),'goal_complete':False}
(work/'native-force-proof.json').write_text(json.dumps(report,indent=2)+'\n')
(work/'native-force-fixtures.inc').write_text(''.join('{'+','.join(str(r[k])+'u' for k in ['spring','stop','inverse_step','force'])+'},\n' for r in fixtures))
print('12 actual Native stores proved; Source impulses exact; old sum-first mismatches:',report['old_sum_first_mismatches'])
