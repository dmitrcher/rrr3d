#include "NativeWheelRayV52.h"
#include <cstdio>
#include <cstring>
#include <cstdint>
using namespace motorrock::wheelrayv52;
float value(uint32_t n) { float f; std::memcpy(&f, &n, 4); return f; }
uint32_t bits(float f) { uint32_t n; std::memcpy(&n, &f, 4); return n; }
int main() { uint32_t n[18]; while(std::scanf("%u", &n[0]) == 1) {
for(int i=1;i<18;++i) if(std::scanf("%u", &n[i]) != 1) return 2;
Vector a{value(n[0]),value(n[1]),value(n[2])}, b{value(n[3]),value(n[4]),value(n[5])}, c{value(n[6]),value(n[7]),value(n[8])};
Vector o{value(n[9]),value(n[10]),value(n[11])}, d{value(n[12]),value(n[13]),value(n[14])};
auto h = intersect(o,d,a,b,c); auto p = interpolate(a,b,c,value(n[16]),value(n[17]));
auto p2 = interpolate(a,b,c,h.u,h.v);
std::printf("%u %u %u %u %u %u %u %u %u\n",bits(h.distance),bits(h.u),bits(h.v),bits(p[0]),bits(p[1]),bits(p[2]),bits(p2[0]),bits(p2[1]),bits(p2[2]));
} }
