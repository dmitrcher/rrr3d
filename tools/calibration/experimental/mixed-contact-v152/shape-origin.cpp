#include <NxPhysics.h>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <cstring>
unsigned word(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
float read(std::ifstream& in){unsigned w;in>>w;if(!in)throw 1;float f;std::memcpy(&f,&w,4);return f;}
void vector(const NxVec3& v){std::cout<<'['<<word(v.x)<<','<<word(v.y)<<','<<word(v.z)<<']';}
int main(int argc,char** argv){NxPhysicsSDK* sdk=0;try{
 if(argc!=2)return 2;std::ifstream in(argv[1]);unsigned count;in>>count;
 sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);if(!sdk)return 3;
 NxSceneDesc sceneDesc;sceneDesc.upAxis=2;NxScene* scene=sdk->createScene(sceneDesc);if(!scene)return 4;
 for(unsigned i=0;i<count;++i){
  NxQuat q;q.x=read(in);q.y=read(in);q.z=read(in);q.w=read(in);
  NxVec3 center;center.x=read(in);center.y=read(in);center.z=read(in);
  NxVec3 local;local.x=read(in);local.y=read(in);local.z=read(in);
  NxVec3 com;com.x=read(in);com.y=read(in);com.z=read(in);
  NxBoxShapeDesc box;box.dimensions=NxVec3(1,1,1);box.localPose.t=local;
  NxBodyDesc body;body.mass=2000;body.massSpaceInertia=NxVec3(1000,1000,1000);body.massLocalPose.t=com;
  NxActorDesc desc;desc.body=&body;desc.shapes.pushBack(&box);desc.flags=NX_AF_LOCK_COM|NX_AF_CONTACT_MODIFICATION;
  NxActor* actor=scene->createActor(desc);if(!actor)return 5;
  actor->setGlobalOrientationQuat(q);actor->setCMassGlobalPosition(center);
  const NxQuat actual=actor->getGlobalOrientationQuat();NxShape* shape=actor->getShapes()[0];
  std::cout<<"{\"case\":"<<i<<",\"actual_quaternion\":["<<word(actual.x)<<','<<word(actual.y)<<','<<word(actual.z)<<','<<word(actual.w)<<"],\"actual_com\":";vector(actor->getCMassGlobalPosition());
  std::cout<<",\"shape_world\":";vector(shape->getGlobalPosition());
  std::cout<<",\"actor_world\":";vector(actor->getGlobalPosition());
  const unsigned base=(unsigned)GetModuleHandleA("PhysXCore.dll");const unsigned* table=*(const unsigned* const*)shape;
  std::cout<<",\"shape_vtable_rvas\":[";for(unsigned j=0;j<80;++j)std::cout<<(j?",":"")<<(table[j]-base);std::cout<<"]}\n";
  scene->releaseActor(*actor);
 }
 sdk->releaseScene(*scene);sdk->release();return 0;
}catch(...){if(sdk)sdk->release();return 6;}}
