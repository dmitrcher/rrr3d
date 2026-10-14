#include "NativeSteeringTireV118.h"
#include <cstdint>
#include <cstring>
#include <iostream>
float value(uint32_t x){float v;std::memcpy(&v,&x,4);return v;}
uint32_t bits(float v){uint32_t x;std::memcpy(&x,&v,4);return x;}
int main(){uint32_t w[7];while(std::cin>>w[0]){
 for(int i=1;i<7;++i)if(!(std::cin>>w[i]))return 1;
 motorrock::wheelposev54::Pose pose{};
 for(int i=0;i<3;++i)pose[i*3]=value(w[1+i]);
 pose[7]=1.0F;
 auto b=motorrock::steeringtirev118::basis(pose,{{0,0,1}},value(w[0]));
 for(float v:b.lateral)std::cout<<bits(v)<<" ";
 for(float v:b.longitudinal)std::cout<<bits(v)<<" ";
 std::cout<<bits(static_cast<float>(std::sin(double(value(w[0])))))<<" "<<bits(static_cast<float>(std::cos(double(value(w[0])))))<<"\n";
}}
