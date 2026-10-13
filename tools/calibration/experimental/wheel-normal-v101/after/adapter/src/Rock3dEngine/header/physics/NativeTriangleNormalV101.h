#pragma once
#include "NativeWheelRayV52.h"
namespace motorrock::trianglenormalv101 {
namespace wr=wheelrayv52;
struct Result {wr::Vector raw,normal;float length,inverse;};
// NxTriangle::normal -> NxVec3::normalize: component stores, float length,
// rounded reciprocal, then component multiplication. Actor rotation follows.
inline Result compute(const std::array<wr::Vector,3>& vertices) {
 const auto raw=wr::cross(wr::subtract(vertices[1],vertices[0]),wr::subtract(vertices[2],vertices[0]));
 const float square=wr::add(wr::add(wr::mul(raw[0],raw[0]),wr::mul(raw[1],raw[1])),wr::mul(raw[2],raw[2]));
 const float length=static_cast<float>(std::sqrt(static_cast<double>(square)));
 const float inverse=length!=0?wr::div(1.0F,length):0.0F;
 wr::Vector normal=raw;if(length!=0)for(auto& v:normal)v=wr::mul(v,inverse);
 return {raw,normal,length,inverse};
}
}
