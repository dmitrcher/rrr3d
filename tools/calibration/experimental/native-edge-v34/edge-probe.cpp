#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "SourceMeshManifold.h"
namespace sm=r3d::physics::sourcemesh;
void vector(const char* name,sm::PxVec3 p){std::printf("\"%s\":[%.9g,%.9g,%.9g]",name,p.x,p.y,p.z);}
int main(){float a[18];for(;;){int n=0;for(;n<18;++n)if(std::scanf("%f",&a[n])!=1)break;if(!n)return 0;if(n!=18)return 2;sm::PxVec3 v1{a[0],a[1],a[2]},p1{a[3],a[4],a[5]},p2{a[6],a[7],a[8]},dir{a[9],a[10],a[11]},p3{a[12],a[13],a[14]},p4{a[15],a[16],a[17]};const auto normal=sm::nativeedge::unit(v1.cross(dir)),v2=sm::nativeedge::unit(p4-p3);const sm::PxPlane plane(normal,-sm::nativeedge::dot(normal,p1));unsigned i=1,j=2;if(std::abs(normal.y)>std::abs(normal.x)&&std::abs(normal.y)>std::abs(normal.z))i=0;else if(std::abs(normal.z)>std::abs(normal.x)){i=0;j=1;}const float coefficient=sm::nativeedge::divide(1,sm::nativeedge::sub(sm::nativeedge::mul(v1[i],dir[j]),sm::nativeedge::mul(v1[j],dir[i])));float distance=0;sm::PxVec3 point;const bool accepted=sm::nativeedge::intersect(v1,plane,p1,p2,dir,p3,p4,distance,point,i,j,coefficient);std::printf("{");vector("plane_normal",normal);std::printf(",");vector("normalized_direction",v2);std::printf(",");vector("clipped_point",point);std::printf(",\"plane_constant\":%.9g,\"coefficient\":%.9g,\"distance\":%.9g,\"accepted\":%s}\n",plane.d,coefficient,distance,accepted?"true":"false");}}
