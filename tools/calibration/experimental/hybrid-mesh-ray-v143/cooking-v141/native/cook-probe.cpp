#include <NxPhysics.h>
#include <NxCooking.h>
#include <windows.h>
#include <cstdio>
#include <vector>
#include "CookStream.h"
static void module(const char* name){char path[MAX_PATH]={0};GetModuleFileNameA(GetModuleHandleA(name),path,MAX_PATH);printf(",\"%s\":\"",name);for(unsigned i=0;path[i];++i){if(path[i]=='\\'||path[i]=='\"')putchar('\\');putchar(path[i]);}putchar('\"');}
int main(){try{
 NxSDKCreateError err;NxPhysicsSDK* sdk=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,0,NxPhysicsSDKDesc(),&err);if(!sdk)return 2;
 NxCookingInterface* c=NxGetCookingLib(NX_PHYSICS_SDK_VERSION);if(!c||!c->NxInitCooking())return 3;
 printf("{\"event\":\"params\",\"hint_speed\":%s",c->NxGetCookingParams().hintCollisionSpeed?"true":"false");module("PhysXCore.dll");module("PhysXCooking.dll");printf("}\n");
 for(unsigned layout=0;layout<2;++layout)for(unsigned n=1;n<=32;++n){
  std::vector<NxVec3> v;std::vector<NxU32> ind;
  for(unsigned j=0;j<n;++j){float x=float(layout?j%4:j)*3,y=float(layout?j/4:0)*3;v.push_back(NxVec3(x,y,0));v.push_back(NxVec3(x+1,y,0));v.push_back(NxVec3(x,y+1,0));ind.push_back(j*3);ind.push_back(j*3+1);ind.push_back(j*3+2);}
  NxTriangleMeshDesc d;d.numVertices=(NxU32)v.size();d.pointStrideBytes=sizeof(NxVec3);d.points=&v[0];d.numTriangles=n;d.triangleStrideBytes=3*sizeof(NxU32);d.triangles=&ind[0];CalibrationStream stream;
  if(!c->NxCookTriangleMesh(d,stream))return 4;NxTriangleMesh* mesh=sdk->createTriangleMesh(stream);if(!mesh)return 5;
  NxTriangleMeshDesc loaded;if(!mesh->saveToDesc(loaded))return 6;
  printf("{\"event\":\"cook\",\"layout\":%u,\"triangles\":%u,\"loaded_triangles\":%u,\"stream_hex\":\"",layout,n,loaded.numTriangles);
  const std::vector<NxU8>& bytes=stream.data();for(unsigned k=0;k<bytes.size();++k)printf("%02x",(unsigned)bytes[k]);printf("\"}\n");sdk->releaseTriangleMesh(*mesh);
 }
 c->NxCloseCooking();sdk->release();return 0;
}catch(...){return 7;}}
