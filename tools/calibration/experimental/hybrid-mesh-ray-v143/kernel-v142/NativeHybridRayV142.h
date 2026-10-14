#pragma once
#include "NativeWheelRayV52.h"
namespace motorrock::hybridrayv142 {
namespace f32=motorrock::wheelrayv52;
using Vector=f32::Vector;
inline float dotYZX(Vector a,Vector b){return f32::add(f32::add(f32::mul(a[1],b[1]),f32::mul(a[2],b[2])),f32::mul(a[0],b[0]));}
struct Result{float determinant;f32::Intersection intersection;};
// Original Core hybrid sequential primitive: determinant Y,Z,X;
// barycentric u, v and distance Z,Y,X. Each SSE store remains Float32.
inline Result compute(Vector origin,Vector direction,Vector a,Vector b,Vector c){
 const auto e1=f32::subtract(b,a),e2=f32::subtract(c,a);
 const auto p=f32::cross(direction,e2),t=f32::subtract(origin,a),q=f32::cross(t,e1);
 const float determinant=dotYZX(p,e1),reciprocal=f32::div(1.0F,determinant);
 return {determinant,{f32::mul(f32::dotZYX(q,e2),reciprocal),f32::mul(f32::dotZYX(t,p),reciprocal),f32::mul(f32::dotZYX(direction,q),reciprocal)}};
}
}
