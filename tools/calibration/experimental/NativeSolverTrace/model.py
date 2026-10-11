from pathlib import Path
import json,struct,sys
sys.path.insert(0,str(Path(__file__).resolve().parents[2]));import compare
f=lambda x:struct.unpack('f',struct.pack('f',x))[0]
fv=lambda a:[f(x) for x in a]
def words(r,k):return list(struct.unpack('32f',struct.pack('32I',*r[k]['words'])))
def cross(a,b):return [f(f(a[1]*b[2])-f(a[2]*b[1])),f(f(a[2]*b[0])-f(a[0]*b[2])),f(f(a[0]*b[1])-f(a[1]*b[0]))]
def normal(q):
 x,y,z,w=q
 return [f(1-f(f(f(2*y)*y)+f(f(2*z)*z))),f(f(f(2*x)*y)+f(f(2*z)*w)),f(f(f(2*x)*z)-f(f(2*y)*w))]
def reconstruct(trace,physics,name):
 r=[json.loads(x) for x in trace.read_text().splitlines()]
 it=[x for x in r if x.get('function')=='solver-iteration-entry'][:40];init=[x for x in r if x.get('function')=='solver-initialize-exit'][:8]
 assert len(it)==40 and len(init)==8
 observed=[words(x,'native_solver_row') for x in init];pre=[x for x in r if x.get('function')=='prepared-row-before-response'][:8]
 rows=[words(x,'prepared_row') for x in pre];body=words(it[0],'native_body0');v=body[:3];w=body[4:7];invM=body[11];invI=body[15:24];ls=[0.]*8;issues=[]
 masses_exact=0;biases_exact=0;dt=f(1/60);scale=f(.8)
 for i,rr in enumerate(rows):
  R=rr[4:7];N=normal(rr[16:20]);C=cross(R,N)
  response=[f(f(f(invI[a]*C[0])+f(invI[3+a]*C[1]))+f(invI[6+a]*C[2])) for a in range(3)]
  point=cross(response,R);term=[f(a*b) for a,b in zip(N,point)];angular=f(f(term[0]+term[1])+term[2])
  rr[20]=f(1/f(invM+angular))
  error=struct.unpack('34f',struct.pack('34I',*pre[i]['input_constraint']['words']))[14]
  raw=f(error*f(1/dt));assert raw==rr[24]
  rr[24]=f(f(raw*rr[20])*scale)
  masses_exact+=rr[20]==observed[i][20];biases_exact+=rr[24]==observed[i][24]
 assert masses_exact==biases_exact==8

 for j in range(40):
  i=j%8;expected=words(it[j],'native_body0');rr=rows[i];N=normal(rr[16:20]);R=rr[4:7]
  if v!=expected[:3] or w!=expected[4:7] or ls[i]!=words(it[j],'native_solver_row')[27]:
   issues.append({'entry':j,'v':v.copy(),'expected_v':expected[:3],'w':w.copy(),'expected_w':expected[4:7],'lambda':ls[i],'expected_lambda':words(it[j],'native_solver_row')[27]})
  pointVelocity=[f(a+b) for a,b in zip(v,cross(w,R))]
  dot=f(f(f(N[0]*pointVelocity[0])+f(N[2]*pointVelocity[2]))+f(N[1]*pointVelocity[1]))
  bias=rr[24] if j<32 else max(0.,rr[24])
  delta=f(f(rr[20]*f(-dot))-bias);total=f(ls[i]+delta)
  if total<0:delta=f(-ls[i]);total=0.
  ls[i]=total
  impulse=[f(n*delta) for n in N];torque=cross(R,impulse)
  v=[f(a+f(b*invM)) for a,b in zip(v,impulse)]
  dw=[f(f(f(invI[a]*torque[0])+f(invI[3+a]*torque[1]))+f(invI[6+a]*torque[2])) for a in range(3)]
  w=[f(a+b) for a,b in zip(w,dw)]
  if j==31:biased=v.copy()
 ticks=[json.loads(x)['sample']['tick'] for x in physics.open() if '"event":"calibration.local-points"' in x]
 # Trace JSON spacing is not part of the contract.
 if not ticks:ticks=[r['sample']['tick'] for r in map(json.loads,physics.read_text(encoding='utf-8-sig').splitlines()) if r['event']=='calibration.local-points']
 tick=min(ticks);case=compare.load(physics,'windows-physx-d3d9')[name];expected=case['rows'][tick]['velocity']
 return {'case':name,'first_tick':tick,'effective_masses_reconstructed_exact':masses_exact,'scaled_biases_reconstructed_exact':biases_exact,'entries_checked':40,'entries_exact':40-len(issues),'issues':issues,'final_velocity':v,'expected_velocity':fv(expected),'final_velocity_exact':v==fv(expected),'biased_velocity':biased,'lambda':ls}

if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser(description='Reconstruct Native arithmetic from recorded Native rows; diagnostics, not standalone Mac parity.')
 p.add_argument('--measurements',type=Path,required=True);p.add_argument('--out',type=Path,required=True);args=p.parse_args()
 report=[]
 for m in range(3):
  for h in range(2):
   for v in range(2):
    name=f'isolate-03-contact-m{m}-h{h}-v{v}-f1'
    pair=[]
    for repeat in range(2):
     prefix=f'reference-native-kernel-{name}-'
     pair.append(reconstruct(args.measurements/(prefix+f'geometry-run{repeat}-v31-windows.jsonl'),args.measurements/(prefix+f'physics-run{repeat}-v31-windows.jsonl'),name))
    assert pair[0]==pair[1],('Numerical kernel repeat differs',name)
    report.append(pair[0]);print(name,pair[0]['entries_exact'],'/40',pair[0]['final_velocity_exact'],flush=True)
 result={'scope':'Recorded contact geometry/constraint error and initial body; effective masses and scaled biases computed by the model, followed by forward arithmetic reconstruction without state resets or target values in the predictor. Diagnostics, not standalone Mac parity.','repeatable':True,'cases':report,'all_exact':all(x['entries_exact']==40 and x['final_velocity_exact'] for x in report),'goal_complete':False}
 args.out.write_text(json.dumps(result,indent=2)+'\n')
 raise SystemExit(0 if result['all_exact'] else 1)
