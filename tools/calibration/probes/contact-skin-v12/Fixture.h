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
 V velocity,angular,halfExtents,shapePosition,centerOfMass,angularDamping;
 double clampRoll,clampPitch,airPitch; bool extraGravity; int steps,obstacle; bool ground,traction; double slopeX,slopeY; bool fixedOrientation;
 double probeHalfZ;double probeSkin;double probePitch;double probeStartX;bool pair;int watched;double peerFriction,peerMass,primaryRoll,peerRoll;V peerPosition,peerVelocity;
 std::vector<Wheel> wheels;
 Case(const std::string& n,double g=-20,double m=1200,double h=12,bool floor=true):name(n),action(n),catalog("synthetic"),gravity(g),mass(m),height(h),bodyFriction(.08),sleepThreshold(0),restBrake(0),tireSpring(0),torqueFactor(1),brakeFactor(1),steerFactor(1),brakeLimit(4000),halfExtents(1.5,.8,.3),angularDamping(1,1,1),clampRoll(0),clampPitch(0),airPitch(0),extraGravity(false),steps(600),obstacle(0),ground(floor),traction(true),slopeX(0),slopeY(0){
  probeHalfZ=.3;probeSkin=.025;probePitch=0;probeStartX=0;pair=false;watched=0;peerFriction=.08;peerMass=1200;primaryRoll=peerRoll=0;fixedOrientation=false;for(int i=0;i<4;++i){Wheel w;w.position=V(i<2?1.1:-1.1,i%2==0?.75:-.75,0);w.driven=w.steering=i<2;wheels.push_back(w);}
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
 std::string metadata=s.str();{std::ostringstream x;x<<",\"probe_initial_x\":"<<std::setprecision(9)<<c.probeStartX;metadata.insert(metadata.size()-1,x.str());}if(c.fixedOrientation)metadata.insert(metadata.size()-1,",\"fixed_orientation\":true");
 if(c.pair){std::ostringstream pair;pair.imbue(std::locale::classic());pair<<std::setprecision(9)<<",\"pair\":{\"watched\":"<<c.watched<<",\"peer_friction\":"<<c.peerFriction<<",\"peer_mass\":"<<c.peerMass<<",\"peer_position\":"<<vector(c.peerPosition)<<",\"peer_velocity\":"<<vector(c.peerVelocity)<<",\"primary_roll\":"<<c.primaryRoll<<",\"peer_roll\":"<<c.peerRoll<<'}';metadata.insert(metadata.size()-1,pair.str());}
 parity::event("calibration.begin",c.name,metadata);
}
inline void sample(const Case& c,int tick,const Command& command,const Sample& a) {
 std::ostringstream s;s.imbue(std::locale::classic());s<<std::setprecision(9)<<"{\"tick\":"<<tick<<",\"time\":"<<((tick+1)/60.0)<<",\"command\":"<<commandJson(command)<<",\"position\":"<<vector(a.position)<<",\"velocity\":"<<vector(a.velocity)<<",\"angular_momentum\":"<<vector(a.momentum)<<",\"rotation\":["<<a.q[0]<<','<<a.q[1]<<','<<a.q[2]<<','<<a.q[3]<<"],\"wheels\":[";
 for(size_t i=0;i<a.wheelSpeed.size();++i){if(i)s<<',';s<<"{\"angular_speed\":"<<a.wheelSpeed[i]<<",\"height\":"<<a.wheelHeight[i]<<",\"contact\":"<<(a.contacts[i]?"true":"false")<<",\"engine_longitudinal_slip\":"<<a.longSlip[i]<<",\"engine_lateral_slip\":"<<a.latSlip[i]<<",\"suspension_length\":"<<a.suspension[i]<<",\"engine_normal_force\":"<<a.normalForce[i]<<",\"longitudinal_impulse\":"<<a.longImpulse[i]<<",\"lateral_impulse\":"<<a.latImpulse[i]<<'}';}
 s<<"]}";parity::event("calibration.sample",c.name,s.str());
}
#include "Catalog.h"
inline std::vector<Case> cases() {
 std::vector<Case> s;const double angles[]={0,.2,.5,.8,-.5};
 for(int thin=0;thin<2;++thin)for(int skin=0;skin<3;++skin)for(int shape=0;shape<2;++shape)for(int angle=0;angle<5;++angle){
  std::ostringstream name;name<<"skin-tilted-"<<(shape?"box":"triangle")<<"-angle"<<angle<<"-skin"<<skin<<"-thin"<<thin;
  Case c(name.str(),0,1200,13,false);c.steps=24;c.obstacle=shape?1:9;c.fixedOrientation=true;c.probePitch=angles[angle];c.probeSkin=skin==0?.01:skin==1?.025:.1;c.probeHalfZ=thin?.02:.3;c.halfExtents.z=c.probeHalfZ;
  const double support=1.5*std::abs(std::cos(c.probePitch))+c.probeHalfZ*std::abs(std::sin(c.probePitch));
  c.probeStartX=(shape?7.75:8)-support+.25;
  for(size_t w=0;w<c.wheels.size();++w)c.wheels[w].position.z=100;s.push_back(c);
 }
 const double depths[]={-.02,0,.025,.05,.25,1};const double speeds[]={-8,0,8};
 for(int locked=0;locked<2;++locked)for(int shape=0;shape<2;++shape)for(int d=0;d<6;++d)for(int v=0;v<3;++v){
  std::ostringstream name;name<<(locked?"bias-free-":"bias-")<<(shape?"box":"triangle")<<"-depth"<<d<<"-speed"<<v;
  Case c(name.str(),0,1200,13,false);c.steps=12;c.obstacle=shape?1:9;c.fixedOrientation=locked==0;c.velocity=V(speeds[v],0,0);
  c.probeStartX=(shape?7.75:8)-1.5+depths[d];for(size_t w=0;w<c.wheels.size();++w)c.wheels[w].position.z=100;
  s.push_back(c);
 }
 return s;
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
