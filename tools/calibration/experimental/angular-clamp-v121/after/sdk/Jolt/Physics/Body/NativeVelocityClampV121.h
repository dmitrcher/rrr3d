#pragma once
#include <array>
#include <cmath>
namespace motorrock_native_velocity_clamp_v121 {
inline float add(float a,float b){volatile float v=a+b;return v;}
inline float mul(float a,float b){volatile float v=a*b;return v;}
inline float div(float a,float b){volatile float v=a/b;return v;}
// Original SSE stores the XYZ squared norm and limit-squared/norm ratio.
// Its x87 sqrt and component products remain wide until each component store.
inline std::array<float,3> clamp(std::array<float,3> velocity,float limitSquared){
 const float norm=add(add(mul(velocity[0],velocity[0]),mul(velocity[1],velocity[1])),mul(velocity[2],velocity[2]));
 if(norm>limitSquared){const float ratio=div(limitSquared,norm);const double scale=std::sqrt(double(ratio));for(float &value:velocity)value=static_cast<float>(double(value)*scale);}
 return velocity;
}
}
