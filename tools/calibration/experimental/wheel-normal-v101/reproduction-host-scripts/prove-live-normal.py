from pathlib import Path
import json,gzip,struct
root=Path.cwd();bits=lambda x:struct.unpack('<I',struct.pack('<f',x))[0];native=json.loads((root/'outputs/wheel-normal-v101/native-api/report.json').read_text());reports=[]
for name,tick in [('slope-longitudinal',19),('slope-lateral',19),('settle',25)]:
 for rep in range(2):
  mac=list(map(json.loads,open(root/f'outputs/wheel-normal-v101/mac-candidate-input/{name}-{tick}-{rep}-normal.jsonl')))
  for m in mac:
   n=next(x['sample'] for x in native['normal_observations'] if x['case']==name and x['event']=='calibration.local-triangle-normal-v101' and x['sample']['triangle']==m['triangle']);assert m['vertices_words']==n['vertices_words'] and m['normal_words']==n['calculation_words'][5:8]
  reports.append({'case':name,'repeat':rep,'physical_tick':tick,'actual_source_vertex_words_exact':9*len(mac),'actual_Native_normal_words_exact':3*len(mac),'contacts':len(mac)})
bindings=[]
for name in ['slope-longitudinal','slope-lateral']:
 for rep in range(2):
  rows=list(map(json.loads,gzip.open(root/f'outputs/tire-angle-v100/native-next-spring19/{name}-19-{rep}-private.jsonl.gz','rt')));rows=[x for x in rows if x.get('function')=='spring-row-ready'][:2];prep=list(map(json.loads,open(root/f'outputs/wheel-normal-v101/mac-spring-candidate19/{name}/{name}-19-{rep}-prep.jsonl')))
  for n in rows:
   obj=n['spring_object']['words'];row=n['constraint_row']['words'];m=next(x for x in prep if list(map(bits,x['world_point']))==obj[12:15]);assert list(map(bits,m['world_axis']))==obj[15:18];assert list(map(bits,m['cached_local_point']))==obj[9:12];assert list(map(bits,m['spring']['jacobian']))==row[12:15];assert list(map(bits,m['spring']['inverse_jacobian']))==row[20:23];assert bits(m['spring']['mass'])==row[11];bindings.append({'case':name,'repeat':rep,'wheel':m['wheel'],'normal3_localpoint3_worldpoint3_Jacobian3_inverseJacobian3_mass1_words_exact':True})
report={'scope':'Actual candidate source vertices and normals exact independently captured Windows cooked/API/ray data. Both slope physical19 spring axes/local/worldpoints/Jacobian/inverseJacobian/currentmass exact on8active constraints. Downstream whole slope histories remain open; solver order/bias are not certified here.','normal_repeats':reports,'spring_bindings':bindings,'goal_complete':False};(root/'outputs/wheel-normal-v101/live-normal-proof.json').write_text(json.dumps(report,indent=2)+'\n');print('Actual Native normals and8spring property bindings exact; whole slope parity still open',flush=True)
