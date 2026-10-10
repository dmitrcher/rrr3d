#include "Fixture.h"
#include "physics/OriginalVehiclePhysics.h"
#include <iostream>
#include <map>
namespace cal=motorrock::calibration;
namespace phys=r3d::physics;
static phys::Vec3 vec(cal::V v){return {float(v.x),float(v.y),float(v.z)};}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 unsetenv("RRR3D_CALIBRATION_PROFILE");
 std::ifstream input(argv[1]);std::string line;
 std::map<std::string,cal::V> oracle;
 while(std::getline(input,line)){if(line.empty() || line[0]=='#')continue;std::istringstream row(line);std::string name;cal::V value;if(!(row>>name>>value.x>>value.y>>value.z))return 3;oracle[name]=value;}
 if(oracle.size()!=17)return 4;
 size_t checked=0;
 for(const auto& c:cal::cases()){
  if(c.name!=c.catalog+"/spin-mixed")continue;
  phys::WorldDescription d;d.gravity=0;d.startPosition={0,0,100};
  phys::TriangleMesh floor;floor.vertices={{-500,-500,-10000},{500,-500,-10000},{500,500,-10000},{-500,500,-10000}};floor.indices={0,1,2,0,2,3};d.collisionMeshes.push_back(floor);
  auto& v=d.vehicle;v.mass=float(c.mass);v.halfExtents=vec(c.halfExtents);v.shapePosition=vec(c.shapePosition);v.centerOfMass=vec(c.centerOfMass);v.bodySleepEnergyThreshold=0;v.brakeTorque=4000;v.maximumTorque=600;v.maximumRpm=7000;v.maximumSpeed=100;v.differentialRatio=3.42F;v.steerAngle=.5235988F;v.restBrakeTorque=0;v.airbornePitchAcceleration=0;
  for(const auto& cw:c.wheels){phys::WheelDescription w;w.position=vec(cw.position);w.radius=float(cw.radius);w.width=.2F;w.suspensionTravel=float(cw.travel);w.spring=float(cw.spring);w.damper=float(cw.damper);w.inverseMass=float(cw.inverseMass);w.flags=cw.flags;w.driven=cw.driven;w.steering=cw.steering;v.wheels.push_back(w);}
  std::string error;auto world=phys::createOriginalVehicleWorld(d,error);if(!world){std::cerr<<error;return 5;}
  const auto found=oracle.find(c.catalog);if(found==oracle.end())return 6;
  const double expected[]={found->second.x,found->second.y,found->second.z};
  for(int axis=0;axis<3;++axis){world->reset();phys::Vec3 omega{};if(axis==0)omega.x=1;else if(axis==1)omega.y=1;else omega.z=1;world->addAngularVelocity(0,omega);world->step(1.0F/60,phys::VehicleInput{});const auto momentum=world->vehicle().angularMomentum;double actual[]={momentum.x,momentum.y,momentum.z};double reference=expected[axis]*(1.0-.05/60);if(std::abs(actual[axis]-reference)>reference*2e-5){std::cerr<<c.catalog<<" axis "<<axis<<": "<<actual[axis]<<" vs Nx "<<reference<<'\n';return 7;}}
  ++checked;
 }
 if(checked!=17)return 8;
 std::cout<<"All 17 production chassis inertia tensors match the independent Nx SDK oracle\n";
 return 0;
}
