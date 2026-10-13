#include <xmmintrin.h>
#include <cstdio>
#include <cstring>
#include "skin-fixtures.inc"
float f(unsigned w){float x;std::memcpy(&x,&w,4);return x;}
int main(){
 for(unsigned i=0;i<sizeof(fixtures)/sizeof(fixtures[0]);++i){
  const unsigned* r=fixtures[i];
  __m128 result=_mm_sub_ss(_mm_set_ss(f(r[0])),_mm_set_ss(f(r[1])));
  result=_mm_add_ss(result,_mm_set_ss(f(r[2])));
  result=_mm_add_ss(result,_mm_set_ss(f(r[3])));
  float value;_mm_store_ss(&value,result);unsigned word;std::memcpy(&word,&value,4);
  std::printf("%u\n",word);
 }
 return 0;
}
