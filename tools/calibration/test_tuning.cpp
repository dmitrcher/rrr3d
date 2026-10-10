#include "physics/CalibrationTuning.h"
#include <cstdio>
#include <iostream>
int main(int argc,char**argv) {
 if(argc!=2)return 2;const char* path=argv[1];
 const char* old=std::getenv("RRR3D_CALIBRATION_PROFILE");const std::string previous=old?old:"";
 unsetenv("RRR3D_CALIBRATION_PROFILE");auto neutral=r3d::physics::CalibrationTuning::load();
 if(neutral.solverSubsteps!=1 || neutral.gravity!=1 || !neutral.physxWheelCompat || !neutral.physxContactSkin || !neutral.physxSuspensionStop || !neutral.physxBoxCorners || !neutral.physxSpeculativeSkin || !neutral.physxIndependentFriction || !neutral.physxBoxContactOrder || !neutral.physxMaterialPairs)return 3;
 setenv("RRR3D_CALIBRATION_PROFILE",path,1);
 {std::ofstream f(path);f<<"physx_wheel_compat 1\nphysx_suspension_stop 0\nphysx_box_corners 0\nphysx_speculative_skin 0\nphysx_independent_friction 0\nphysx_box_contact_order 0\nphysx_material_pairs 0\nsolver_substeps 1\nspring_scale 1.5\nlongitudinal_grip_scale 0.5\nbrake_scale 1.2\n";}
 auto t=r3d::physics::CalibrationTuning::load();r3d::physics::WorldDescription d;d.vehicle.brakeTorque=100;d.vehicle.wheels.resize(1);d.vehicle.wheels[0].spring=100;d.vehicle.wheels[0].longitudinalTire.extremumValue=2;
 auto changed=t.apply(d);
 if(!t.physxWheelCompat || t.physxSuspensionStop || t.physxBoxCorners || t.physxSpeculativeSkin || t.physxIndependentFriction || t.physxBoxContactOrder || t.physxMaterialPairs || t.solverSubsteps!=1 || changed.vehicle.wheels[0].spring!=150 || changed.vehicle.wheels[0].longitudinalTire.extremumValue!=1 || std::abs(changed.vehicle.brakeTorque-120)>1e-4 || d.vehicle.wheels[0].spring!=100)return 4;
 const char* invalid[]={"physx_material_pairs 2\n","physx_material_pairs 1\nphysx_material_pairs 0\n","physx_independent_friction 2\n","physx_box_contact_order -1\n","physx_independent_friction 1\nphysx_independent_friction 0\n","physx_box_contact_order 0\nphysx_box_contact_order 1\n","physx_box_corners 2\n","physx_speculative_skin -1\n","physx_box_corners 0\nphysx_box_corners 1\n","physx_speculative_skin 1\nphysx_speculative_skin 0\n","physx_suspension_stop 2\n","physx_suspension_stop 1\nphysx_suspension_stop 0\n","physx_contact_skin 2\n","physx_wheel_compat 2\n","physx_wheel_compat -1\n","physx_wheel_compat 1\nphysx_wheel_compat 0\n","gravity_scale nan\n","solver_substeps 3\n","unknown 1\n","spring_scale 0\n","spring_scale 1\nspring_scale 2\n","gravity_scale 1 extra\n"};
 for(const auto* text:invalid){ {std::ofstream f(path);f<<text;}bool rejected=false;try{static_cast<void>(r3d::physics::CalibrationTuning::load());}catch(const std::exception&){rejected=true;}if(!rejected)return 5;}
 std::remove(path);if(previous.empty())unsetenv("RRR3D_CALIBRATION_PROFILE");else setenv("RRR3D_CALIBRATION_PROFILE",previous.c_str(),1);
 std::cout<<"Calibration neutral/default settings, overrides and invalid-profile rejection passed\n";return 0;
}
