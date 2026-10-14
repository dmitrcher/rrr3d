#pragma once
#include "NativePublicAngular.h"
#include "ContactFrameScalar.h"
#include <algorithm>
#include <cfloat>
namespace motorrock::gameangularv113 {
namespace P=nativepublicangular;namespace C=contactframe;
using Vector=std::array<float,3>;using Quaternion=std::array<float,4>;using Matrix=P::Matrix;
inline Matrix inverseTensor(const Quaternion& q,const Vector& d){
 const auto r=P::orientation(q);Matrix out{};
 for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){
  const auto term=[&](unsigned k){return P::mul(r[j][k],P::mul(d[k],r[i][k]));};
  out[i][j]=out[j][i]=P::add(P::add(term(2),term(0)),term(1));
 }
 out[0][0]=P::add(P::add(P::mul(r[0][2],P::mul(d[2],r[0][2])),P::mul(r[0][1],P::mul(d[1],r[0][1]))),P::mul(r[0][0],P::mul(d[0],r[0][0])));
 return out;
}
inline Vector velocity(const Matrix& m,Vector v){
 auto mul=P::mul;auto add=P::add;auto sub=P::sub;
 float det=add(add(mul(mul(m[1][1],m[2][2]),m[0][0]),mul(mul(m[2][0],m[0][1]),m[1][2])),mul(mul(m[0][2],m[1][0]),m[2][1]));
 det=sub(sub(sub(det,mul(mul(m[0][2],m[1][1]),m[2][0])),mul(mul(m[0][1],m[1][0]),m[2][2])),mul(mul(m[1][2],m[2][1]),m[0][0]));
 volatile float quotient=FLT_MAX/std::max(det,1.0f);const float bound=quotient;
 for(auto& x:v)x=std::max(-bound,std::min(bound,x));
 Vector out{};for(unsigned i=0;i<3;++i)out[i]=add(add(mul(m[i][2],v[2]),mul(m[i][1],v[1])),mul(m[i][0],v[0]));return out;
}
inline Vector stabilize(const Quaternion& q,const Vector& inverse,const Vector& omega,const Vector& damping,double roll,double pitch,bool contact){
 Vector inertia{};for(unsigned i=0;i<3;++i)inertia[i]=C::store(1.0/inverse[i]);
 const auto actor=P::orientation(q);const auto world=P::momentum(P::inertia(actor,inertia),omega);
 C::Matrix r;for(unsigned i=0;i<3;++i)for(unsigned j=0;j<3;++j)r.a[i][j]=actor[i][j];
 const auto inv=C::inverse(r);Vector local{};
 for(unsigned i=0;i<3;++i)local[i]=C::store(double(inv.a[i][0])*world[0]+double(inv.a[i][1])*world[1]+double(inv.a[i][2])*world[2]);
 if(!contact){
  if(roll>0){const float limit=C::store(double(inertia[0])*roll*2);local[0]=std::max(-limit,std::min(limit,local[0]));}
  if(pitch>0){const float limit=C::store(double(inertia[1])*pitch*2);local[1]=std::max(-limit,std::min(limit,local[1]));}
 }
 for(unsigned i=0;i<3;++i)local[i]=P::mul(local[i],damping[i]);
 Vector out{};for(unsigned i=0;i<3;++i)out[i]=C::store(double(r.a[i][0])*local[0]+double(r.a[i][1])*local[1]+double(r.a[i][2])*local[2]);return out;
}
// Original Shoemake XYZ static-axis conversion, including normalization of
// the matrix derived from a slightly non-unit input quaternion.
inline Quaternion clampOrientation(Quaternion q,double roll,double pitch){
 const double x=q[0],y=q[1],z=q[2],w=q[3];const double n=x*x+y*y+z*z+w*w,s=n>0?2/n:0;
 const double xs=x*s,ys=y*s,zs=z*s;
 const float r00=C::store(1-(y*ys+z*zs)),r10=C::store(x*ys+w*zs),r20=C::store(x*zs-w*ys),r21=C::store(y*zs+w*xs),r22=C::store(1-(x*xs+y*ys));
 const float cy=C::store(std::sqrt(double(C::store(double(r00)*r00+double(r10)*r10))));
 float ax,ay,az;
 if(cy>16*FLT_EPSILON){ax=C::store(std::atan2(double(r21),double(r22)));ay=C::store(std::atan2(-double(r20),double(cy)));az=C::store(std::atan2(double(r10),double(r00)));}
 else {const float r12=C::store(y*zs-w*xs),r11=C::store(1-(x*xs+z*zs));ax=C::store(std::atan2(-double(r12),double(r11)));ay=C::store(std::atan2(-double(r20),double(cy)));az=0;}
 if(roll>0){const float limit=float(roll);ax=std::max(-limit,std::min(limit,ax));}
 if(pitch>0){const float limit=float(pitch);ay=std::max(-limit,std::min(limit,ay));}
 const double ci=std::cos(double(ax)*.5),cj=std::cos(double(ay)*.5),ch=std::cos(double(az)*.5),si=std::sin(double(ax)*.5),sj=std::sin(double(ay)*.5),sh=std::sin(double(az)*.5);
 const double cc=ci*ch,cs=ci*sh,sc=si*ch,ss=si*sh;
 return {{C::store(cj*sc-sj*cs),C::store(cj*ss+sj*cc),C::store(cj*cs-sj*sc),C::store(cj*cc+sj*ss)}};
}
}
