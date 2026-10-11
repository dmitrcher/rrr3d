#ifndef MOTORROCK_CALIBRATION_FIXTURE_H
#define MOTORROCK_CALIBRATION_FIXTURE_H
// Deliberately C++98: this exact protocol also compiles with VC2008.
#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <vector>
#include <set>
#include "ParityTrace.h"
namespace motorrock { namespace calibration {
struct V { double x,y,z; V(double a=0,double b=0,double c=0):x(a),y(b),z(c){} };
struct Command { V acceleration, impulse; double torque,brake,steer; Command():torque(0),brake(0),steer(0){} };
struct Tire { double extremumSlip,extremumValue,asymptoteSlip,asymptoteValue,stiffness;
 Tire():extremumSlip(1),extremumValue(.02),asymptoteSlip(2),asymptoteValue(.01),stiffness(1000000){} };
struct Wheel { V position; double radius,travel,spring,damper,target,inverseMass; unsigned flags; bool driven,steering; Tire longitudinal,lateral;
 Wheel():radius(.4),travel(.5),spring(35000),damper(4500),target(0),inverseMass(.1),flags(64),driven(false),steering(false){} };
struct Case {
 std::string name, action, catalog; double gravity,mass,height,bodyFriction,sleepThreshold,restBrake,tireSpring,torqueFactor,brakeFactor,steerFactor,brakeLimit;
 V startOffset,velocity,angular,halfExtents,shapePosition,centerOfMass,angularDamping;
 double initialPitch,bodySkinWidth,clampRoll,clampPitch,airPitch; bool extraGravity; int steps,obstacle; bool ground,traction; double slopeX,slopeY; bool fixedOrientation;
 bool pair;int watched;double peerFriction,peerMass,primaryRoll,peerRoll;V peerPosition,peerVelocity;
 std::vector<Wheel> wheels;
 Case(const std::string& n,double g=-20,double m=1200,double h=12,bool floor=true):name(n),action(n),catalog("synthetic"),gravity(g),mass(m),height(h),bodyFriction(.08),sleepThreshold(0),restBrake(0),tireSpring(0),torqueFactor(1),brakeFactor(1),steerFactor(1),brakeLimit(4000),halfExtents(1.5,.8,.3),angularDamping(1,1,1),initialPitch(0),bodySkinWidth(.025),clampRoll(0),clampPitch(0),airPitch(0),extraGravity(false),steps(600),obstacle(0),ground(floor),traction(true),slopeX(0),slopeY(0){
  pair=false;watched=0;peerFriction=.08;peerMass=1200;primaryRoll=peerRoll=0;fixedOrientation=false;for(int i=0;i<4;++i){Wheel w;w.position=V(i<2?1.1:-1.1,i%2==0?.75:-.75,0);w.driven=w.steering=i<2;wheels.push_back(w);}
 }
};
inline std::vector<Case> coreCases() {
 std::vector<Case> s;
 s.push_back(Case("force",0,1200,100,false));s.back().steps=120;
 s.push_back(Case("force-heavy",0,2400,100,false));s.back().steps=120;
 s.push_back(Case("impulse",0,1200,100,false));s.back().steps=120;
 s.push_back(Case("gravity",-20,1200,100,false));s.back().steps=120;
 s.push_back(Case("spin",0,1200,100,false));s.back().angular=V(0,0,2);s.back().steps=120;
 const char* names[]={"settle","wheel-drive","coast","brake","steer-left","steer-right","lateral","suspension-drop"};
 for(int i=0;i<8;++i)s.push_back(Case(names[i],-20,1200,i==7?15:12));return s;
}
inline double parameter(const char* name,double fallback,double minimum,double maximum) {
 const char* text=std::getenv(name);if(!text || !*text)return fallback;
 char* end=0;const double value=std::strtod(text,&end);
 if(end==text || *end || !(value>=minimum && value<=maximum))throw std::runtime_error(std::string("Invalid calibration input: ")+name);
 return value;
}
struct Inputs {
 double acceleration,torque,brake,steer,impulse;
 Inputs():acceleration(parameter("RRR3D_CALIBRATION_ACCELERATION",3,-100,100)),
 torque(parameter("RRR3D_CALIBRATION_WHEEL_TORQUE",600,-10000,10000)),
 brake(parameter("RRR3D_CALIBRATION_BRAKE_TORQUE",4000,0,10000)),
 steer(parameter("RRR3D_CALIBRATION_STEER",.3,-.5,.5)),
 impulse(parameter("RRR3D_CALIBRATION_DELTA_V",5,-50,50)){}
};
inline const Inputs& inputs(){static const Inputs value;return value;}
inline Command command(const Case& c,int tick) {
 Command x;
 if(c.action=="force" || c.action=="force-heavy") x.acceleration=V(inputs().acceleration,0,0);
 if(c.action=="impulse" && tick==0) x.impulse=V(inputs().impulse,0,0); // delta-v, not Newton seconds
 if(c.action=="wheel-drive" && tick>=120) x.torque=inputs().torque*c.torqueFactor;
 if((c.action=="coast"||c.action=="brake"||c.action=="steer-left"||c.action=="steer-right") && tick>=120 && tick<300) x.torque=inputs().torque*c.torqueFactor;
 if(c.action=="brake" && tick>=300) x.brake=inputs().brake*c.brakeFactor;
 if(c.action=="steer-left" && tick>=240) x.steer=inputs().steer*c.steerFactor;
 if(c.action=="steer-right" && tick>=240) x.steer= -inputs().steer*c.steerFactor;
 if(c.action=="lateral" && tick==180) x.impulse=V(0,inputs().impulse,0);
 if(c.action=="force-y")x.acceleration=V(0,inputs().acceleration,0);
 if(c.action=="force-z")x.acceleration=V(0,0,inputs().acceleration);
 if(c.action=="diagonal-slide" && tick==180)x.impulse=V(10,inputs().impulse,0);
 if(c.action=="low-speed-slide" && tick==180)x.impulse=V(0,.2,0);
 if(c.action=="fast-slide" && tick==180)x.impulse=V(0,20,0);
 if(c.action=="roll-kick" && tick==180)x.impulse=V(0,0,-5);
 if(c.action=="wheel-air-spin")x.torque=inputs().torque;
 if(c.action=="brake-moving"){if(tick==180)x.impulse=V(10,0,0);if(tick>=240)x.brake=inputs().brake*c.brakeFactor;}
 if(c.action=="reverse-drive" && tick>=120)x.torque=-inputs().torque*c.torqueFactor;
 if(c.action=="coast-moving" && tick==180)x.impulse=V(10,0,0);
 if(x.brake==0)x.brake=c.restBrake;
 return x;
}
inline std::string vector(V v) { std::ostringstream s; s.imbue(std::locale::classic()); s<<std::setprecision(9)<<'['<<v.x<<','<<v.y<<','<<v.z<<']'; return s.str(); }
inline std::string commandJson(const Command& c) { std::ostringstream s; s.imbue(std::locale::classic());s<<std::setprecision(9)<<"{\"acceleration\":"<<vector(c.acceleration)<<",\"delta_velocity\":"<<vector(c.impulse)<<",\"wheel_torque\":"<<c.torque<<",\"brake_torque\":"<<c.brake<<",\"steer_radians\":"<<c.steer<<'}';return s.str(); }
struct Sample { V position,velocity,momentum; double q[4]; std::vector<double> wheelSpeed,wheelHeight,longSlip,latSlip,suspension,normalForce,longImpulse,latImpulse; std::vector<int> contacts;
 Sample(){ q[0]=q[1]=q[2]=0;q[3]=1; }
};
inline std::string tireJson(const Tire& t){std::ostringstream s;s.imbue(std::locale::classic());s<<std::setprecision(9)<<'['<<t.extremumSlip<<','<<t.extremumValue<<','<<t.asymptoteSlip<<','<<t.asymptoteValue<<','<<t.stiffness<<']';return s.str();}
inline void begin(const Case& c,const std::string& profile) {
 std::ostringstream s;s.imbue(std::locale::classic());s<<std::setprecision(9)<<"{\"schema\":1,\"fixture\":\"source-vehicle-matrix-v7-sdk\",\"dt\":0.016666666666666666,\"sample_phase\":\"after_solve\",\"steps\":"<<c.steps<<",\"mass\":"<<c.mass<<",\"gravity\":"<<c.gravity<<",\"height\":"<<c.height<<",\"ground\":"<<(c.ground?"true":"false")<<",\"initial_velocity\":"<<vector(c.velocity)<<",\"initial_angular_velocity\":"<<vector(c.angular)<<",\"profile\":"<<parity::quote(profile)<<",\"fixture_parameters\":{\"catalog\":"<<parity::quote(c.catalog)<<",\"half_extents\":"<<vector(c.halfExtents)<<",\"shape_position\":"<<vector(c.shapePosition)<<",\"center_of_mass\":"<<vector(c.centerOfMass)<<",\"body_friction\":"<<c.bodyFriction<<",\"sleep_threshold\":"<<c.sleepThreshold<<",\"tire_spring\":"<<c.tireSpring<<",\"traction\":"<<(c.traction?"true":"false")<<",\"obstacle\":"<<c.obstacle<<",\"slope_x\":"<<c.slopeX<<",\"slope_y\":"<<c.slopeY<<",\"angular_damping\":0.05,\"skin_width\":0.025,\"adaptive_force\":false,\"actor_flags\":20,\"ground_friction\":0.1,\"ground_restitution\":0,\"brake_limit\":"<<c.brakeLimit<<",\"ground_height\":10,\"wheels\":[";
 for(size_t i=0;i<c.wheels.size();++i){const Wheel& w=c.wheels[i];if(i)s<<',';s<<"{\"position\":"<<vector(w.position)<<",\"radius\":"<<w.radius<<",\"travel\":"<<w.travel<<",\"spring\":"<<w.spring<<",\"damper\":"<<w.damper<<",\"target\":"<<w.target<<",\"inverse_mass\":"<<w.inverseMass<<",\"flags\":"<<w.flags<<",\"driven\":"<<(w.driven?"true":"false")<<",\"steering\":"<<(w.steering?"true":"false")<<",\"longitudinal_tire\":"<<tireJson(w.longitudinal)<<",\"lateral_tire\":"<<tireJson(w.lateral)<<'}';}
 s<<"],\"extra_gravity\":"<<(c.extraGravity?"true":"false")<<",\"air_pitch\":"<<c.airPitch<<",\"clamp_roll\":"<<c.clampRoll<<",\"clamp_pitch\":"<<c.clampPitch<<",\"game_angular_damping\":"<<vector(c.angularDamping)<<"},\"units\":\"source-world, seconds, radians, kg, Nm; acceleration=world/s^2; impulse=delta-v\"}";
 std::string metadata=s.str();if(c.initialPitch!=0){std::ostringstream x;x<<",\"initial_pitch\":"<<std::setprecision(9)<<c.initialPitch;metadata.insert(metadata.size()-1,x.str());}if(c.bodySkinWidth!=.025){std::ostringstream x;x<<",\"body_skin_width\":"<<std::setprecision(9)<<c.bodySkinWidth;metadata.insert(metadata.size()-1,x.str());}if(c.startOffset.x!=0||c.startOffset.y!=0||c.startOffset.z!=0)metadata.insert(metadata.size()-1,",\"initial_position\":"+vector(V(c.startOffset.x,c.startOffset.y,c.height+c.startOffset.z)));if(c.fixedOrientation)metadata.insert(metadata.size()-1,",\"fixed_orientation\":true");
 if(c.pair){std::ostringstream pair;pair.imbue(std::locale::classic());pair<<std::setprecision(9)<<",\"pair\":{\"watched\":"<<c.watched<<",\"peer_friction\":"<<c.peerFriction<<",\"peer_mass\":"<<c.peerMass<<",\"peer_position\":"<<vector(c.peerPosition)<<",\"peer_velocity\":"<<vector(c.peerVelocity)<<",\"primary_roll\":"<<c.primaryRoll<<",\"peer_roll\":"<<c.peerRoll<<'}';metadata.insert(metadata.size()-1,pair.str());}
 parity::event("calibration.begin",c.name,metadata);
}
inline void sample(const Case& c,int tick,const Command& command,const Sample& a) {
 std::ostringstream s;s.imbue(std::locale::classic());s<<std::setprecision(9)<<"{\"tick\":"<<tick<<",\"time\":"<<((tick+1)/60.0)<<",\"command\":"<<commandJson(command)<<",\"position\":"<<vector(a.position)<<",\"velocity\":"<<vector(a.velocity)<<",\"angular_momentum\":"<<vector(a.momentum)<<",\"rotation\":["<<a.q[0]<<','<<a.q[1]<<','<<a.q[2]<<','<<a.q[3]<<"],\"wheels\":[";
 for(size_t i=0;i<a.wheelSpeed.size();++i){if(i)s<<',';s<<"{\"angular_speed\":"<<a.wheelSpeed[i]<<",\"height\":"<<a.wheelHeight[i]<<",\"contact\":"<<(a.contacts[i]?"true":"false")<<",\"engine_longitudinal_slip\":"<<a.longSlip[i]<<",\"engine_lateral_slip\":"<<a.latSlip[i]<<",\"suspension_length\":"<<a.suspension[i]<<",\"engine_normal_force\":"<<a.normalForce[i]<<",\"longitudinal_impulse\":"<<a.longImpulse[i]<<",\"lateral_impulse\":"<<a.latImpulse[i]<<'}';}
 s<<"]}";parity::event("calibration.sample",c.name,s.str());
}
#include "Catalog.h"
#ifdef MOTORROCK_EXPERIMENT_SPRING_PROBES
#include "experimental/SpringProbeCases.h"
#endif
inline std::vector<Case> cases() {
 std::vector<Case> s=coreCases();Case c("probe");
 const char* groundNames[]={"diagonal-slide","low-speed-slide","fast-slide","brake-moving","coast-moving","reverse-drive"};
 for(int i=0;i<6;++i)s.push_back(Case(groundNames[i]));
 const char* freeNames[]={"force-y","force-z","spin-roll","spin-pitch","spin-limit","spin-mixed","wheel-air-spin"};
 for(int i=0;i<7;++i){c=Case(freeNames[i],0,1200,100,false);c.steps=120;if(i==2)c.angular=V(2,0,0);if(i==3)c.angular=V(0,2,0);if(i==4)c.angular=V(0,0,20);if(i==5)c.angular=V(1,2,3);s.push_back(c);}
 const char* suspensionNames[]={"spring-soft","spring-hard","damper-low","damper-high","travel-short","travel-long","radius-small","radius-large","mass-light","mass-heavy","target-half","com-forward","com-low"};
 for(int i=0;i<13;++i){c=Case(suspensionNames[i],-20,1200,15);for(size_t w=0;w<c.wheels.size();++w){if(i==0)c.wheels[w].spring*=.5;if(i==1)c.wheels[w].spring*=2;if(i==2)c.wheels[w].damper*=.25;if(i==3)c.wheels[w].damper*=2;if(i==4)c.wheels[w].travel=.25;if(i==5)c.wheels[w].travel=1;if(i==6)c.wheels[w].radius=.2;if(i==7)c.wheels[w].radius=.7;if(i==10)c.wheels[w].target=.5;}if(i==8)c.mass=600;if(i==9)c.mass=2400;if(i==11)c.centerOfMass.x=.5;if(i==12)c.centerOfMass.z=-.5;s.push_back(c);}
 for(int i=0;i<6;++i){c=Case(i==0?"grip-low":i==1?"grip-high":i==2?"inertia-light":i==3?"inertia-heavy":i==4?"rest-brake-drive":"all-wheel-drive");c.action="wheel-drive";for(size_t w=0;w<c.wheels.size();++w){if(i<2){double k=i==0?.5:2;c.wheels[w].longitudinal.extremumValue*=k;c.wheels[w].longitudinal.asymptoteValue*=k;}if(i==2)c.wheels[w].inverseMass=.2;if(i==3)c.wheels[w].inverseMass=.05;if(i==5)c.wheels[w].driven=true;}if(i==4)c.restBrake=400;s.push_back(c);}
 for(int i=0;i<5;++i){c=Case(i==0?"oil-slide":i==1?"tire-overload":i==2?"slope-longitudinal":i==3?"slope-lateral":"sleep-settle");if(i==0){c.action="lateral";c.traction=false;}if(i==1){c.action="wheel-drive";c.tireSpring=.2;}if(i==2)c.slopeX=.1;if(i==3)c.slopeY=.1;if(i==4)c.sleepThreshold=.05;s.push_back(c);}
 for(int i=0;i<4;++i){c=Case(i==0?"wall-hit":i==1?"wall-oblique":i==2?"bump-crossing":"curb-side");c.obstacle=i+1;c.velocity=V(8,i==1?2:0,0);s.push_back(c);}
 const char* gameNames[]={"game-air-gravity","game-air-pitch","game-angular-damping","game-clamp-roll","game-clamp-pitch","game-clamp-mixed"};
 for(int i=0;i<6;++i){c=Case(gameNames[i],i<2?-20:0,1200,100,false);c.steps=120;if(i<2)c.extraGravity=true;if(i==1){c.airPitch=4;c.velocity=V(5,0,0);}if(i==2){c.angular=V(1,2,3);c.angularDamping=V(.99,.98,.97);}if(i>=3){c.angular=V(3,4,2);if(i!=4)c.clampRoll=.3;if(i!=3)c.clampPitch=.4;}s.push_back(c);}
 const char* bodyNames[]={"body-slide-x","body-slide-y","body-slide-diagonal","body-tilt-pitch","body-tilt-roll"};
 for(int i=0;i<5;++i){c=Case(bodyNames[i],-20,1200,11);c.steps=180;c.velocity=V(i==1?0:5,i==1||i==2?5:0,-3);if(i==3)c.angular=V(0,1,0);if(i==4)c.angular=V(1,0,0);for(size_t w=0;w<c.wheels.size();++w)c.wheels[w].position.z=100;s.push_back(c);}
 for(int i=0;i<3;++i){c=Case(i==0?"body-wheel-contact-x":i==1?"body-wheel-contact-pitch":"body-wheel-contact-roll",-20,1200,11);c.steps=180;c.velocity=V(5,0,-3);if(i==1)c.angular=V(0,1,0);if(i==2)c.angular=V(1,0,0);for(size_t w=0;w<c.wheels.size();++w){c.wheels[w].spring=c.wheels[w].damper=0;c.wheels[w].travel=5;c.wheels[w].longitudinal.stiffness=c.wheels[w].lateral.stiffness=0;}s.push_back(c);}
 const double wallSpeeds[]={1,2,4,8,12,24,4,12};
 for(int i=0;i<8;++i){std::ostringstream name;name<<"wall-air-"<<(i>=6?"oblique-":"straight-")<<wallSpeeds[i];c=Case(name.str(),0,1200,13,false);c.obstacle=1;c.velocity=V(wallSpeeds[i],i>=6?2:0,0);s.push_back(c);}
 c=Case("wall-air-oblique-negative-4",0,1200,13,false);c.obstacle=1;c.velocity=V(4,-2,0);s.push_back(c);
 for(int i=0;i<2;++i){c=Case(i==0?"wall-air-y-straight-8":"wall-air-y-oblique-8",0,1200,13,false);c.obstacle=6;c.velocity=V(i==0?0:2,8,0);s.push_back(c);}
 const char* lockedNames[]={"body-friction-locked-x","body-friction-locked-y","body-friction-locked-diagonal","body-friction-locked-skew"};
 for(int i=0;i<4;++i){c=Case(lockedNames[i],-20,1200,10.25,false);c.steps=180;c.obstacle=5;c.fixedOrientation=true;c.velocity=V(i==1?0:5,i==0?0:i==3?2:5,0);for(size_t w=0;w<c.wheels.size();++w)c.wheels[w].position.z=100;s.push_back(c);}
 const char* pairNames[]={"same-head","same-oblique","head-on","heavy-peer","weak-first","weak-second","tilted-first","tilted-second","pole","pole-oblique","free-oblique","stock-head","stock-oblique","stock-weak-second","stock-pole","vertical-locked","vertical-weak-peer"};
 for(int i=0;i<17;++i)for(int watched=0;watched<2;++watched){
  std::ostringstream n;n<<"pair-"<<pairNames[i]<<"-"<<watched;c=Case(n.str(),0,1200,13,false);c.steps=300;c.pair=true;c.watched=watched;c.fixedOrientation=i<10||i>=15;c.velocity=V(8,i==0||i==2?0:2,0);c.peerPosition=V(8,0,13);
  if(i==2){c.velocity=V(4,0,0);c.peerVelocity=V(-4,0,0);}if(i==3)c.peerMass=2400;
  if(i==4){c.bodyFriction=.02;c.primaryRoll=.35;}if(i==5||i==13||i==16){c.peerFriction=.02;c.peerRoll=.35;}
  if(i==6)c.primaryRoll=.35;if(i==7)c.peerRoll=.35;
  if(i==8||i==9||i==14){c.peerPosition=V(0,0,16);c.velocity=V(i==9?2:0,0,4);}
  if(i>=15){c.velocity=V(8,0,2);c.peerPosition=V(8,0,14.25);}
  if(i==11)c.velocity=V(8,0,0);if(i<11||i>=15)for(size_t w=0;w<c.wheels.size();++w)c.wheels[w].position.z=100;s.push_back(c);
 }
 const char* borderNames[]={"border-air-straight","border-air-oblique","border-grounded","border-pole"};
 for(int i=0;i<4;++i){c=Case(borderNames[i],i>=2?-20:0,1200,i>=2?12:13,i>=2);c.steps=600;c.obstacle=i==3?8:7;c.velocity=V(8,i==0?0:2,0);if(i==3){c.ground=false;c.height=10.25;c.fixedOrientation=true;for(size_t w=0;w<c.wheels.size();++w)c.wheels[w].position.z=100;}s.push_back(c);}
 const char* frontBorderNames[]={"border-front-air-straight","border-front-air-oblique","border-front-grounded"};
 for(int i=0;i<3;++i){c=Case(frontBorderNames[i],i==2?-20:0,1200,i==2?12:13,i==2);c.obstacle=9;c.velocity=V(8,i==0?0:2,0);s.push_back(c);}
 const double contactDepths[]={-.02,0,.025,.05,.25,1};const double contactSpeeds[]={-8,0,8};
 for(int locked=0;locked<2;++locked)for(int shape=0;shape<2;++shape)for(int depth=0;depth<6;++depth)for(int speed=0;speed<3;++speed){
  std::ostringstream n;n<<"contact-"<<(locked?"free-":"locked-")<<(shape?"box":"triangle")<<"-depth"<<depth<<"-speed"<<speed;
  c=Case(n.str(),0,1200,13,false);c.steps=12;c.obstacle=shape?1:9;c.fixedOrientation=locked==0;c.velocity=V(contactSpeeds[speed],0,0);
  c.startOffset.x=(shape?7.75:8)-1.5+contactDepths[depth];for(size_t w=0;w<c.wheels.size();++w)c.wheels[w].position.z=100;s.push_back(c);
 }
 // Independent Nx stationary skin probes include tilted and thin boxes.
 const double skinAngles[]={0,.2,.5,.8,-.5};const double skinWidths[]={.01,.025,.1};
 for(int thin=0;thin<2;++thin)for(int skin=0;skin<3;++skin)for(int shape=0;shape<2;++shape)for(int angle=0;angle<5;++angle){
  std::ostringstream n;n<<"skin-tilted-"<<(shape?"box":"triangle")<<"-angle"<<angle<<"-skin"<<skin<<"-thin"<<thin;
  c=Case(n.str(),0,1200,13,false);c.steps=24;c.obstacle=shape?1:9;c.fixedOrientation=true;c.initialPitch=skinAngles[angle];c.bodySkinWidth=skinWidths[skin];c.halfExtents.z=thin?.02:.3;
  const double support=c.halfExtents.x*std::abs(std::cos(c.initialPitch))+c.halfExtents.z*std::abs(std::sin(c.initialPitch));
  c.startOffset.x=(shape?7.75:8)-support+.25;for(size_t w=0;w<c.wheels.size();++w)c.wheels[w].position.z=100;s.push_back(c);
 }

#ifdef MOTORROCK_EXPERIMENT_SPRING_PROBES
 appendSpringProbes(s);appendScalarSpringProbes(s);appendLostSpringProbes(s);appendHardSpringProbes(s);appendCoupledFloorProbes(s);appendTireSlipProbes(s);appendHardTireProbes(s);appendZeroLoadTireProbes(s);
#endif
 appendCatalogCases(s);return s;
}
inline int run(int argc,char**argv, void (*execute)(const Case&), const std::string& profile) {
 static_cast<void>(inputs());
 if(argc!=2 && argc!=3) throw std::runtime_error("Usage: physics-calibration OUTPUT.jsonl [CASE|all]");
 const std::string selected=argc==3?argv[2]:"all";bool found=false;
 const std::vector<Case> suite=cases();
 if(selected.find("list:")==0){
  std::set<std::string> requested;std::string name;std::istringstream names(selected.substr(5));
  if(selected.size()==5||selected[selected.size()-1]==',')throw std::runtime_error("Empty calibration list item");
  while(std::getline(names,name,',')){
   bool exists=false;for(size_t i=0;i<suite.size();++i)if(suite[i].name==name)exists=true;
   if(name.empty()||!exists||!requested.insert(name).second)throw std::runtime_error("Unknown/duplicate calibration list item: "+name);
  }
 }
 std::ofstream clear(argv[1],std::ios::binary|std::ios::trunc); if(!clear)throw std::runtime_error("Cannot create output trace"); clear.close();
#ifdef _WIN32
 _putenv_s("RRR3D_TRACE_FILE",argv[1]);
#else
 setenv("RRR3D_TRACE_FILE",argv[1],1);
#endif
 for(size_t i=0;i<suite.size();++i)if(selected=="all" || (selected=="core" && i<13) || (selected=="catalog" && suite[i].name.find("/")!=std::string::npos && suite[i].name.find("mobility/")!=0) || (selected=="mobility" && suite[i].name.find("mobility/")==0) || (selected=="synthetic" && suite[i].name.find("/")==std::string::npos) || (selected.find("list:")==0 && (std::string(",")+selected.substr(5)+",").find(std::string(",")+suite[i].name+",")!=std::string::npos) || selected==suite[i].name){ found=true;begin(suite[i],profile);execute(suite[i]);parity::event("calibration.end",suite[i].name);}
 if(!found)throw std::runtime_error("Unknown calibration case");
 return 0;
}
} }
#endif
