#include "SourceMeshManifold.h"
#include <cstdio>
#include <cstring>
namespace sm=r3d::physics::sourcemesh;
float read(){unsigned w;if(scanf("%u",&w)!=1)throw 1;float f;memcpy(&f,&w,4);return f;}
unsigned word(float f){unsigned w;memcpy(&w,&f,4);return w;}
sm::PxVec3 vec(){const float x=read(),y=read(),z=read();return {x,y,z};}
sm::Cm::Matrix34 mat(){auto x=vec(),y=vec(),z=vec(),p=vec();return {{x,y,z},p};}
int main(){try{
 auto half=vec();auto box=mat(),world=mat();std::array<sm::PxVec3,3> triangle={vec(),vec(),vec()};auto inverse=box.getInverseRT();auto plane=sm::trianglePlane(triangle);
 for(int sat=0;sat<2;++sat)for(unsigned mask=0;mask<8;++mask){
  setenv("RRR3D_EXPERIMENT_NATIVE_BOX_TRIANGLE_SAT_V60",sat?"1":"0",1);
  auto c=sm::boxtrianglesatv60::select(half,box,triangle,mask);
  sm::ContactBuffer out;sm::faceContacts(out,half,box,triangle,12,true,mask,&inverse,&plane,&world);
  auto local=box.rotateTranspose(-c.axis);int axis=0;if(std::abs(local.y)>std::abs(local.x))axis=1;if(std::abs(local.z)>std::abs(local[axis]))axis=2;sm::PxVec3 normal(0);normal[axis]=local[axis]<0?-1.f:1.f;
  printf("mode %d mask %u separated %d triangleFace %d axis %u %u %u hull_alignment %.9g triangle_alignment %.9g depth %.9g contacts %u\n",sat,mask,c.separated,c.triangleFace,word(c.axis.x),word(c.axis.y),word(c.axis.z),std::abs(box.rotate(normal).dot(c.axis)),std::abs(plane.n.dot(c.axis)),c.depth,out.count);
  for(unsigned i=0;i<out.count;++i){auto p=out.contacts[i];printf("p %u %u %u %u\n",word(p.point.x),word(p.point.y),word(p.point.z),word(p.separation));}
 }
 return 0;
}catch(int){return 2;}}
