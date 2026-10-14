#include "NativeLocalAngularAccelerationV114.h"
#include <iostream>
#include <cstdint>
#include <cstring>
float f(uint32_t w){float v;std::memcpy(&v,&w,4);return v;}
int main(){uint32_t w[7];while(std::cin>>w[0]){
 for(unsigned i=1;i<7;++i)std::cin>>w[i];
 auto result=motorrock::localangularv114::world({{f(w[0]),f(w[1]),f(w[2]),f(w[3])}},{{f(w[4]),f(w[5]),f(w[6])}});
 for(float v:result){uint32_t word;std::memcpy(&word,&v,4);std::cout<<word<<' ';}std::cout<<'\n';
}}
