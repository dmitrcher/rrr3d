#include "SourceBoxManifold.h"
#include <cstring>
#include <cstdio>
using namespace r3d::physics::sourcebox;
static unsigned inputs[][31]={{1065353216u,0u,0u,0u,1064336059u,1051693133u,0u,3199176781u,1064336059u,1090519040u,0u,1097072640u,1065353216u,0u,0u,0u,1065353216u,0u,0u,0u,1065353216u,1084367395u,0u,1097090134u,1069547520u,1061997773u,1050253722u,1069547520u,1061997773u,1050253722u,0u},{1065353216u,0u,0u,0u,1064336059u,1051693133u,0u,3199176781u,1064336059u,1090519040u,0u,1097072640u,1065353216u,0u,0u,0u,1065353216u,0u,0u,0u,1065353216u,1084367395u,0u,1097090134u,1069547520u,1061997773u,1050253722u,1069547520u,1061997773u,1050253722u,0u}};
float f(unsigned w){float v;std::memcpy(&v,&w,4);return v;}
unsigned bits(float f){unsigned v;std::memcpy(&v,&f,4);return v;}
PxVec3 v(unsigned *p){return {f(p[0]),f(p[1]),f(p[2])};}
Cm::Matrix34 matrix(unsigned*p){return {{v(p),v(p+3),v(p+6)},v(p+9)};}
int main(){for(auto &w:inputs){ContactBuffer points;unsigned axis=w[30];contacts(points,v(w+24),v(w+27),matrix(w),matrix(w+12),axis,0,true);std::printf("%u",axis);for(unsigned i=0;i<points.count;++i){auto c=points.contacts[i];for(float f:{c.point.x,c.point.y,c.point.z,c.normal.x,c.normal.y,c.normal.z,c.separation})std::printf(" %u",bits(f));}std::puts("");}}