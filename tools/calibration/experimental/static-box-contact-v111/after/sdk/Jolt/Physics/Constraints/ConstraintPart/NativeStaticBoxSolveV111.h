#pragma once
#include "NativePairContactArithmeticV73.h"
// Original static box contact: restitution stays inside the velocity term
// before effective mass multiplication. Existing friction/force arithmetic
// is retained; no measured states, coefficients or fixture rules are used.
namespace motorrock_native_static_box_v111 {
using namespace motorrock_native_pair_v73;
inline Vector solve(Row &row,std::array<Body,2>&bodies,float restitutionVelocity) {
 std::array<Vector,2> points;
 for(unsigned b=0;b<2;++b){const auto spin=cross(bodies[b].angular,bodies[b].lever);for(unsigned k=0;k<3;++k)points[b][k]=add(bodies[b].velocity[k],spin[k]);}
 Vector point;for(unsigned k=0;k<3;++k)point[k]=sub(points[0][k],points[1][k]);
 const float old=row.normal_impulse;
 float delta=sub(mul(sub(restitutionVelocity,dot(row.normal,point)),row.mass[0]),row.bias[0]);
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
}
