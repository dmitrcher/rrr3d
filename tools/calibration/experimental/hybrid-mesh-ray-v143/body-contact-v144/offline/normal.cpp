#include "SourceMeshManifold.h"
#include "NativePlacedMeshV126.h"
#include <cstdio>
#include <cstring>
float read(){unsigned w;if(std::scanf("%u",&w)!=1)throw 1;float f;std::memcpy(&f,&w,4);return f;}
unsigned word(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
int main(){try{while(true){motorrock::contactframe::Quaternion q;for(unsigned j=0;j<4;++j)q[j]=read();motorrock::wheelrayv52::Vector p;for(auto& x:p)x=read();std::array<r3d::physics::sourcemesh::PxVec3,3> tri;for(auto& t:tri){t.x=read();t.y=read();t.z=read();}auto pose=motorrock::placedmeshv126::pose(q,p);auto plane=r3d::physics::sourcemesh::trianglePlane(tri);auto n=motorrock::placedmeshv126::direction(pose,{{plane.n.x,plane.n.y,plane.n.z}});printf("%u %u %u\n",word(n[0]),word(n[1]),word(n[2]));}}catch(int){return 0;}}
