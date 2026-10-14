from pathlib import Path
import json,gzip,struct
r=Path.cwd();bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];swap=lambda v:[v[0],v[2],v[1]]
def load(p):return [json.loads(line,parse_int=lambda v:-0.0 if v=='-0' else int(v)) for line in (gzip.open(p,'rt') if str(p).endswith('.gz') else p.open())]
reports=[]
for tick in [13,224]:
 for rep in range(2):
  n=[x for x in load(r/f'outputs/static-box-query-v107/native-next-curb224/curb-side-230-{rep}-private.jsonl.gz') if x.get('sample_tick_v71')==tick];ni=[x for x in n if x.get('function')=='spring-solve-input'];nr=[x for x in n if x.get('function')=='spring-solve-result'];p=r/'outputs/static-box-query-v107/mac-curb-checkpoints';tag=f'curb-side-{tick}-{rep}';prep=load(p/(tag+'-prep.jsonl'));s=load(p/(tag+'-solver.jsonl'));si=[x for x in s if x['phase']=='spring-before'];sr=[x for x in s if x['phase']=='spring-after'];ti=[x for x in load(p/(tag+'-tire.jsonl')) if x['event'].endswith('-input')];count=len(si)//5;entries=[]
  print('tick',tick,'rep',rep,'NativeRows',len(ni),'MacSpring',len(si),'MacTire',len(ti))
  for i in range(count):
   m=si[i];x=ni[i];row=x['solve_row']['words'];pr=next(z for z in prep if z['wheel']==m['wheel']);v=list(map(bits,m['velocity']));a=list(map(bits,m['angular']));entry={'wheel':m['wheel'],'mass_exact':bits(m['mass'])==row[11],'linear_exact':v==x['solve_body0']['words'][:3],'angular_exact':a==x['solve_body0']['words'][4:7],'lambda_result_exact':bits(sr[i]['lambda'])==nr[i]['solve_row']['words'][28],'Native_jac':row[12:15],'Mac_jac':list(map(bits,pr['spring']['jacobian'])),'Native_invjac':row[20:23],'Mac_invjac':list(map(bits,pr['spring']['inverse_jacobian']))}
   entries.append(entry);print(entry)
  for i in range(2):
   m=ti[i];x=ni[count+i];row=x['solve_row']['words'];print('tire',i,m['event'],'mass',bits(m['mass'])==row[11],'axis',list(map(bits,[-v for v in swap(m['axis'])]))==row[8:11],'jac',list(map(bits,swap(m['r_cross_axis'])))==row[12:15],'invjac',list(map(bits,swap(m['inv_i_r_cross_axis'])))==row[20:23],'linear',list(map(bits,swap(m['linear'])))==x['solve_body0']['words'][:3])
  reports.append({'tick':tick,'repeat':rep,'first_springs':entries})
(r/'outputs/static-box-query-v107/curb-solver-inspection.json').write_text(json.dumps({'scope':'Actual original Native and actual live V107 spring checkpoints. Signed zero in raw Jacobian kept explicit. Do not infer geometry equality only from equal mass.','checks':reports,'goal_complete':False},indent=2)+'\n')
