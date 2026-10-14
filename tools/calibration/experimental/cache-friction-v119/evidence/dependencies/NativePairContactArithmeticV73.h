// Original MotorRock/PhysX paired contact arithmetic reconstructed from x86 captures.
// SPDX-License-Identifier: MIT
#pragma once
#include <array>
#include <cmath>
#include <cfloat>
namespace motorrock_native_pair_v73 {
using Vector=std::array<float,3>;
using Matrix=std::array<float,9>; // Column-major, original Z-up coordinates.
inline float add(float a,float b){volatile float r=a+b;return r;}
inline float sub(float a,float b){volatile float r=a-b;return r;}
inline float mul(float a,float b){volatile float r=a*b;return r;}
inline float div(float a,float b){volatile float r=a/b;return r;}
inline Vector cross(const Vector&a,const Vector&b){return {{sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))}};}
inline float dot(const Vector&a,const Vector&b){return add(add(mul(a[0],b[0]),mul(a[2],b[2])),mul(a[1],b[1]));}
inline Vector tensor(const Matrix&i,const Vector&v){Vector r;for(unsigned k=0;k<3;++k)r[k]=add(add(mul(i[k],v[0]),mul(i[3+k],v[1])),mul(i[6+k],v[2]));return r;}
struct Body {Vector velocity{},angular{},lever{};Matrix inverse_inertia{};float inverse_mass=0;};
struct Row {Vector normal{},tangent0{},tangent1{},friction{};std::array<float,3> mass{},bias{};std::array<float,2> static_mu{},dynamic_mu{};float normal_impulse=0;};
inline Vector solve(Row &row,std::array<Body,2>&bodies) {
 std::array<Vector,2> points;
 for(unsigned b=0;b<2;++b){const auto spin=cross(bodies[b].angular,bodies[b].lever);for(unsigned k=0;k<3;++k)points[b][k]=add(bodies[b].velocity[k],spin[k]);}
 Vector point;for(unsigned k=0;k<3;++k)point[k]=sub(points[0][k],points[1][k]);
 const float old=row.normal_impulse;
 float delta=sub(mul(row.mass[0],-dot(row.normal,point)),row.bias[0]);
 float total=add(old,delta);
 if(total<0){total=0;delta=-old;}
 else if(total>FLT_MAX){total=FLT_MAX;delta=sub(total,old);}
 Vector df{};
 if(total>0){
  std::array<float,2> change;
  const Vector axes[]={row.tangent0,row.tangent1};
  for(unsigned j=0;j<2;++j){
   const float previous=dot(axes[j],row.friction);
   float desired=sub(mul(row.mass[1+j],-dot(axes[j],point)),row.bias[1+j]);
   const float target=add(previous,desired);
   // Original axis0 static comparison retains x87 product before f32 store.
   const double static_limit=j==0?double(row.static_mu[j])*double(total):double(mul(row.static_mu[j],total));
   if(std::abs(target)>static_limit)desired=sub(mul(div(mul(row.dynamic_mu[j],total),std::abs(target)),target),previous);
   change[j]=desired;
  }
  for(unsigned k=0;k<3;++k){df[k]=add(mul(row.tangent0[k],change[0]),mul(row.tangent1[k],change[1]));row.friction[k]=add(row.friction[k],df[k]);}
 }
 Vector force;for(unsigned k=0;k<3;++k)force[k]=add(df[k],mul(row.normal[k],delta));
 for(unsigned b=0;b<2;++b){
  Vector applied;for(unsigned k=0;k<3;++k)applied[k]=mul(b==0?1.0f:-1.0f,force[k]);
  const auto spin=tensor(bodies[b].inverse_inertia,cross(bodies[b].lever,applied));
  for(unsigned k=0;k<3;++k){bodies[b].velocity[k]=add(bodies[b].velocity[k],mul(applied[k],bodies[b].inverse_mass));bodies[b].angular[k]=add(bodies[b].angular[k],spin[k]);}
 }
 row.normal_impulse=total;
 return force;
}
inline std::array<Vector,3> axes(const std::array<float,4>&q){
 const float x=q[0],y=q[1],z=q[2],w=q[3];
 const float xx=mul(mul(2,x),x),yy=mul(mul(2,y),y),zz=mul(mul(2,z),z),xy=mul(mul(2,x),y),xz=mul(mul(2,x),z),yz=mul(mul(2,y),z),xw=mul(mul(2,x),w),yw=mul(mul(2,y),w),zw=mul(mul(2,z),w);
 return {{Vector{{sub(1,add(yy,zz)),add(xy,zw),sub(xz,yw)}},Vector{{sub(xy,zw),sub(1,add(xx,zz)),add(yz,xw)}},Vector{{add(xz,yw),sub(yz,xw),sub(1,add(xx,yy))}}}};
}
}
