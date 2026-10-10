#pragma once
#include "physics/OriginalVehiclePhysics.h"
#include "ParityTrace.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
namespace r3d::physics {
// Explicitly opt-in. An invalid profile fails world creation rather than
// silently running an experiment with different settings.
struct CalibrationTuning {
 float gravity=1, torque=1, brake=1, spring=1, damper=1,
       longitudinalGrip=1, lateralGrip=1, bodyFriction=1;
 int solverSubsteps=2;
 std::string path;
 static CalibrationTuning load() {
  CalibrationTuning t; const char* p=std::getenv("RRR3D_CALIBRATION_PROFILE");
  if(!p || !*p)return t;
  t.path=p;std::ifstream in(p);if(!in)throw std::runtime_error("Cannot read calibration profile: "+t.path);
  std::map<std::string,float*> keys{{"gravity_scale",&t.gravity},{"torque_scale",&t.torque},{"brake_scale",&t.brake},{"spring_scale",&t.spring},{"damper_scale",&t.damper},{"longitudinal_grip_scale",&t.longitudinalGrip},{"lateral_grip_scale",&t.lateralGrip},{"body_friction_scale",&t.bodyFriction}};
  std::map<std::string,bool> seen;std::string line;
  while(std::getline(in,line)) {
   line=line.substr(0,line.find('#'));if(line.find_first_not_of(" \t\r")==std::string::npos)continue;
   std::istringstream row(line);std::string key,extra;double value;
   if(!(row>>key>>value) || (row>>extra) || !std::isfinite(value) || seen[key])throw std::runtime_error("Malformed/duplicate calibration profile entry: "+line);
   seen[key]=true;
   if(key=="solver_substeps") { if(value!=1 && value!=2 && value!=4)throw std::runtime_error("solver_substeps must be 1, 2 or 4");t.solverSubsteps=static_cast<int>(value); }
   else {auto k=keys.find(key);if(k==keys.end() || value<0.1 || value>10)throw std::runtime_error("Unknown or out-of-range calibration setting: "+key);*k->second=static_cast<float>(value);}
  }
  if(!in.eof())throw std::runtime_error("Error reading calibration profile");
  std::ostringstream sample; sample << "{\"solver_substeps\":" << t.solverSubsteps;
  for (const auto& key : keys) sample << "," << motorrock::parity::quote(key.first) << ":" << *key.second;
  sample << "}";
  motorrock::parity::event("calibration.profile", t.path, sample.str());
  return t;
 }
 WorldDescription apply(WorldDescription d) const {
  d.gravity*=gravity;
  auto vehicle=[this](VehicleDescription& v){v.bodyFriction*=bodyFriction;v.brakeTorque*=brake;for(auto& w:v.wheels){w.spring*=spring;w.damper*=damper;w.longitudinalTire.stiffnessFactor*=longitudinalGrip;w.lateralTire.stiffnessFactor*=lateralGrip;w.longitudinalTire.extremumValue*=longitudinalGrip;w.longitudinalTire.asymptoteValue*=longitudinalGrip;w.lateralTire.extremumValue*=lateralGrip;w.lateralTire.asymptoteValue*=lateralGrip;}};
  vehicle(d.vehicle);for(auto& spawn:d.spawns)vehicle(spawn.vehicle);return d;
 }
};
}
