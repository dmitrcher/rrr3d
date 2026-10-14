#include "NativeWheelFrameV57.h"
#include <cstdint>
#include <cstring>
#include <iostream>
float value(uint32_t x){float v;std::memcpy(&v,&x,4);return v;}
uint32_t bits(float v){uint32_t x;std::memcpy(&x,&v,4);return x;}
int main(){
 uint32_t words[13];
 while(std::cin>>words[0]){
  for(int i=1;i<13;++i)if(!(std::cin>>words[i]))return 1;
  namespace n=motorrock_native_wheel_frame_v57;
  n::Quaternion q{{value(words[0]),value(words[1]),value(words[2]),value(words[3])}};
  n::Vector p{{value(words[4]),value(words[5]),value(words[6])}},c{{value(words[7]),value(words[8]),value(words[9])}},a{{value(words[10]),value(words[11]),value(words[12])}};
  auto axis=n::body_axis(q,a),lever=n::body_lever(q,p,c),jac=n::cross(lever,axis);
  for(int i=0;i<3;++i)std::cout<<(i?" ":"")<<bits(jac[i]);std::cout<<"\n";
 }
}
