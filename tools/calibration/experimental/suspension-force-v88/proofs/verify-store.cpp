#include "NativeSuspensionForceV88.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
float value(uint32_t x){float v;std::memcpy(&v,&x,4);return v;}
uint32_t bits(float v){uint32_t x;std::memcpy(&x,&v,4);return x;}
int main(){uint32_t fixtures[][3]={
#include "native-store-fixtures.inc"
};unsigned checked=0,failed=0,oldFailed=0;
for(const auto& r:fixtures){float impulse=value(r[0]),step=value(r[1]);if(bits(motorrock::suspensionforcev88::store(impulse,step))!=r[2])++failed;volatile float old=impulse/step;if(bits(old)!=r[2])++oldFailed;++checked;}
std::printf("Native4force stores: %u words, %u mismatches; old division %u failures\n",checked,failed,oldFailed);return failed||oldFailed!=1?1:0;}
