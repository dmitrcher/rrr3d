import argparse,collections,json,pathlib,struct
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);a=p.parse_args();root=a.measurements
def floats(words):return [struct.unpack('<f',struct.pack('<I',w))[0] for w in words]
def f32(v):return struct.unpack('<f',struct.pack('<f',v))[0]
audit={'scope':'First contact only; all 120 rows across eight fields exact, long-run parity remains incomplete','repeats':{}}
for repeat in range(2):
 records={};current=None
 for line in (root/f'calibration-plane-first-ordinary-run{repeat}-v38-mac.jsonl').open():
  row=json.loads(line)
  if row.get('event')=='calibration.begin':current=row['detail']
  if row.get('event')=='physics.native-contact-row' and row['sample']['step']==0:records.setdefault(current,[]).append(row['sample'])
 assert len(records)==4
 result={}
 for name,mac in records.items():
  native=collections.defaultdict(list)
  for line in (root/f'reference-native-point-{name}-geometry-run{repeat}-v37-windows.jsonl').open():
   row=json.loads(line)
   if 'function' in row:native[row['function']].append(row)
  count=native['geometry-dispatch-after'][0]['geometry_candidate_size'];assert len(mac)==5*count
  differences=[];exact=collections.Counter()
  for j,m in enumerate(mac):
   assert (m['iteration'],m['index'])==divmod(j,count)
   entry=native['solver-iteration-entry'][j];initialized=native['solver-initialize-exit'][j]
   row=floats(initialized['native_solver_row']['words']);entryrow=floats(entry['native_solver_row']['words']);body=floats(entry['native_body0']['words'])
   constraint=floats(native['prepared-row-before-response'][j%count]['input_constraint']['words'])
   fields={'effective_mass':row[20],'scaled_bias':row[24] if j<4*count else entryrow[24],'constraint_error':constraint[14],'lambda':entryrow[27],'lever_arm':row[4:7],'velocity':body[:3],'angular':body[4:7],'inverse_inertia':body[15:24]}
   for field,expected in fields.items():
    actual=m[field];actual=[f32(v) for v in actual] if isinstance(actual,list) else f32(actual)
    if actual==expected:exact[field]+=1
    else:differences.append({'row':j,'index':m['index'],'iteration':m['iteration'],'field':field,'mac':actual,'windows':expected})
  result[name]={'points':count,'rows':len(mac),'exact_fields':dict(exact),'differences':differences}
 assert all(not c['differences'] for c in result.values())
 for field in ('effective_mass','scaled_bias','constraint_error','lambda','lever_arm','velocity','angular'):
  assert result['frame-v25-long-p7-x1-y0']['exact_fields'][field]==40
 audit['repeats'][str(repeat)]=result
assert audit['repeats']['0']==audit['repeats']['1']
(root/'physics-v38-first-solver-reverified.json').write_text(json.dumps(audit,indent=2)+'\n')
for name,c in audit['repeats']['0'].items():print(name,c['rows'],c['exact_fields'],'remaining fields',len(c['differences']))
