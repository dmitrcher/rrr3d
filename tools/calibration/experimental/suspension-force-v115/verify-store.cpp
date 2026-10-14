#include "NativeSuspensionForceV115.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
float value(uint32_t x){float v;std::memcpy(&v,&x,4);return v;}
uint32_t bits(float v){uint32_t x;std::memcpy(&x,&v,4);return x;}
int main(){
 uint32_t fixtures[][4]={
#include "native-force-fixtures.inc"
 };
 unsigned failed=0,oldFailed=0;
 for(const auto& r:fixtures){
  float spring=value(r[0]),stop=value(r[1]),inverse=value(r[2]);
  volatile float step=1.0f/inverse;
  if(bits(motorrock::suspensionforcev115::store(spring,stop,step))!=r[3])++failed;
  volatile float sum=spring+stop;
  if(bits(float(double(sum)*double(inverse)))!=r[3])++oldFailed;
 }
 std::printf("12 Native force stores: %u mismatches; old sum-first %u mismatches\n",failed,oldFailed);
 return failed||oldFailed!=5?1:0;
}
