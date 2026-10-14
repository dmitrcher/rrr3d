#include "NativeStartupOrientationV129.h"
#include <cstdio>
#include <cstring>
#include <cstdint>
float v(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
int main(){uint32_t u[3];while(std::scanf("%u %u %u",u,u+1,u+2)==3){auto r=motorrock::startupv129::computeHorizontalStartup({{v(u[0]),v(u[1]),v(u[2])}});for(int i=0;i<3;++i)std::printf("%u ",bits(r.direction[i]));for(int i=0;i<3;++i)std::printf("%u ",bits(r.up[i]));for(auto f:r.matrix)std::printf("%u ",bits(f));for(int i=0;i<4;++i)std::printf("%u ",bits(r.quaternion[i]));std::puts("");}}
