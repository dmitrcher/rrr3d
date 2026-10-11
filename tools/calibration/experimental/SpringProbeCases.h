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

// Coupled floor/body contacts and collinear wheel suspension; no tire friction.
inline void appendCoupledFloorProbes(std::vector<Case>& suite) {
 const double heights[]={10.6,10.59,10.55,10.4};
 for(int mass=0;mass<2;++mass) for(int active=0;active<3;++active)
 for(int height=0;height<4;++height) for(int spring=0;spring<3;++spring) for(int speed=0;speed<3;++speed) {
  std::ostringstream name;name<<"coupled-floor-"<<mass<<"-"<<active<<"-"<<height<<"-"<<spring<<"-"<<speed;
  Case c(name.str(),-20,mass?1200:300,heights[height]);c.steps=6;c.traction=false;c.halfExtents.z=.6;c.fixedOrientation=true;
  c.velocity.z=speed==0?0:speed==1?-1:-5;
  for(size_t index=0;index<c.wheels.size();++index) {
   c.wheels[index].position=V(0,0,index<(size_t)(active==0?0:active==1?1:4)?0:100);
   c.wheels[index].spring=spring==0?0:spring==1?35000:140000;
   c.wheels[index].damper=spring==0?0:4500;
  }
  suite.push_back(c);
 }
}

// Independently vary wheel friction with no drive/brake command; body is thin.
inline void appendTireSlipProbes(std::vector<Case>& suite) {
 const double speeds[]={.2,2,10};
 for(int mass=0;mass<2;++mass) for(int layout=0;layout<2;++layout)
 for(int active=0;active<2;++active) for(int locked=0;locked<2;++locked)
 for(int axis=0;axis<3;++axis) for(int speed=0;speed<3;++speed)
 for(int traction=0;traction<2;++traction) for(int spring=0;spring<2;++spring) {
  std::ostringstream name;name<<"tire-slip-"<<mass<<"-"<<layout<<"-"<<active<<"-"<<locked<<"-"<<axis<<"-"<<speed<<"-"<<traction<<"-"<<spring;
  Case c(name.str(),-20,mass?1200:300,10.75);c.steps=24;c.traction=traction!=0;c.halfExtents.z=.02;c.fixedOrientation=locked!=0;
  c.velocity=V(axis==1?0:speeds[speed],axis==0?0:speeds[speed],0);
  for(size_t index=0;index<c.wheels.size();++index) {
   if(layout==0)c.wheels[index].position.x=c.wheels[index].position.y=0;
   if(index>=(size_t)(active?4:1))c.wheels[index].position.z=100;
   c.wheels[index].spring=spring?140000:35000;
  }
  suite.push_back(c);
 }
}

inline void appendHardTireProbes(std::vector<Case>& suite) {
 const double heights[]={10.34,10.39,10.4};
 for(int mass=0;mass<2;++mass) for(int active=0;active<2;++active)
 for(int height=0;height<3;++height) for(int spring=0;spring<3;++spring) {
  std::ostringstream name;name<<"hard-tire-"<<mass<<"-"<<active<<"-"<<height<<"-"<<spring;
  Case c(name.str(),-20,mass?1200:300,heights[height]);c.steps=6;c.traction=true;c.halfExtents.z=.02;c.fixedOrientation=true;c.velocity=V(0,2,0);
  for(size_t index=0;index<c.wheels.size();++index) {
   c.wheels[index].position=V(0,0,index<(size_t)(active?4:1)?0:100);
   c.wheels[index].spring=spring==0?0:spring==1?35000:140000;c.wheels[index].damper=spring==0?0:4500;
  }
  suite.push_back(c);
 }
}

inline void appendZeroLoadTireProbes(std::vector<Case>& suite) {
 for(int mass=0;mass<2;++mass) for(int active=0;active<2;++active) for(int spring=0;spring<2;++spring) {
  std::ostringstream name;name<<"zero-load-tire-"<<mass<<"-"<<active<<"-"<<spring;
  Case c(name.str(),0,mass?1200:300,10.75);c.steps=12;c.traction=true;c.halfExtents.z=.02;c.fixedOrientation=true;c.velocity=V(0,2,0);
  for(size_t index=0;index<c.wheels.size();++index) {c.wheels[index].position=V(0,0,index<(size_t)(active?4:1)?0:100);c.wheels[index].spring=spring?140000:35000;}
  suite.push_back(c);
 }
}
