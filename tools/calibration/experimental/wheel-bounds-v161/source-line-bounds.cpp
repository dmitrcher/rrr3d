#include "NativeWheelPoseV54.h"
#include "NativeQuaternionPoseV43.h"
#include <cstdio>
#include <cstring>
namespace wp=motorrock::wheelposev54;
float value(unsigned w){float v;std::memcpy(&v,&w,4);return v;}
unsigned word(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
int main(){unsigned w[15];while(std::scanf("%u",w)==1){for(unsigned i=1;i<15;++i)if(std::scanf("%u",w+i)!=1)return 2;const auto p=motorrock::quaternionposev43::origin({{value(w[0]),value(w[1]),value(w[2]),value(w[3])}},{{value(w[4]),value(w[5]),value(w[6])}},{{value(w[7]),value(w[8]),value(w[9])}});const auto ray=wp::ray(wp::pose(wp::compose({{p[0],p[1],p[2],value(w[0]),value(w[1]),value(w[2]),value(w[3])}},{{value(w[10]),value(w[11]),value(w[12]),.5f,.5f,.5f,.5f}})),value(w[13]),value(w[14]));for(unsigned a=0;a<3;++a)std::printf("%s%u",a?" ":"",word(std::min(ray.start[a],ray.end[a])));for(unsigned a=0;a<3;++a)std::printf(" %u",word(std::max(ray.start[a],ray.end[a])));std::puts("");}}
