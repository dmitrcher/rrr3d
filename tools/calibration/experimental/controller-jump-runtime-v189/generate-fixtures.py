from pathlib import Path
import json,struct,math,hashlib
R=Path.cwd();W=R/'work/controller-jump-runtime-v189';O=R/'outputs/controller-jump-runtime-v189';word=lambda v:struct.unpack('<I',struct.pack('<f',v))[0];value=lambda u:struct.unpack('<f',struct.pack('<I',u))[0];f=lambda v:value(word(v));cases=[]
def add(group,contact=False,mass=1000.,gravity=0.,velocity=(0.,0.,0.),pitch=4.,spring=0.,delta=f(1/60)):
 cases.append(dict(case=len(cases),group=group,contact=contact,mass_word=word(mass),gravity_word=word(gravity),velocity_words=list(map(word,velocity)),pitch_word=word(pitch),spring_word=word(spring),method_delta_word=word(delta)))
for contact in [False,True]:
 for speed in [0.,value(word(1.)-1),1.,value(word(1.)+1),5.]:
  for pitch in [0.,4.,-4.]:
   for spring in [0.,1.5]:
    for dt in [0.,f(1/60),value(word(1.5)-1),1.5,value(word(1.5)+1),2.]:add('scalar-gates',contact,velocity=(speed,0,0),pitch=pitch,spring=spring,delta=dt)
for mass in [.1,.333,1.,300.,1200.,4800.,1e6]:
 for gravity in [0.,-20.,3.]:
  for pitch in [0.,4.]:add('mass-gravity',mass=mass,gravity=gravity,velocity=(2,0,0),pitch=pitch)
for i in range(24):
 angle=(i+.25)*math.pi/12;x=f(math.cos(angle));y=f(math.sin(angle))
 for offset in [-2,-1,0,1,2]:
  uy=word(y);uy=max(0,uy+offset) # deterministic adjacent float words, signed inputs included.
  add('multi-axis-speed-boundary',velocity=(x,value(uy),0.))
for velocity in [(0,0,2),(0,0,-2),(0,2,0),(-2,0,0),(.6,.8,0),(.8,.6,0),(3,4,12),(-3,-4,-12)]:add('velocity-directions',velocity=velocity)
wire=O/'input.words';wire.write_text(''.join(' '.join(map(str,[c['case'],int(c['contact']),c['mass_word'],c['gravity_word'],*c['velocity_words'],c['pitch_word'],c['spring_word'],c['method_delta_word']]))+'\n' for c in cases));(O/'cases.json').write_text(json.dumps(dict(scope='One call to actual method followed by SDK/Jolt step1/60. Contact boolean is explicitly synthetic. Initial orientation identity, four free wheels, one car, no AI. No later state is fed back.',count=len(cases),input_sha256=hashlib.sha256(wire.read_bytes()).hexdigest(),cases=cases),indent=2)+'\n');print(len(cases),'deterministic isolated fixtures')
