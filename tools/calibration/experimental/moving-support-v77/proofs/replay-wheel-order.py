from pathlib import Path
import json,gzip,struct
here=Path(__file__).resolve().parent;root=here.parent
f=lambda w:struct.unpack('<f',struct.pack('<I',w))[0]
F=lambda v:struct.unpack('<f',struct.pack('<f',v))[0]
bits=lambda v:struct.unpack('<I',struct.pack('<f',v))[0]
add=lambda a,b:F(a+b);sub=lambda a,b:F(a-b);mul=lambda a,b:F(a*b);div=lambda a,b:F(a/b)
rows=list(map(json.loads,gzip.open(root/'captures/kernel-creation-pair-stock-pole-1-28-0-private.jsonl.gz','rt')))
inputs=[x for x in rows if x.get('function')=='spring-solve-input'];applied=[x for x in rows if x.get('function')=='spring-solve-applied'];assert len(inputs)==len(applied)==60
initial=[list(map(f,x['solve_row']['words'][4:]))for x in inputs[:12]]
def scalar_jv(r,b0,b1):
 r0=add(mul(r[10],b0[6]),mul(r[8],b0[4]));r0=add(r0,mul(r[2],b0[2]));r0=add(r0,mul(r[1],b0[1]));r0=add(r0,mul(r[9],b0[5]));r0=add(r0,mul(r[0],b0[0]))
 r1=add(mul(r[13],b1[5]),mul(r[14],b1[6]));r1=add(r1,mul(r[12],b1[4]));r1=add(r1,mul(r[6],b1[2]));r1=add(r1,mul(r[5],b1[1]));r1=add(r1,mul(r[4],b1[0]));return sub(r0,r1)
def replay(order):
 rr=[r.copy()for r in initial];b0=list(map(f,inputs[0]['solve_body0']['words']));b1=list(map(f,inputs[0]['solve_body1']['words']));history=[]
 for iteration in range(5):
  if iteration==4:
   for r in rr:
    r[3]=0.0
    if abs(r[24])>f(0x2edbe6ff):r[15]=mul(div(r[23],r[24]),r[15])
  for index in order:
   r=rr[index];jv=scalar_jv(r,b0,b1);velocity=mul(sub(r[27],jv),r[7]);position=mul(r[3],r[11]);unbiased=add(r[23],mul(sub(velocity,mul(r[15],r[23])),r[19]));delta=mul(sub(sub(velocity,position),mul(r[15],r[24])),r[19]);biased=add(r[24],delta)
   if biased>r[25]:biased=r[25];delta=sub(biased,r[24])
   elif r[26]>biased:biased=r[26];delta=sub(biased,r[24])
   r[23:25]=unbiased,biased
   if delta!=0:
    for a in range(3):
     b0[a]=add(b0[a],mul(r[a],mul(b0[11],delta)));b1[a]=sub(b1[a],mul(r[a+4],mul(b1[11],delta)))
     b0[a+4]=add(b0[a+4],mul(r[a+16],delta));b1[a+4]=sub(b1[a+4],mul(r[a+20],delta))
   history.append({'iteration':iteration+1,'row':index,'delta':delta,'lambda':biased,'body0':[b0[a]for a in (0,1,2,4,5,6)],'body1':[b1[a]for a in (0,1,2,4,5,6)]})
 return history
native=replay(list(range(12)));mismatches=[]
for n,(got,expected) in enumerate(zip(native,applied)):
 for body in ('body0','body1'):
  want=[expected['solve_'+body]['words'][a]for a in (0,1,2,4,5,6)]
  if list(map(bits,got[body]))!=want:mismatches.append({'record':n,'field':body})
assert not mismatches,mismatches[:10]
# Explicit diagnostic intervention only: replay measured Mac spring traversal.
# This permutation is not an implementation rule and is never installed in the game.
mac=replay([2,0,3,1]+list(range(4,12)))
actual=[x for x in map(json.loads,open(root/'captures/mac-onset-wheel.jsonl'))if x['phase']=='spring-after']
assert len(actual)==20
mac_springs=[x for x in mac if x['row']<4];live_checks=[]
for n,(m,x) in enumerate(zip(mac_springs,actual)):
 live_checks.append({'record':n,'lambda_exact':bits(m['lambda'])==bits(x['lambda']),'velocity_exact':list(map(bits,m['body0'][:3]))==list(map(bits,x['velocity'])),'angular_numeric_exact':m['body0'][3:]==list(map(F,x['angular']))})
assert all(all(x[k] for k in ('lambda_exact','velocity_exact','angular_numeric_exact')) for x in live_checks),live_checks
report={'original_continuous_replay':{'physical_tick':28,'body_transitions':60,'body_words':720,'all_raw_words_exact':True,'reset_body_per_row':False,'original_final_softness_rule_applied':True},'measured_mac_order_intervention':{'spring_order_mac_indices':[3,2,1,0],'spring_order_original_indices':[2,0,3,1],'diagnostic_only_never_installed':True,'matches_live_mac':live_checks,'windows_final_body0':native[-1]['body0'],'reordered_final_body0':mac[-1]['body0'],'first_four_windows':native[:4],'first_four_reordered':mac[:4]},'limitations':['Rows, initial bodies and traversal inputs are measured original values. This isolates solver/order causality; it does not establish a general original queue construction rule or full Mac parity.'],'goal_complete':False}
print('Native continuous replay:720/720 raw body words exact. Live Mac intervention checks:',live_checks)
