import json,math,struct,sys
from pathlib import Path
F=lambda v:struct.unpack('f',struct.pack('f',v))[0]
def add(a,b):return [x+y for x,y in zip(a,b)]
def mul(a,b):return [x*y for x,y in zip(a,b)]
def scale(a,b):return [x*b for x in a]
def dot(a,b):return sum(x*y for x,y in zip(a,b))
def cross(a,b):return [a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]]
def sub(a,b):return [x-y for x,y in zip(a,b)]
def clamp(a,lo,hi):return max(lo,min(hi,a))
def solve(case,n,zero=True,contacts_first=False):
 d=case['iterations'][str(n)];inp=case['input'];m=case['actual']['mass'];im=1/m;ii=[1/v for v in case['actual']['inertia']];dt=F(1/60);height=F(inp['height']);center=[0,0,height];v=add(inp['initial_velocity'],[0,0,inp['gravity']*dt]);omega=[0,0,0]
 rows=[]
 for point,frame in zip(d['points'],d['frames']):
  r=sub(point['point0'],center);directions=[frame['after_axis0'],frame['after_axis1'],frame['after_axis2']]
  directions=[scale(a,1/math.sqrt(dot(a,a))) for a in directions]
  axes=[{'axis':a,'rx':cross(r,a),'eff':1/(im+dot(cross(r,a),mul(ii,cross(r,a)))),'lambda':0} for a in directions]
  rows.append({'r':r,'axes':axes,'bias':.8*point['constraint_error'][0]/dt,'mu':[0,point['friction1']]})
 springs=[]
 for w,res in zip(inp['fixture_parameters']['wheels'],d['result']['wheels']):
  if not res['contact']:continue
  length=res['suspension_length'];gamma=1/(dt*(w['damper']+dt*w['spring'])) if w['damper']+dt*w['spring'] else 0;eff=1/(im+gamma)
  springs.append({'soft':w['spring']>0 or w['damper']>0,'hard':length<0,'bh':.7*(length+.05)/dt,'bs':.7*dt*w['spring']*gamma*(length-w['travel']+.05-w['target']),'eff':eff,'r':eff*im,'lh':0,'ls':0})
 def wheels(final):
  nonlocal v
  for row in reversed(springs):
   if row['hard']:
    old=row['lh'];row['lh']=max(0,old-m*(v[2]+(0 if final else row['bh'])));v[2]+=(row['lh']-old)*im
   if row['soft']:
    old=row['ls'];row['ls']=max(0,row['r']*old-row['eff']*(v[2]+row['bs']*(1-row['r']**n if final else 1)));v[2]+=(row['ls']-old)*im
 def contacts(final):
  nonlocal v,omega
  for row in rows:
   speed=add(v,cross(omega,row['r']));proposed=[]
   for index,a in enumerate(row['axes']):proposed.append(a['lambda']-a['eff']*(dot(speed,a['axis'])+((max(0,row['bias']) if final else row['bias']) if index==0 else 0)))
   proposed[0]=max(0,proposed[0])
   for index in [1,2]:
    proposed[index]=row['axes'][index]['lambda'] if zero and proposed[0]==0 else clamp(proposed[index],-row['mu'][index-1]*proposed[0],row['mu'][index-1]*proposed[0])
   for a,new in zip(row['axes'],proposed):
    delta=new-a['lambda'];a['lambda']=new;v=add(v,scale(a['axis'],delta*im));omega=add(omega,scale(mul(ii,a['rx']),delta))
 for step in range(n):wheels(False);contacts(False)
 pose=add(center,scale(v,dt))
 if contacts_first:contacts(True);wheels(True)
 else:wheels(True);contacts(True)
 return pose,v
if __name__=='__main__':
 cases=json.load(open(sys.argv[1]))['cases'];rows=[]
 for c in cases:
  for n in [1,2,4,8]:
   p,v=solve(c,n);wrong=solve(c,n,zero=False);wrong_order=solve(c,n,contacts_first=True);ref=c['iterations'][str(n)]['result'];rows.append({'case':c['name'],'n':n,'P':math.dist(p,ref['position']),'V':math.dist(v,ref['velocity']),'wrongV':math.dist(wrong[1],ref['velocity']),'wrongOrderV':math.dist(wrong_order[1],ref['velocity'])})
 assert len(cases)==216 and len(rows)==864
 assert max(r['P'] for r in rows)<1e-6 and max(r['V'] for r in rows)<2e-6
 rejected=sum(r['wrongV']>.001 for r in rows);rejected_order=sum(r['wrongOrderV']>.001 for r in rows);assert rejected>=20 and rejected_order>=20
 print('864 independent Nx coupled first steps passed; max P/V',max(r['P'] for r in rows),max(r['V'] for r in rows),'rejected no-retention/contact-first',rejected,rejected_order)
