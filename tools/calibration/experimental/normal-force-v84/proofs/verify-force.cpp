#include "physics/NativeNormalForceV84.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
float value(uint32_t x) {float f; std::memcpy(&f,&x,4);return f;}
uint32_t bits(float f) {uint32_t x;std::memcpy(&x,&f,4);return x;}
int main() {
 uint32_t rows[][8]={
#include "native-force-fixtures.inc"
 };
 unsigned checked=0,oldDifferent=0;
 uint32_t callbackRows[][8]={
#include "native-callback-fixtures.inc"
 };
 for(auto& r:callbackRows) {
  const auto result=motorrock::normalforcev84::modify(value(r[0]),value(r[7]),value(r[1]),value(r[2]),4,r[6]!=0,value(r[3]));
  if(bits(result.force)!=r[5]){std::fprintf(stderr,"callback force mismatch mass %.9g normal %.9g actual %.9g expected %.9g\n",value(r[7]),value(r[1]),result.force,value(r[5]));return 2;}
  namespace n=motorrock::tirev55;
  const float dt=1.0F/60.0F;
  const float oldLoadImpulse=n::div(n::mul(n::mul(value(r[7]),std::abs(n::mul(value(r[1]),value(r[2])))),dt),4);
  const float oldForce=n::div(n::mul(oldLoadImpulse,1.5F),dt);
  if(r[6] && value(r[3])==0 && bits(oldForce)!=r[5])++oldDifferent;
  if(bits(motorrock::normalforcev84::normalLoad(value(r[7]),value(r[1]),value(r[2]),4))!=r[4])return 3;
  checked+=2;
 }
 for(auto& row:rows) {
  namespace n=motorrock::tirev55;
  float impulse=std::abs(n::impulseScale(value(row[0]),value(row[1]),value(row[2])));
  float old=std::abs(n::impulseScale(n::div(n::mul(value(row[0]),value(row[2])),value(row[2])),value(row[1]),value(row[2])));
  if(bits(old)!=bits(impulse))++oldDifferent;
  float result[]={impulse,n::mul(value(row[3]),impulse),n::mul(value(row[4]),impulse)};
  for(int i=0;i<3;++i){if(bits(result[i])!=row[5+i]){std::fprintf(stderr,"row %u field %d failed\n",checked/3,i);return 1;}++checked;}
 }
 std::printf("Native fixtures: %u exact float32 words; old roundtrip differs in %u records\n",checked,oldDifferent);
 return 0;
}
