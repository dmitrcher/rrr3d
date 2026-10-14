#include "NativePlacedMeshV126.h"
#include <cstdio>
#include <cstring>
namespace p=motorrock::placedmeshv126;namespace s=motorrock::wheelrayv52;
float value(unsigned w){float f;std::memcpy(&f,&w,4);return f;}unsigned bits(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
int main(){unsigned w[13];while(std::scanf("%u",w)==1){for(unsigned i=1;i<13;++i)if(std::scanf("%u",w+i)!=1)return 2;p::Pose pose;pose.quaternion={{value(w[0]),value(w[1]),value(w[2]),value(w[3])}};pose.matrix=motorrock::physicalposev42::rotation(pose.quaternion);pose.position={{value(w[4]),value(w[5]),value(w[6])}};auto center=p::point(pose,{{value(w[7]),value(w[8]),value(w[9])}});s::Vector e;for(unsigned i=0;i<3;++i)e[i]=s::add(s::add(s::mul(std::abs(pose.matrix.a[i][0]),value(w[10])),s::mul(std::abs(pose.matrix.a[i][1]),value(w[11]))),s::mul(std::abs(pose.matrix.a[i][2]),value(w[12])));for(unsigned i=0;i<3;++i)std::printf("%s%u",i?" ":"",bits(s::sub(center[i],e[i])));for(unsigned i=0;i<3;++i)std::printf(" %u",bits(s::add(center[i],e[i])));std::puts("");}}
