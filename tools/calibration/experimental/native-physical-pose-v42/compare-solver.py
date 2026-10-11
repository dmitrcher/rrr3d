import collections,json,pathlib,struct,argparse
p=argparse.ArgumentParser();p.add_argument('--measurements',type=pathlib.Path,required=True);p.add_argument('--step',type=int,required=True);p.add_argument('--reference-dir',type=pathlib.Path,required=True);a=p.parse_args()
root=pathlib.Path.cwd();out=a.measurements
def floats(words):return [struct.unpack('<f',struct.pack('<I',w))[0] for w in words]
def f32(v):return struct.unpack('<f',struct.pack('<f',v))[0]
audit={'scope':'Target simulation step of independently simulated force-isolated fixtures','repeats':{}}
for repeat in range(2):
 records={};current=None
 for line in (out/f'calibration-depth-ordinary-run{repeat}-v39-mac.jsonl').open():
  r=json.loads(line)
  if r.get('event')=='calibration.begin':current=r['detail']
  if r.get('event')=='physics.native-contact-row' and r['sample']['step']==a.step:records.setdefault(current,[]).append(r['sample'])
 result={}
 for name,mac in records.items():
  groups=[];group=None
  for line in (a.reference_dir/f'reference-native-point-{name}-geometry-run{repeat}-v37-windows.jsonl').open():
   r=json.loads(line);fn=r.get('function')
   if fn=='geometry-dispatch-after':
    if group:groups.append(group)
    group={'count':r['geometry_candidate_size'],'events':collections.defaultdict(list)}
   elif group:group['events'][fn].append(r)
   if len(groups)>=a.step+1:break
  group=groups[a.step];count=group['count'];native=group['events'];assert len(mac)==5*count,(name,len(mac),count)
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
 audit['repeats'][str(repeat)]=result
assert audit['repeats']['0']==audit['repeats']['1'],'Diagnostic repeats differ'
(out/'solver-comparison.json').write_text(json.dumps(audit,indent=2)+'\n')
for n,c in audit['repeats']['0'].items():print(n,'rows',c['rows'],'differences',len(c['differences']),'first',c['differences'][:5])
