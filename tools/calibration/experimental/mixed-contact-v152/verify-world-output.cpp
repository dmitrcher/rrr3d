#include "SourceMeshManifold.h"
#include <cstdio>
#include <cstring>
namespace sm=r3d::physics::sourcemesh;
float read(){unsigned w;if(std::scanf("%u",&w)!=1)throw 1;float f;std::memcpy(&f,&w,4);return f;}
unsigned word(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
sm::PxVec3 vec(){float x=read(),y=read(),z=read();return {x,y,z};}
int main(){try{
 const auto half=vec();auto x=vec(),y=vec(),z=vec(),p=vec();
 const sm::Cm::Matrix34 world{{x,y,z},p};
 for(unsigned mode=0;mode<2;++mode)for(unsigned mask=0;mask<8;++mask){
  const sm::PxVec3 point{mask&1?half.x:-half.x,mask&2?half.y:-half.y,mask&4?half.z:-half.z};
  auto v=world.transform(point);
  if(mode)for(unsigned r=0;r<3;++r)v[r]=sm::nativeedge::add(sm::nativeedge::add(sm::nativeedge::add(sm::nativeedge::mul(world.m.column2[r],point.z),sm::nativeedge::mul(world.m.column1[r],point.y)),sm::nativeedge::mul(world.m.column0[r],point.x)),world.p[r]);
  std::printf("%u %u %u\n",word(v.x),word(v.y),word(v.z));
 }
 return 0;
}catch(int){return 2;}}
