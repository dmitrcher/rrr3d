#include <iostream>
#include <iomanip>
#ifdef _MSC_VER
#include <float.h>
#endif
#include "SourceMeshManifold98.h"
using namespace r3d::physics::sourcemesh;
int main(){
#ifdef _MSC_VER
 const char* bits=std::getenv("RRR3D_GEOMETRY_X87_BITS");
 if(bits && std::strcmp(bits,"24")==0)_controlfp(_PC_24,_MCW_PC);
 if(bits && std::strcmp(bits,"53")==0)_controlfp(_PC_53,_MCW_PC);
 if(bits && std::strcmp(bits,"64")==0)_controlfp(_PC_64,_MCW_PC);
 std::cerr<<"floating_control="<<_controlfp(0,0)<<std::endl;
#endif
 std::cout<<std::setprecision(9);float f;while(std::cin>>f){PxVec3 half(f,0,0);std::cin>>half.y>>half.z;Cm::Matrix34 box;std::cin>>box.p.x>>box.p.y>>box.p.z;
 for(int c=0;c<3;++c)for(int r=0;r<3;++r)std::cin>>box.m(r,c);
 int n;std::cin>>n;ContactBuffer out;
 for(int t=0;t<n;++t){unsigned index;std::cin>>index;FixedArray<PxVec3,3> tri;for(int v=0;v<3;++v)std::cin>>tri[v].x>>tri[v].y>>tri[v].z;faceContacts(out,half,box,tri,index);}
 std::cout<<out.count;for(unsigned p=0;p<out.count;++p){const ContactPoint& c=out.contacts[p];std::cout<<' '<<c.point.x<<' '<<c.point.y<<' '<<c.point.z<<' '<<c.normal.x<<' '<<c.normal.y<<' '<<c.normal.z<<' '<<c.separation<<' '<<c.internalFaceIndex1;}std::cout<<'\n';}
}
