import argparse,json,pathlib,struct,subprocess
p=argparse.ArgumentParser();p.add_argument('--mac',type=pathlib.Path,required=True);p.add_argument('--native',type=pathlib.Path,required=True);p.add_argument('--probe',type=pathlib.Path,required=True);p.add_argument('--out',type=pathlib.Path,required=True);a=p.parse_args()
f=lambda words:[struct.unpack('<f',struct.pack('<I',w))[0] for w in words]
f32=lambda v:struct.unpack('<f',struct.pack('<f',v))[0]
audit={'scope':'Independent Mac body-local tensor transformed using recorded native actor quaternion; diagnostic formula validation only','fixtures':{}}
for repeat in range(2):
 records={};current=None
 for line in (a.mac/f'calibration-inertia-first-ordinary-run{repeat}-v37-mac.jsonl').open():
  row=json.loads(line)
  if row.get('event')=='calibration.begin':current=row['detail'];records[current]={}
  if row.get('event')=='physics.source-local-inertia':records[current]['local']=list(map(f32,row['sample']['local_inverse_inertia']))
  if row.get('event')=='physics.native-contact-row' and row['sample']['index']==0 and row['sample']['iteration']==0:records[current]['world']=list(map(f32,row['sample']['inverse_inertia']))
 assert len(records)==4
 for name,mac in records.items():
  native={}
  for line in (a.native/f'reference-native-point-{name}-geometry-run{repeat}-v37-windows.jsonl').open():
   row=json.loads(line);func=row.get('function')
   if func and func not in native:native[func]=row
  q=f(native['material-inputs-ready']['material_stack']['words'][36:40])
  expected=f(native['solver-iteration-entry']['native_body0']['words'][15:24])
  local=[mac['local'][col*3+row] for row in range(3) for col in range(3)]
  result=subprocess.run([str(a.probe.resolve())],input=' '.join(map(str,q+local))+'\n',text=True,capture_output=True,check=True)
  actual=list(map(f32,map(float,result.stdout.split())))
  assert actual==expected,(name,actual,expected)
  assert mac['world']==expected,(name,'Independent Mac contact tensor differs')
  audit['fixtures'][name+f'-run{repeat}']={'entries':9,'formula_exact':True,'independent_mac_contact_tensor_exact':True,'native_actor_quaternion':q,'mac_local_tensor':mac['local']}
a.out.write_text(json.dumps(audit,indent=2)+'\n');print('All 72 tensor entries exact across four fixtures and two repeats.')
