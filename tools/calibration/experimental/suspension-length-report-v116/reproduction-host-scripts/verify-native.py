from pathlib import Path
import gzip,json,struct,math,hashlib,collections
r=Path.cwd();w=r/'work/wheel-length-v116';out=r/'outputs/wheel-length-v116/native'
value=lambda u:struct.unpack('<f',struct.pack('<I',u))[0]
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
def st0(x):
 b=bytes(x['x87_register_bytes'][:10]);s=int.from_bytes(b[:8],'little');se=int.from_bytes(b[8:],'little');e=se&32767;assert e not in [0,32767] or s==0
 return (-1 if se&32768 else 1)*math.ldexp(float(s),e-16383-63) if e else 0.0
exe=r/'work/v47-package/bin/Windows/SpringProbe.exe';blob=exe.read_bytes();assert hashlib.sha256(blob).hexdigest()=='3935cb983910290e2ff844bb437286d140d0fafa6cfbcbfbcbfd0f07b6208262'
pe=struct.unpack_from('<I',blob,60)[0];imagebase=struct.unpack_from('<I',blob,pe+24+28)[0]
names=['travel-long','radius-small'];mac={n:[] for n in names}
with open(r/'outputs/spring-force-v115/broad/candidate-run0.jsonl') as f:
 for x in map(json.loads,f):
  if x.get('event')=='calibration.sample' and x['detail'] in mac:mac[x['detail']].append(x['sample'])
assert all(len(v)==600 for v in mac.values())
summary={};fixtures=[]
for name in names:
 runs=[]
 for rep in range(2):
  rows=list(map(json.loads,gzip.open(out/f'{name}-{rep}-private.jsonl.gz','rt')))
  calls={x['hit']:x for x in rows if x.get('function')=='wheel-contact-getter-call'}
  returns=[x for x in rows if x.get('function')=='wheel-contact-getter-return' and x['stack']['words'][0]==imagebase+0xf79f]
  assert len(returns)==2400 and all(v==4 for v in collections.Counter(x['sample_tick_v71'] for x in returns).values())
  radius=[x for x in rows if x.get('function')=='radius-getter-return'];radius_by={}
  for x in radius:
   assert bits(st0(x))==x['radius_word']['words'][0]
   radius_by.setdefault((x['sample_tick_v71'],x['ecx']),[]).append(x)
  native=[x['sample'] for x in map(json.loads,gzip.open(out/f'{name}-{rep}-physical.jsonl.gz','rt')) if x.get('event')=='calibration.sample'];assert len(native)==600
  current=[];owners={}
  for i,x in enumerate(returns):
   tick,wheel=divmod(i,4);assert x['sample_tick_v71']==tick;call=calls[x['hit']];assert call['sample_tick_v71']==tick and call['ecx']==x['ecx']
   owners.setdefault(wheel,x['ecx']);assert owners[wheel]==x['ecx'];assert len(set(owners.values()))==len(owners)
   has_contact=bool(x['returned_shape']);assert has_contact==native[tick]['wheels'][wheel]['contact']==mac[name][tick]['wheels'][wheel]['contact']
   radius_word=call['radius_word']['words'][0];position=x['contact_output']['words'][18] if has_contact else None
   if has_contact:
    actual_radius=[z for z in radius_by[(tick,x['ecx'])] if z['stack']['words'][0]==imagebase+0xf890];assert len(actual_radius)==1 and actual_radius[0]['radius_word']['words'][0]==radius_word
   derived=value(position)-value(radius_word) if has_contact else 0.0
   printed=float(format(derived,'.9g'));assert printed==native[tick]['wheels'][wheel]['suspension_length'],(name,tick,wheel,'Native report did not reproduce')
   assert bits(derived)==bits(mac[name][tick]['wheels'][wheel]['suspension_length']),(name,tick,wheel,'Correctly rounded Native length differs from Source')
   current.append({'case':name,'tick':tick,'wheel':wheel,'has_contact':has_contact,'contact_position_word':position,'radius_word':radius_word,'derived_double':derived,'original_reported_decimal':printed,'correctly_rounded_native_word':bits(derived),'mac_reported_word':bits(mac[name][tick]['wheels'][wheel]['suspension_length']),'reparsed_native_decimal_word':bits(printed)})
  runs.append(current)
 assert runs[0]==runs[1];fixtures+=runs[0];summary[name]={'states_per_repeat':600,'wheel_lengths_per_repeat':2400,'repeats_exact':True,'native_api_radius_result_word_exact':True,'native_double_report_reproduced':True,'correctly_rounded_native_lengths_match_mac':True,'reparsed_decimal_false_differences':sum(x['correctly_rounded_native_word']!=x['reparsed_native_decimal_word'] for x in runs[0])}
with gzip.open(w/'native-api-fixtures.jsonl.gz','wt') as f:
 for x in fixtures:f.write(json.dumps(x,separators=(',',':'))+'\n')
with gzip.open(r/'work/normal-force-v84/native-probe.disasm.txt.gz','rt') as f:
 lines=[x for x in f if '  40f872'<=x[:10]<'  40f8ad']
(w/'original-report-subtraction.disasm.txt').write_text(''.join(lines))
report={'scope':'All actual Native public sample getter results from two600-step cases, two repeats. contactPosition word18 (NxWheelContactData+48), radius getter RVA192f0/192f6. Sample getter caller RVAf79f and radius caller RVAf890, bound to the immutable EXE PE image base. Original sample subtracts in x87 and stores double at RVAf89c, not float. Nine-significant-digit text loses midpoint rounding information; Source stores float before reporting. Every correctly rounded actual Native derived length already matches Mac V115. No reference or solver modification.','cases':summary,'actual_native_lengths_checked_per_repeat':4800,'all_native_public_histories_preserved':True,'source_raw_distance_and_radius_words_observed':False,'source_same_input_causal_proof_complete':False,'source_physics_changed':False,'goal_complete':False}
(w/'native-report-proof.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(summary,indent=2))
