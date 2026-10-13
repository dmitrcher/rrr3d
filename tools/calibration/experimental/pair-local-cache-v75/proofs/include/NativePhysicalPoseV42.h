#pragma once
// Prepared original physical pose uses scalar SSE matrix construction.
// The public NxMat33 helper is a distinct, extended-precision operation path.
#include "ContactFrameScalar.h"
namespace motorrock { namespace physicalposev42 {
namespace cf=contactframe;
inline float mul(float a,float b){return cf::store(double(a)*b);}
inline float add(float a,float b){return cf::store(double(a)+b);}
inline float sub(float a,float b){return cf::store(double(a)-b);}
inline cf::Matrix rotation(cf::Quaternion q) {
 const float x=q[0],y=q[1],z=q[2],w=q[3];
 const float dx=mul(x,2),dy=mul(y,2),dz=mul(z,2);
 const float xx=mul(x,dx),yy=mul(y,dy),zz=mul(z,dz);
 const float xy=mul(y,dx),xz=mul(z,dx),yz=mul(z,dy);
 const float wx=mul(w,dx),wy=mul(w,dy),wz=mul(w,dz);
 cf::Matrix r;
 r.a[0][0]=sub(1,add(zz,yy));r.a[0][1]=sub(xy,wz);r.a[0][2]=add(xz,wy);
 r.a[1][0]=add(xy,wz);r.a[1][1]=sub(1,add(zz,xx));r.a[1][2]=sub(yz,wx);
 r.a[2][0]=sub(xz,wy);r.a[2][1]=add(yz,wx);r.a[2][2]=sub(1,add(yy,xx));
 return r;
}
}}
