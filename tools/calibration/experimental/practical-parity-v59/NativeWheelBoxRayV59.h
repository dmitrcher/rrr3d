#pragma once
#include "NativeWheelRayV52.h"
#include <cfloat>
#include <cmath>
namespace motorrock::wheelboxrayv59 {
namespace f32=motorrock::wheelrayv52;
using Vector=f32::Vector;
struct Box { Vector half,position; std::array<Vector,3> columns; };
struct Hit { bool hit=false; float distance=0; Vector point{},normal{}; };
// Measured independently in the original SDK: only exactly parallel axes
// compare the origin against F32(half_extent + 1e-5), including at extent 200.
// Non-parallel slab intersections keep the actual, unexpanded dimensions.
inline constexpr float parallel_bounds_epsilon=1.0e-5f;
inline float dot(const Vector &a,const Vector &b) {
 return f32::add(f32::add(f32::mul(a[0],b[0]),f32::mul(a[1],b[1])),f32::mul(a[2],b[2]));
}
inline Hit cast(const Box &box,const Vector &start,const Vector &direction,float length) {
 const auto delta=f32::subtract(start,box.position);
 Vector local_start{},local_direction{};
 for(unsigned i=0;i<3;++i){local_start[i]=dot(delta,box.columns[i]);local_direction[i]=dot(direction,box.columns[i]);}
 float near=-FLT_MAX,far=FLT_MAX,normal_sign=1;unsigned normal_axis=0;
 for(unsigned i=0;i<3;++i) {
  if(local_direction[i]==0.0f) {
   const float parallel_bound=f32::add(box.half[i],parallel_bounds_epsilon);
   if(local_start[i]<-parallel_bound||local_start[i]>parallel_bound)return {};
   continue;
  }
  float a=f32::div(f32::sub(-box.half[i],local_start[i]),local_direction[i]);
  float b=f32::div(f32::sub(box.half[i],local_start[i]),local_direction[i]);
  float sign=-1;
  if(a>b){float temporary=a;a=b;b=temporary;sign=1;}
  if(a>near){near=a;normal_axis=i;normal_sign=sign;}
  if(b<far)far=b;
  if(near>far)return {};
 }
 // Original Nx box queries do not return a hit when the ray starts inside.
 if(far<0||near<0||near>length)return {};
 Hit result;result.hit=true;result.distance=near;
 for(unsigned i=0;i<3;++i){
  result.point[i]=f32::add(start[i],f32::mul(direction[i],result.distance));
  result.normal[i]=f32::mul(box.columns[normal_axis][i],normal_sign);
 }
 return result;
}
}
