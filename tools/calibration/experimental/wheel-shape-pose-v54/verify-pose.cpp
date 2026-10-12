#include "NativeWheelPoseV54.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace motorrock::wheelposev54;
static unsigned checked=0;
static uint32_t word() { uint32_t x; if(std::fread(&x,4,1,stdin)!=1)std::abort();return x; }
static float value() { auto x=word();float f;std::memcpy(&f,&x,4);return f; }
template<size_t N> std::array<float,N> input() {std::array<float,N> a;for(auto& f:a)f=value();return a;}
template<size_t N> void expect(std::array<float,N> a) {for(auto f:a){uint32_t got;std::memcpy(&got,&f,4);auto wanted=word();++checked;if(got!=wanted){std::fprintf(stderr,"word %u: %08x != %08x\n",checked,got,wanted);std::exit(1);}}}
int main() {
 unsigned n=word();for(unsigned i=0;i<n;++i) {auto parent=input<7>(),local=input<7>();expect(compose(parent,local));}
 n=word();for(unsigned i=0;i<n;++i) {auto parent=input<7>(),local=input<7>();float radius=value(),travel=value();auto p=pose(compose(parent,local));expect(p);auto r=ray(p,radius,travel);expect(r.start);expect(r.end);if(word())expect(r.unit);}
 std::printf("%u original float32 words exact\n",checked);
}
