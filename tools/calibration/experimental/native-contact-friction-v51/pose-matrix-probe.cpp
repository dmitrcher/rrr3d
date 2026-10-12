#include <Jolt/Jolt.h>
#include <Jolt/Math/Mat44.h>
#include <fstream>
#include <iostream>
#include <cstring>
static unsigned failed=0;
static bool observe(const char*,const char*,const char*,JPH::uint){++failed;return false;}
namespace JPH { AssertFailedFunction AssertFailed=observe; }
int main(int argc,char**argv){if(argc!=3&&argc!=4)return 2;const bool enabled=std::strcmp(argv[2],"1")==0;setenv("RRR3D_EXPERIMENT_NATIVE_NONUNIT_POSE_MATRIX",argv[2],1);setenv("RRR3D_EXPERIMENT_NATIVE_ANGULAR_INTEGRATION",argc==4?argv[3]:"1",1);std::ifstream in(argv[1]);uint32_t u[4];unsigned rows=0;
while(in>>u[0]>>u[1]>>u[2]>>u[3]){float q[4];std::memcpy(q,u,16);failed=0;auto m=JPH::Mat44::sRotation(JPH::Quat(-q[0],-q[2],-q[1],q[3]));auto rotation=JPH::Quat(-q[0],-q[2],-q[1],q[3]);const JPH::Vec3 v(1,2,3);const JPH::Vec3 vectors[]={rotation*v,rotation.InverseRotate(v),rotation.RotateAxisX(),rotation.RotateAxisY(),rotation.RotateAxisZ()};std::cout<<failed;for(unsigned c=0;c<4;++c)for(unsigned r=0;r<4;++r){float v=m(r,c);uint32_t w;std::memcpy(&w,&v,4);std::cout<<' '<<w;}for(auto v:vectors)for(unsigned j=0;j<3;++j){float a=v[j];uint32_t w;std::memcpy(&w,&a,4);std::cout<<' '<<w;}std::cout<<'\n';++rows;}
// Explicitly invalid inputs must still reach the assertion in both modes.
for(auto q:{JPH::Quat(0,0,0,2),JPH::Quat(NAN,0,0,1)}){failed=0;(void)JPH::Mat44::sRotation(q);(void)(q*JPH::Vec3(1,2,3));(void)q.InverseRotate(JPH::Vec3(1,2,3));(void)q.RotateAxisX();(void)q.RotateAxisY();(void)q.RotateAxisZ();if(failed!=6)return 4;}
return rows==3?0:3;}
