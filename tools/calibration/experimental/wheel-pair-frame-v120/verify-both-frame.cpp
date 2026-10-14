#include "NativeWheelFrameV57.h"
#include <cstdint>
#include <cstring>
#include <iostream>
float f(uint32_t w){float v;std::memcpy(&v,&w,4);return v;}uint32_t bits(float v){uint32_t w;std::memcpy(&w,&v,4);return w;}
int main(){uint32_t w[26];while(std::cin>>w[0]){for(unsigned i=1;i<26;++i)std::cin>>w[i];namespace N=motorrock_native_wheel_frame_v57;for(unsigned b=0;b<2;++b){const unsigned i=b*13;N::Quaternion q{{f(w[i]),f(w[i+1]),f(w[i+2]),f(w[i+3])}};N::Vector p{{f(w[i+4]),f(w[i+5]),f(w[i+6])}},c{{f(w[i+7]),f(w[i+8]),f(w[i+9])}},a{{f(w[i+10]),f(w[i+11]),f(w[i+12])}};auto axis=N::body_axis(q,a),lever=N::body_lever(q,p,c),jac=N::cross(lever,axis);for(auto v:axis)std::cout<<bits(v)<<' ';for(auto v:jac)std::cout<<bits(v)<<' ';}std::cout<<'\n';}}
