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
