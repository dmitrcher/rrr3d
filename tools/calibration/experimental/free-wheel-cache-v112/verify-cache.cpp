#include "NativeFreeWheelCacheV112.h"
#include <cstdint>
#include <cstring>
#include <iostream>
float f(uint32_t w){float v;std::memcpy(&v,&w,4);return v;}
int main(){uint32_t current,forward,radius,stiffness,valid,flags;
 while(std::cin>>current>>forward>>radius>>stiffness>>valid>>flags){
  float value=motorrock::freewheelcachev112::axle(f(current),f(forward),f(radius),f(stiffness),valid!=0,flags);
  uint32_t word;std::memcpy(&word,&value,4);std::cout<<word<<'\n';
 }
}
