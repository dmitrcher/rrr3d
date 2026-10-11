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

}
