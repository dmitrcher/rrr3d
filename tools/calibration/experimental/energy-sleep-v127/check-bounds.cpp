#include "NativePlacedMeshV126.h"
#include <cstdio>
#include <cstring>
#include <cstdint>
namespace p=motorrock::placedmeshv126;namespace s=motorrock::wheelrayv52;
float value(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
int main(){uint32_t w[13];while(std::scanf("%u",w)==1){for(unsigned i=1;i<13;++i)if(std::scanf("%u",w+i)!=1)return 2;auto pose=p::pose({{value(w[0]),value(w[1]),value(w[2]),value(w[3])}},{{value(w[4]),value(w[5]),value(w[6])}});s::Vector center,extent;for(unsigned i=0;i<3;++i){center[i]=s::mul(s::add(value(w[7+i]),value(w[10+i])),.5F);extent[i]=s::mul(s::sub(value(w[10+i]),value(w[7+i])),.5F);}center=p::point(pose,center);s::Vector e;for(unsigned i=0;i<3;++i)e[i]=s::add(s::add(s::mul(std::abs(pose.matrix.a[i][0]),extent[0]),s::mul(std::abs(pose.matrix.a[i][1]),extent[1])),s::mul(std::abs(pose.matrix.a[i][2]),extent[2]));for(unsigned i=0;i<3;++i)std::printf("%s%u",i?" ":"",bits(s::sub(center[i],e[i])));for(unsigned i=0;i<3;++i)std::printf(" %u",bits(s::add(center[i],e[i])));std::puts("");}}
