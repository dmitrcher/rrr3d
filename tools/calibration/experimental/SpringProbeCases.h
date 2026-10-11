// Isolate suspension stiffness, damping and target without body impacts.
inline void appendSpringProbes(std::vector<Case>& s) {
 for (int type=0;type<6;++type) for(int speed=0;speed<3;++speed) for(int grip=0;grip<2;++grip) {
  std::ostringstream n;n<<"spring-probe-"<<type<<"-"<<speed<<"-"<<grip;
  Case c(n.str(),-20,type==4?2000:1200,10.75);c.steps=12;c.traction=grip!=0;
  c.velocity.z=speed==0?0:speed==1?-1:1;
  if(type==5)c.centerOfMass.x=.1;
  for(size_t w=0;w<c.wheels.size();++w) {
   if(type==1)c.wheels[w].spring=140000;
   if(type==2)c.wheels[w].damper=1000;
   if(type==3)c.wheels[w].target=.5;
  }
  s.push_back(c);
 }
}

inline void appendScalarSpringProbes(std::vector<Case>& suite) {
 for(int active=1;active<=4;active*=2) for(int type=0;type<3;++type) for(int speed=0;speed<3;++speed) {
  std::ostringstream name;name<<"scalar-spring-"<<active<<"-"<<type<<"-"<<speed;
  Case c(name.str(),-20,1200,10.75);c.steps=1;c.traction=false;
  c.velocity.z=speed==0?0:speed==1?-1:1;
  for(size_t index=0;index<c.wheels.size();++index) {
   c.wheels[index].position=V(0,0,index<(size_t)active?0:100);
   if(type==1)c.wheels[index].spring=140000;
   if(type==2)c.wheels[index].damper=1000;
  }
  suite.push_back(c);
 }
}

// History-dependent suspension contact transitions, isolated from torque and slip.
inline void appendLostSpringProbes(std::vector<Case>& suite) {
 const double heights[]={10.75,10.89,10.95,11.1};
 for(int mass=0;mass<2;++mass) for(int active=0;active<2;++active)
 for(int height=0;height<4;++height) for(int target=0;target<3;++target) for(int speed=0;speed<3;++speed) {
  std::ostringstream name;name<<"lost-spring-"<<mass<<"-"<<active<<"-"<<height<<"-"<<target<<"-"<<speed;
  Case c(name.str(),-20,mass?1200:300,heights[height]);c.steps=32;c.traction=false;
  c.velocity.z=speed==0?0:speed==1?1:3;
  for(size_t index=0;index<c.wheels.size();++index) {
   c.wheels[index].position=V(0,0,index<(size_t)(active?4:1)?0:100);
   c.wheels[index].target=target*.5;
  }
  suite.push_back(c);
 }
}

// Thin body stays above the ground: only the suspension compression stop acts.
inline void appendHardSpringProbes(std::vector<Case>& suite) {
 const double offsets[]={-.06,-.05,-.01,0,.01};
 for(int mass=0;mass<2;++mass) for(int active=0;active<2;++active)
 for(int height=0;height<5;++height) for(int spring=0;spring<3;++spring) for(int speed=0;speed<3;++speed) {
  std::ostringstream name;name<<"hard-spring-"<<mass<<"-"<<active<<"-"<<height<<"-"<<spring<<"-"<<speed;
  Case c(name.str(),-20,mass?1200:300,10.4+offsets[height]);c.steps=1;c.traction=false;c.halfExtents.z=.02;
  c.velocity.z=speed==0?0:speed==1?-1:-5;
  for(size_t index=0;index<c.wheels.size();++index) {
   c.wheels[index].position=V(0,0,index<(size_t)(active?4:1)?0:100);
   c.wheels[index].spring=spring==0?0:spring==1?35000:140000;
   c.wheels[index].damper=spring==0?0:4500;
  }
  suite.push_back(c);
 }
}
