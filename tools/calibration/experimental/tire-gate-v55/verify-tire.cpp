#include "NativeTireV55.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace motorrock::tirev55;
static unsigned checked=0;static uint32_t word(){uint32_t x;if(std::fread(&x,4,1,stdin)!=1)std::abort();return x;}static float value(){auto x=word();float f;std::memcpy(&f,&x,4);return f;}
template<size_t N> std::array<float,N> input(){std::array<float,N> a;for(auto& f:a)f=value();return a;}
static void expect(float f){uint32_t b;std::memcpy(&b,&f,4);auto e=word();++checked;if(b!=e){std::fprintf(stderr,"word%u %08x != %08x\n",checked,b,e);std::exit(1);}}
template<size_t N> void expect(std::array<float,N> a){for(auto f:a)expect(f);}
int main(){unsigned n=word();for(unsigned i=0;i<n;++i){auto p=input<4>();float slip=value();expect(curve(slip,p[0],p[1],p[2],p[3]));}
n=word();for(unsigned i=0;i<n;++i){auto p=input<12>();auto normal=input<3>();auto b=basisZeroSteer(p,normal);expect(b.lateral);expect(b.longitudinal);expect(dotZYX(b.suspension,normal));auto pt=input<3>(),old=input<3>();float skin=value(),axle=value();auto a=anchor(pt,old,b.lateral,b.longitudinal,skin,axle);if(a.update!=bool(word()))return 2;expect(a.lateral_error);expect(a.longitudinal_error);}
n=word();for(unsigned i=0;i<n;++i){float force=value(),projection=value(),dt=value(),lat=value(),lng=value();float scale=impulseScale(force,projection,dt);expect(scale);expect(mul(scale,lat));expect(mul(scale,lng));}std::printf("%u original float32 words exact\n",checked);}
