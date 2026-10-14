#include "NativeVelocityClampV121.h"
#include <iostream>
#include <cstring>
float f(unsigned w){float v;std::memcpy(&v,&w,4);return v;}unsigned bits(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
int main(){unsigned w[4];while(std::cin>>w[0]){for(unsigned i=1;i<4;++i)std::cin>>w[i];for(float v:motorrock_native_velocity_clamp_v121::clamp({f(w[0]),f(w[1]),f(w[2])},f(w[3])))std::cout<<bits(v)<<' ';std::cout<<'\n';}}
