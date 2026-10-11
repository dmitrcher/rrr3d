// C++98. Force-level isolation ladder; no AI or GameCar gameplay commands.
inline Case isolatedBody(const std::string& name, double mass) {
 Case c(name,0,mass,100,false);c.steps=120;c.action="none";
 c.bodyFriction=0;c.bodyLinearDamping=c.bodyAngularDamping=0;
 c.wheelContactsEnabled=false;
 c.bodyContactFrictionEnabled=false;
 c.restBrake=c.brakeLimit=c.tireSpring=0;c.traction=false;
 c.extraGravity=false;c.airPitch=c.clampRoll=c.clampPitch=0;
 c.angularDamping=V(1,1,1);c.angular=V();
 for(size_t i=0;i<c.wheels.size();++i) {
  Wheel& w=c.wheels[i];w.spring=w.damper=0;
  w.longitudinal.stiffness=w.lateral.stiffness=0;
  // Retain the game's required axle definition. Applied motor/brake torque
  // is zero throughout stages 00/01; wheel collisions and tire curves are off.
  w.driven=i<2;w.steering=false;
 }
 return c;
}
inline void appendIsolationProbes(std::vector<Case>& suite) {
 const double masses[]={300,1200,2400};
 const double amplitudes[]={-.2,.2,-3,3,-10,10};
 // Stage 00: stationary and constant-velocity free flight, no forces.
 for(int mass=0;mass<3;++mass)for(int velocity=0;velocity<3;++velocity) {
  std::ostringstream n;n<<"isolate-00-free-m"<<mass<<"-v"<<velocity;
  Case c=isolatedBody(n.str(),masses[mass]);
  c.velocity=velocity==0?V():velocity==1?V(2,-3,1):V(-2,3,-1);
  suite.push_back(c);
 }
 // Stage 01: add only a constant linear acceleration, axis by axis.
 for(int mass=0;mass<3;++mass)for(int axis=0;axis<3;++axis)
 for(int value=0;value<6;++value)for(int velocity=0;velocity<2;++velocity) {
  std::ostringstream n;n<<"isolate-01-accel-m"<<mass<<"-a"<<axis<<"-k"<<value<<"-v"<<velocity;
  Case c=isolatedBody(n.str(),masses[mass]);c.action="isolation-acceleration";
  c.velocity=velocity?V(2,-3,1):V();
  c.commandedAcceleration=axis==0?V(amplitudes[value],0,0):axis==1?V(0,amplitudes[value],0):V(0,0,amplitudes[value]);
  suite.push_back(c);
 }
 // Stage 02: add gravity, including acceleration-plus-gravity combinations.
 const double gravities[]={-9.81,-20};
 for(int mass=0;mass<3;++mass)for(int gravity=0;gravity<2;++gravity)
 for(int velocity=0;velocity<2;++velocity)for(int acceleration=0;acceleration<4;++acceleration) {
  std::ostringstream n;n<<"isolate-02-gravity-m"<<mass<<"-g"<<gravity<<"-v"<<velocity<<"-a"<<acceleration;
  Case c=isolatedBody(n.str(),masses[mass]);c.gravity=gravities[gravity];
  c.velocity=velocity?V(2,-3,1):V();
  if(acceleration){c.action="isolation-acceleration";c.commandedAcceleration=acceleration==1?V(3,0,0):acceleration==2?V(0,-.2,0):V(0,0,.2);}
  suite.push_back(c);
 }

 // Stage 03: enable only chassis/floor contact, no wheel rays or friction.
 // Ground-off companions use identical height, DOFs and incoming velocity.
 const double heights[]={12,10.31};
 for(int mass=0;mass<3;++mass)for(int height=0;height<2;++height)
 for(int velocity=0;velocity<2;++velocity)for(int ground=0;ground<2;++ground) {
  std::ostringstream n;n<<"isolate-03-contact-m"<<mass<<"-h"<<height<<"-v"<<velocity<<"-f"<<ground;
  Case c=isolatedBody(n.str(),masses[mass]);c.gravity=-20;c.height=heights[height];
  c.ground=ground!=0;c.fixedOrientation=true;c.velocity.z=velocity?-1:0;
  suite.push_back(c);
 }

 // Contact-frame diagnostics: vary pitch and plane slopes without adding forces.
 const double pitches[]={-1.57,-.8,-.25,-.01,-.0001,0,.0001,.01,.25,.8,1.57,3.14};
 const double slopes[]={-.2,0,.2};
 for(int pitch=0;pitch<12;++pitch)for(int slope=0;slope<3;++slope)for(int y=0;y<2;++y) {
  std::ostringstream n;n<<"frame-v23-p"<<pitch<<"-x"<<slope<<"-y"<<y;
  Case c=isolatedBody(n.str(),1200);c.ground=true;c.gravity=-20;c.steps=8;
  c.fixedOrientation=true;c.initialPitch=pitches[pitch];c.slopeX=slopes[slope];c.slopeY=y?.1:0;
  c.height=10+1.5*std::abs(std::sin(c.initialPitch))+.3*std::abs(std::cos(c.initialPitch))-.07;
  suite.push_back(c);
  // Eight steps can stop before impact on a nearly parallel slope. Keep the
  // diagnostic case, and independently certify impact/response over 120 steps.
  std::ostringstream longName;longName<<"frame-v25-long-p"<<pitch<<"-x"<<slope<<"-y"<<y;
  c.name=c.action=longName.str();c.steps=120;suite.push_back(c);
 }

 // Frozen contact pose coverage: shape origin and center of mass are independent.
 // Keep AI, wheel rays, tire forces, motor/brake torque and damping disabled.
 for(int type=0;type<6;++type) {
  std::ostringstream name;name<<"pose-v42-frozen-"<<type;
  Case c=isolatedBody(name.str(),1200);c.steps=120;c.ground=true;c.gravity=-20;
  c.fixedOrientation=true;c.initialPitch=.25;c.slopeY=.1;
  if(type==1)c.centerOfMass=V(.5,0,0);
  if(type==2)c.centerOfMass=V(0,0,-.5);
  if(type>=3)c.shapePosition=V(.2,0,.1);
  if(type==4)c.centerOfMass=V(.5,0,-.5);
  if(type==5){c.shapePosition=V(.2,.1,.1);c.centerOfMass=V(.2,-.3,-.4);}
  c.height=10+1.5*std::abs(std::sin(c.initialPitch))+.3*std::abs(std::cos(c.initialPitch))-.07;
  // Offset the actor so the geometric box has the same initial floor penetration.
  c.height+=c.shapePosition.x*std::sin(c.initialPitch)-c.shapePosition.z*std::cos(c.initialPitch)+c.slopeY*c.shapePosition.y;
  suite.push_back(c);
 }

}
