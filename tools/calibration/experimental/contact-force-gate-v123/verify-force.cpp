#include "NativeContactForceGateV123.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
float value(uint32_t u){float f;std::memcpy(&f,&u,4);return f;}
uint32_t bits(float f){uint32_t u;std::memcpy(&u,&f,4);return u;}
int main(){uint32_t u[3];while(std::scanf("%u %u %u",u,u+1,u+2)==3){std::array<float,3> f{{value(u[0]),value(u[1]),value(u[2])}};std::printf("%u %u\n",bits(motorrock_native_force_gate_v123::squared_norm(f)),unsigned(motorrock_native_force_gate_v123::should_apply(f)));}}
