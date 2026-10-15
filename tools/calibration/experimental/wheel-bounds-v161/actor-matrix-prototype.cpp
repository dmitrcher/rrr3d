#include "NativeWheelPoseV54.h"
#include "NativeQuaternionPoseV43.h"
#include "NativePhysicalPoseV42.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
namespace wp=motorrock::wheelposev54;namespace f=motorrock::derivederror;
float value(unsigned w){float v;std::memcpy(&v,&w,4);return v;}
unsigned word(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
int main(int argc,char**argv){const bool physical=argc>1&&std::atoi(argv[1])==2;unsigned w[15];while(std::scanf("%u",w)==1){for(unsigned i=1;i<15;++i)if(std::scanf("%u",w+i)!=1)return 2;motorrock::contactframe::Quaternion q={{value(w[0]),value(w[1]),value(w[2]),value(w[3])}};const auto origin=motorrock::quaternionposev43::origin(q,{{value(w[4]),value(w[5]),value(w[6])}},{{value(w[7]),value(w[8]),value(w[9])}});const auto pub=wp::pose({{0,0,0,q[0],q[1],q[2],q[3]}});auto m=motorrock::physicalposev42::rotation(q);if(!physical)for(unsigned r=0;r<3;++r)for(unsigned c=0;c<3;++c)m.a[r][c]=pub[3*r+c];motorrock::contactframe::Vector pos;for(unsigned a=0;a<3;++a)pos[a]=f::add(origin[a],f::add(f::add(f::mul(m.a[a][0],value(w[10])),f::mul(m.a[a][1],value(w[11]))),f::mul(m.a[a][2],value(w[12]))));for(unsigned a=0;a<3;++a)std::printf("%s%u",a?" ":"",word(origin[a]));const unsigned cols[]={1,2,0};for(unsigned c:cols)for(unsigned a=0;a<3;++a)std::printf(" %u",word(m.a[a][c]));for(unsigned a=0;a<3;++a)std::printf(" %u",word(pos[a]));const float half=f::mul(f::add(value(w[13]),value(w[14])),.5f);motorrock::contactframe::Vector mid,extent;for(unsigned a=0;a<3;++a){mid[a]=f::sub(pos[a],f::mul(m.a[a][2],half));extent[a]=f::mul(std::abs(m.a[a][2]),half);}for(unsigned a=0;a<3;++a)std::printf(" %u",word(f::sub(mid[a],extent[a])));for(unsigned a=0;a<3;++a)std::printf(" %u",word(f::add(mid[a],extent[a])));std::puts("");}}
