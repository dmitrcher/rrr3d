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
// Source compatibility is the default; profiles can select the historical
// adapter or scale experiments. Invalid profiles fail world creation.
struct CalibrationTuning {
 float gravity=1, torque=1, brake=1, spring=1, damper=1,
       longitudinalGrip=1, lateralGrip=1, bodyFriction=1;
 int solverSubsteps=1;
 bool physxWheelCompat=true;
 bool physxContactSkin=true;
 bool physxSuspensionStop=true;
 bool physxBoxCorners=true;
 bool physxSpeculativeSkin=true;
 bool physxIndependentFriction=true;
 bool physxBoxContactOrder=true;
 bool physxMaterialPairs=true;
 bool physxContactOffsets=true;
 bool physxMeshEdges=true;
 std::string path;
 static CalibrationTuning load() {
  CalibrationTuning t; const char* p=std::getenv("RRR3D_CALIBRATION_PROFILE");
  std::map<std::string,float*> keys{{"gravity_scale",&t.gravity},{"torque_scale",&t.torque},{"brake_scale",&t.brake},{"spring_scale",&t.spring},{"damper_scale",&t.damper},{"longitudinal_grip_scale",&t.longitudinalGrip},{"lateral_grip_scale",&t.lateralGrip},{"body_friction_scale",&t.bodyFriction}};
  if(p && *p) {
   t.path=p;std::ifstream in(p);if(!in)throw std::runtime_error("Cannot read calibration profile: "+t.path);
  std::map<std::string,bool> seen;std::string line;
  while(std::getline(in,line)) {
   line=line.substr(0,line.find('#'));if(line.find_first_not_of(" \t\r")==std::string::npos)continue;
   std::istringstream row(line);std::string key,extra;double value;
   if(!(row>>key>>value) || (row>>extra) || !std::isfinite(value) || seen[key])throw std::runtime_error("Malformed/duplicate calibration profile entry: "+line);
   seen[key]=true;
   if(key=="physx_contact_skin") {if(value!=0 && value!=1)throw std::runtime_error("physx_contact_skin must be 0 or 1");t.physxContactSkin=value==1;}
   else if(key=="physx_box_corners") {if(value!=0 && value!=1)throw std::runtime_error("physx_box_corners must be 0 or 1");t.physxBoxCorners=value==1;}
   else if(key=="physx_speculative_skin") {if(value!=0 && value!=1)throw std::runtime_error("physx_speculative_skin must be 0 or 1");t.physxSpeculativeSkin=value==1;}
   else if(key=="physx_independent_friction") {if(value!=0 && value!=1)throw std::runtime_error("physx_independent_friction must be 0 or 1");t.physxIndependentFriction=value==1;}
   else if(key=="physx_material_pairs") {if(value!=0 && value!=1)throw std::runtime_error("physx_material_pairs must be 0 or 1");t.physxMaterialPairs=value==1;}
   else if(key=="physx_contact_offsets") {if(value!=0 && value!=1)throw std::runtime_error("physx_contact_offsets must be 0 or 1");t.physxContactOffsets=value==1;}
   else if(key=="physx_mesh_edges") {if(value!=0 && value!=1)throw std::runtime_error("physx_mesh_edges must be 0 or 1");t.physxMeshEdges=value==1;}
   else if(key=="physx_box_contact_order") {if(value!=0 && value!=1)throw std::runtime_error("physx_box_contact_order must be 0 or 1");t.physxBoxContactOrder=value==1;}
   else if(key=="physx_suspension_stop") {if(value!=0 && value!=1)throw std::runtime_error("physx_suspension_stop must be 0 or 1");t.physxSuspensionStop=value==1;}
   else if(key=="physx_wheel_compat") {if(value!=0 && value!=1)throw std::runtime_error("physx_wheel_compat must be 0 or 1");t.physxWheelCompat=value==1;}
   else if(key=="solver_substeps") { if(value!=1 && value!=2 && value!=4)throw std::runtime_error("solver_substeps must be 1, 2 or 4");t.solverSubsteps=static_cast<int>(value); }
   else {auto k=keys.find(key);if(k==keys.end() || value<0.1 || value>10)throw std::runtime_error("Unknown or out-of-range calibration setting: "+key);*k->second=static_cast<float>(value);}
  }
  if(!in.eof())throw std::runtime_error("Error reading calibration profile");
  }
  std::ostringstream sample; sample << "{\"solver_substeps\":" << t.solverSubsteps;
  sample << ",\"physx_contact_skin\":" << (t.physxContactSkin ? 1 : 0);
  sample << ",\"physx_wheel_compat\":" << (t.physxWheelCompat ? 1 : 0);
  sample << ",\"physx_suspension_stop\":" << (t.physxSuspensionStop ? 1 : 0);
  sample << ",\"physx_box_corners\":" << (t.physxBoxCorners ? 1 : 0);
  sample << ",\"physx_speculative_skin\":" << (t.physxSpeculativeSkin ? 1 : 0);
  sample << ",\"physx_independent_friction\":" << (t.physxIndependentFriction ? 1 : 0);
  sample << ",\"physx_box_contact_order\":" << (t.physxBoxContactOrder ? 1 : 0);
  sample << ",\"physx_material_pairs\":" << (t.physxMaterialPairs ? 1 : 0);
  sample << ",\"physx_contact_offsets\":" << (t.physxContactOffsets ? 1 : 0);
  sample << ",\"physx_mesh_edges\":" << (t.physxMeshEdges ? 1 : 0);
  for (const auto& key : keys) sample << "," << motorrock::parity::quote(key.first) << ":" << *key.second;
  sample << "}";
  motorrock::parity::event("calibration.profile", t.path.empty() ? "source-v12-default" : t.path, sample.str());
  return t;
 }
 WorldDescription apply(WorldDescription d) const {
  d.gravity*=gravity;
  auto vehicle=[this](VehicleDescription& v){v.bodyFriction*=bodyFriction;v.brakeTorque*=brake;for(auto& w:v.wheels){w.spring*=spring;w.damper*=damper;w.longitudinalTire.stiffnessFactor*=longitudinalGrip;w.lateralTire.stiffnessFactor*=lateralGrip;w.longitudinalTire.extremumValue*=longitudinalGrip;w.longitudinalTire.asymptoteValue*=longitudinalGrip;w.lateralTire.extremumValue*=lateralGrip;w.lateralTire.asymptoteValue*=lateralGrip;}};
  vehicle(d.vehicle);for(auto& spawn:d.spawns)vehicle(spawn.vehicle);return d;
 }
};
}
