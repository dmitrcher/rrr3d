#pragma once
// PhysXCore solver-body inertia at RVA 0x7c5a0; scalar SSE operation order.
// Same reconstruction already verified for contact rows in NativeInertiaRotation.h.
#include "NativeWheelRowV56.h"
namespace motorrock_native_wheel_inertia_v86 {
using namespace motorrock_native_wheel_row_v56;
using Quaternion=std::array<float,4>;
inline bool diagonal(const Matrix& local) {
 for(unsigned i=0;i<3;++i)for(unsigned j=0;j<3;++j)if(i!=j && local[i*3+j]!=0)return false;
 return true;
}
inline Matrix solver(Quaternion q,const Matrix& local) {
 const float x=q[0],y=q[1],z=q[2],w=q[3];
 const float dx=mul(x,2),dy=mul(y,2),dz=mul(z,2);
 const float xx=mul(x,dx),yy=mul(y,dy),zz=mul(z,dz);
 const float xy=mul(y,dx),xz=mul(z,dx),yz=mul(z,dy);
 const float wx=mul(w,dx),wy=mul(w,dy),wz=mul(w,dz);
 const float r00=sub(1,add(zz,yy)),r11=sub(1,add(zz,xx)),r22=sub(1,add(yy,xx));
 const float r01=sub(xy,wz),r10=add(xy,wz),r02=add(xz,wy),r20=sub(xz,wy),r12=sub(yz,wx),r21=add(yz,wx);
 const float a=local[0],b=local[4],c=local[8];
 const auto weighted=[](float d,float l,float r){return mul(mul(d,l),r);};
 Matrix out;
 out[0]=add(add(weighted(b,r01,r01),weighted(c,r02,r02)),weighted(a,r00,r00));
 out[1]=out[3]=add(add(weighted(a,r00,r10),weighted(c,r12,r02)),weighted(b,r11,r01));
 out[2]=out[6]=add(add(weighted(a,r00,r20),weighted(c,r22,r02)),weighted(b,r21,r01));
 out[4]=add(add(mul(mul(r12,r12),c),weighted(a,r10,r10)),weighted(b,r11,r11));
 out[5]=out[7]=add(add(mul(mul(r22,r12),c),weighted(a,r10,r20)),weighted(b,r21,r11));
 out[8]=add(add(mul(mul(r22,r22),c),weighted(a,r20,r20)),weighted(b,r21,r21));
 return out;
}
}
