#include "NativeSpringSplitV53.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
float value(uint32_t x){float v;std::memcpy(&v,&x,4);return v;}
uint32_t bits(float v){uint32_t x;std::memcpy(&x,&v,4);return x;}
int main(){
 uint32_t fixtures[][8]={
#include "native-zero-preparation-fixtures.inc"
 };
 unsigned failed=0;
 for(const auto& x:fixtures){
  auto c=motorrock_native_spring_v53::prepare(value(x[0]),value(x[1]),value(x[2]),value(x[3]),value(x[4]));
  if(bits(c.mass)!=x[5]||bits(c.softness)!=x[6]||bits(c.position_coefficient)!=x[7])++failed;
 }
 std::printf("4 original zero-spring preparations: %u mismatches\n",failed);
 return failed?1:0;
}
