#include "SourceMeshManifold.h"
#include <cstdio>
#include <cstring>
namespace sm=r3d::physics::sourcemesh;
float rd(){unsigned w;if(std::scanf("%u",&w)!=1)throw 1;float f;std::memcpy(&f,&w,4);return f;}
sm::PxVec3 vec(){float x=rd(),y=rd(),z=rd();return {x,y,z};}
unsigned word(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
int main(){try{
 unsigned n;if(std::scanf("%u",&n)!=1)return 2;
 for(unsigned i=0;i<n;++i){
  auto half=vec(),a=vec(),b=vec(),c=vec(),p=vec();sm::Cm::Matrix34 box{{a,b,c},p};
  std::array<sm::PxVec3,3> tri;for(auto& v:tri)v=vec();
  for(unsigned mask:{3u,7u}){
   const auto r=sm::boxtrianglesatv60::select(half,box,tri,mask);
   std::printf("%u %u %u %u %u %u %u\n",mask,r.separated,r.triangleFace,word(r.axis.x),word(r.axis.y),word(r.axis.z),word(r.depth));
  }
 }
 return 0;
}catch(int){return 2;}}
