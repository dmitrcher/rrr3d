#include "NativeWheelSolverInertiaV86.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
using namespace motorrock_native_wheel_inertia_v86;
float value(uint32_t x){float v;std::memcpy(&v,&x,4);return v;}
uint32_t bits(float v){uint32_t x;std::memcpy(&x,&v,4);return x;}
int main(){
#include "native-geometry-fixtures.inc"
 Quaternion q;Matrix local;
 for(unsigned i=0;i<4;++i)q[i]=value(quaternion[i]);
 for(unsigned i=0;i<9;++i)local[i]=value(localWords[i]);
 auto tensor=solver(q,local);unsigned checked=0,failed=0,oldFailed=0;
 for(unsigned i=0;i<9;++i){++checked;if(bits(tensor[i])!=world[i])++failed;}
 for(const auto& r:stops){
  float travel=value(r[0]),radius=value(r[1]),slop=value(r[2]),dt=value(r[3]),mass=value(r[4]);
  float legalLength=add(travel,radius);
  float rate=div(add(sub(legalLength,radius),slop),dt);
  float term=mul(rate,mul(mass,.7f));
  checked+=2;if(bits(rate)!=r[5])++failed;if(bits(term)!=r[6])++failed;
  float oldRate=div(sub(legalLength,sub(radius,slop)),dt);
  if(bits(oldRate)!=r[5])++oldFailed;
 }
 std::printf("Native geometry: %u words, %u mismatches; old radius roundtrip %u rate failures\n",checked,failed,oldFailed);
 return failed||oldFailed!=4?1:0;
}
