#include "NativeWheelPoseV54.h"
#include "NativeQuaternionPoseV43.h"
#include "NativeShapeCenterV153.h"
#include "NativePhysicalPoseV42.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
namespace wp=motorrock::wheelposev54;
float value(unsigned w){float v;std::memcpy(&v,&w,4);return v;}
unsigned word(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
int main(int argc,char**argv){const int mode=argc>1?std::atoi(argv[1]):0;unsigned w[15];while(std::scanf("%u",w)==1){for(unsigned i=1;i<15;++i)if(std::scanf("%u",w+i)!=1)return 2;const auto p=motorrock::quaternionposev43::origin({{value(w[0]),value(w[1]),value(w[2]),value(w[3])}},{{value(w[4]),value(w[5]),value(w[6])}},{{value(w[7]),value(w[8]),value(w[9])}});const auto ray=wp::ray(wp::pose(wp::compose({{p[0],p[1],p[2],value(w[0]),value(w[1]),value(w[2]),value(w[3])}},{{value(w[10]),value(w[11]),value(w[12]),.5f,.5f,.5f,.5f}})),value(w[13]),value(w[14]));auto low=ray.start,high=ray.end;
if(mode){namespace f=motorrock::derivederror;const motorrock::contactframe::Quaternion q={{value(w[0]),value(w[1]),value(w[2]),value(w[3])}};motorrock::contactframe::Vector offset;for(unsigned a=0;a<3;++a)offset[a]=f::sub(value(w[10+a]),value(w[7+a]));const auto center=motorrock::shapecenterv153::center(q,{{value(w[4]),value(w[5]),value(w[6])}},offset);const auto shape=wp::compose({{0,0,0,q[0],q[1],q[2],q[3]}},{{0,0,0,.5f,.5f,.5f,.5f}});const auto basis=motorrock::physicalposev42::rotation({{shape[3],shape[4],shape[5],shape[6]}});const auto oldBasis=wp::pose(shape);const float length=f::add(value(w[13]),value(w[14]));
for(unsigned a=0;a<3;++a){const float axis=mode==1?oldBasis[3*a+1]:basis.a[a][1];if(mode<3){low[a]=center[a];high[a]=f::sub(center[a],f::mul(axis,length));}else{const float half=f::mul(length,.5f),mid=f::sub(center[a],f::mul(axis,half)),extent=f::mul(std::abs(axis),half);low[a]=f::sub(mid,extent);high[a]=f::add(mid,extent);}}}
for(unsigned a=0;a<3;++a)std::printf("%s%u",a?" ":"",word(std::min(low[a],high[a])));for(unsigned a=0;a<3;++a)std::printf(" %u",word(std::max(low[a],high[a])));std::puts("");}}
