#include "NativeTriangleNormalV101.h"
#include "NativeRayNormalV157.h"
#include "NativePhysicalPoseV42.h"
#include <cstdio>
#include <cstring>
namespace wr=motorrock::wheelrayv52;
float read(){unsigned w;if(scanf("%u",&w)!=1)throw 1;float f;memcpy(&f,&w,4);return f;}unsigned word(float f){unsigned w;memcpy(&w,&f,4);return w;}
int main(){try{
 std::array<wr::Vector,3> vs;for(auto& v:vs)for(auto& x:v)x=read();motorrock::contactframe::Quaternion q;for(unsigned i=0;i<4;++i)q[i]=read();auto matrix=motorrock::physicalposev42::rotation(q);auto t=motorrock::trianglenormalv101::compute(vs);
 wr::Vector before,old;
 for(unsigned i=0;i<3;++i){auto row=wr::Vector{{matrix.a[i][0],matrix.a[i][1],matrix.a[i][2]}};old[i]=wr::dotZYX(row,t.normal);unsigned a=i==1?1:2,b=i==1?2:0,c=i==1?0:1;before[i]=wr::add(wr::add(wr::mul(row[a],t.raw[a]),wr::mul(row[b],t.raw[b])),wr::mul(row[c],t.raw[c]));}
 float square=wr::add(wr::add(wr::mul(before[0],before[0]),wr::mul(before[1],before[1])),wr::mul(before[2],before[2]));float length=float(std::sqrt(double(square))),inverse=length!=0?wr::div(1,length):0;motorrock::placedmeshv126::Pose pose{q,matrix,{{0,0,0}}};auto production=motorrock::raynormalv157::compute(pose,t.raw);for(unsigned i=0;i<3;++i)printf("%u %u %u\n",word(old[i]),word(before[i]),word(production[i]));
 for(unsigned col=0;col<3;++col)for(unsigned row=0;row<3;++row)printf("%u ",word(matrix.a[row][col]));printf("\n");return 0;
}catch(int){return 2;}}
