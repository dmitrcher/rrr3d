#pragma once
#include <array>
namespace motorrock::nativepublicangular {
inline float add(float a,float b){volatile float v=a+b;return v;}
inline float sub(float a,float b){volatile float v=a-b;return v;}
inline float mul(float a,float b){volatile float v=a*b;return v;}
using Matrix=std::array<std::array<float,3>,3>;
inline Matrix orientation(std::array<float,4> q){
 const auto x=q[0],y=q[1],z=q[2],w=q[3];
 const auto xx=mul(mul(x,x),2),yy=mul(mul(y,y),2),zz=mul(mul(z,z),2);
 const auto xy=mul(mul(y,x),2),xz=mul(mul(z,x),2),yz=mul(mul(z,y),2);
 const auto wx=mul(mul(x,w),2),wy=mul(mul(y,w),2),wz=mul(mul(z,w),2);
 return {{{sub(sub(1,yy),zz),sub(xy,wz),add(xz,wy)},
          {add(xy,wz),sub(sub(1,xx),zz),sub(yz,wx)},
          {sub(xz,wy),add(yz,wx),sub(sub(1,xx),yy)}}};
}
// Original public NxActor tensor accessor uses a different quaternion decoder
// and sum order from the solver. 'diagonal' is the public reciprocal of the
// stored principal inverse inertia, in source axes; local frame must be identity.
inline Matrix inertia(const Matrix&r,std::array<float,3> diagonal){
 const auto a=diagonal[0],b=diagonal[1],c=diagonal[2];Matrix v{};
 v[0][0]=add(add(mul(r[0][2],mul(r[0][2],c)),mul(r[0][1],mul(r[0][1],b))),mul(r[0][0],mul(r[0][0],a)));
 v[0][1]=v[1][0]=add(add(mul(r[1][2],mul(r[0][2],c)),mul(r[1][1],mul(r[0][1],b))),mul(r[1][0],mul(r[0][0],a)));
 v[0][2]=v[2][0]=add(add(mul(r[2][2],mul(r[0][2],c)),mul(r[2][0],mul(r[0][0],a))),mul(r[2][1],mul(r[0][1],b)));
 v[1][1]=add(add(mul(r[1][2],mul(r[1][2],c)),mul(r[1][1],mul(r[1][1],b))),mul(r[1][0],mul(r[1][0],a)));
 v[1][2]=v[2][1]=add(add(mul(r[2][2],mul(r[1][2],c)),mul(r[2][0],mul(r[1][0],a))),mul(r[2][1],mul(r[1][1],b)));
 v[2][2]=add(add(mul(mul(r[2][2],c),r[2][2]),mul(mul(r[2][0],a),r[2][0])),mul(mul(r[2][1],b),r[2][1]));
 return v;
}
inline std::array<float,3> momentum(const Matrix&m,std::array<float,3> v){
 return {add(add(mul(v[0],m[0][0]),mul(v[2],m[0][2])),mul(v[1],m[0][1])),
         add(add(mul(v[1],m[1][1]),mul(v[2],m[1][2])),mul(v[0],m[1][0])),
         add(add(mul(v[0],m[2][0]),mul(v[1],m[2][1])),mul(v[2],m[2][2]))};
}
}
