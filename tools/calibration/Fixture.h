#ifndef MOTORROCK_CALIBRATION_FIXTURE_H
#define MOTORROCK_CALIBRATION_FIXTURE_H
// Deliberately C++98: this exact protocol also compiles with VC2008.
#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <vector>
#include "ParityTrace.h"
namespace motorrock { namespace calibration {
struct V { double x,y,z; V(double a=0,double b=0,double c=0):x(a),y(b),z(c){} };
struct Command { V acceleration, impulse; double torque,brake,steer; Command():torque(0),brake(0),steer(0){} };
struct Case {
 std::string name; double gravity, mass, height; V velocity, angular; int steps; bool ground;
 Case(const std::string& n,double g= -20,double m=1200,double h=12,bool floor=true):name(n),gravity(g),mass(m),height(h),steps(600),ground(floor){}
};
inline std::vector<Case> cases() {
 std::vector<Case> s;
 s.push_back(Case("force",0,1200,100,false)); s.back().steps=120;
 s.push_back(Case("force-heavy",0,2400,100,false)); s.back().steps=120;
 s.push_back(Case("impulse",0,1200,100,false)); s.back().steps=120;
 s.push_back(Case("gravity",-20,1200,100,false)); s.back().steps=120;
 s.push_back(Case("spin",0,1200,100,false)); s.back().angular=V(0,0,2); s.back().steps=120;
 s.push_back(Case("settle"));
 s.push_back(Case("wheel-drive"));
 s.push_back(Case("coast"));
 s.push_back(Case("brake"));
 s.push_back(Case("steer-left"));
 s.push_back(Case("steer-right"));
 s.push_back(Case("lateral"));
 s.push_back(Case("suspension-drop",-20,1200,15));
 return s;
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
 if(c.name=="force" || c.name=="force-heavy") x.acceleration=V(inputs().acceleration,0,0);
 if(c.name=="impulse" && tick==0) x.impulse=V(inputs().impulse,0,0); // delta-v, not Newton seconds
 if(c.name=="wheel-drive" && tick>=120) x.torque=inputs().torque;
 if((c.name=="coast"||c.name=="brake"||c.name=="steer-left"||c.name=="steer-right") && tick>=120 && tick<300) x.torque=inputs().torque;
 if(c.name=="brake" && tick>=300) x.brake=inputs().brake;
 if(c.name=="steer-left" && tick>=240) x.steer=inputs().steer;
 if(c.name=="steer-right" && tick>=240) x.steer= -inputs().steer;
 if(c.name=="lateral" && tick==180) x.impulse=V(0,inputs().impulse,0);
 return x;
}
inline std::string vector(V v) { std::ostringstream s; s.imbue(std::locale::classic()); s<<std::setprecision(9)<<'['<<v.x<<','<<v.y<<','<<v.z<<']'; return s.str(); }
inline std::string commandJson(const Command& c) { std::ostringstream s; s.imbue(std::locale::classic());s<<std::setprecision(9)<<"{\"acceleration\":"<<vector(c.acceleration)<<",\"delta_velocity\":"<<vector(c.impulse)<<",\"wheel_torque\":"<<c.torque<<",\"brake_torque\":"<<c.brake<<",\"steer_radians\":"<<c.steer<<'}';return s.str(); }
struct Sample { V position,velocity,momentum; double q[4]; std::vector<double> wheelSpeed,wheelHeight; std::vector<int> contacts;
 Sample(){ q[0]=q[1]=q[2]=0;q[3]=1; }
};
inline void begin(const Case& c,const std::string& profile) {
 std::ostringstream s;s.imbue(std::locale::classic());s<<std::setprecision(9)<<"{\"schema\":1,\"fixture\":\"box-four-wheels-v3-clamped\",\"dt\":0.016666666666666666,\"sample_phase\":\"after_solve\",\"steps\":"<<c.steps<<",\"mass\":"<<c.mass<<",\"gravity\":"<<c.gravity<<",\"height\":"<<c.height<<",\"ground\":"<<(c.ground?"true":"false")<<",\"initial_velocity\":"<<vector(c.velocity)<<",\"initial_angular_velocity\":"<<vector(c.angular)<<",\"profile\":"<<parity::quote(profile)<<",\"fixture_parameters\":{\"half_extents\":[1.5,0.8,0.3],\"body_friction\":0.08,\"angular_damping\":0.05,\"skin_width\":0.025,\"wheel_flags\":64,\"wheel_radius\":0.4,\"wheel_travel\":0.5,\"spring\":35000,\"damper\":4500,\"inverse_wheel_mass\":0.1,\"powered_wheels\":[0,1],\"ground_height\":10},\"units\":\"source-world, seconds, radians, kg, Nm; acceleration=world/s^2; impulse=delta-v\"}";
 parity::event("calibration.begin",c.name,s.str());
}
inline void sample(const Case& c,int tick,const Command& command,const Sample& a) {
 std::ostringstream s;s.imbue(std::locale::classic());s<<std::setprecision(9)<<"{\"tick\":"<<tick<<",\"time\":"<<((tick+1)/60.0)<<",\"command\":"<<commandJson(command)<<",\"position\":"<<vector(a.position)<<",\"velocity\":"<<vector(a.velocity)<<",\"angular_momentum\":"<<vector(a.momentum)<<",\"rotation\":["<<a.q[0]<<','<<a.q[1]<<','<<a.q[2]<<','<<a.q[3]<<"],\"wheels\":[";
 for(size_t i=0;i<a.wheelSpeed.size();++i){if(i)s<<',';s<<"{\"angular_speed\":"<<a.wheelSpeed[i]<<",\"height\":"<<a.wheelHeight[i]<<",\"contact\":"<<(a.contacts[i]?"true":"false")<<'}';}
 s<<"]}";parity::event("calibration.sample",c.name,s.str());
}
inline int run(int argc,char**argv, void (*execute)(const Case&), const std::string& profile) {
 static_cast<void>(inputs());
 if(argc!=2 && argc!=3) throw std::runtime_error("Usage: physics-calibration OUTPUT.jsonl [CASE|all]");
 std::ofstream clear(argv[1],std::ios::binary|std::ios::trunc); if(!clear)throw std::runtime_error("Cannot create output trace"); clear.close();
#ifdef _WIN32
 _putenv_s("RRR3D_TRACE_FILE",argv[1]);
#else
 setenv("RRR3D_TRACE_FILE",argv[1],1);
#endif
 const std::string selected=argc==3?argv[2]:"all";bool found=false;
 const std::vector<Case> suite=cases();
 for(size_t i=0;i<suite.size();++i)if(selected=="all"||selected==suite[i].name){ found=true;begin(suite[i],profile);execute(suite[i]);parity::event("calibration.end",suite[i].name);}
 if(!found)throw std::runtime_error("Unknown calibration case");
 return 0;
}
} }
#endif
