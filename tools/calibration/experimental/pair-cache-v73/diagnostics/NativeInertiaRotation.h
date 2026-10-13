#pragma once
// Experimental native global inverse-inertia transform. Keep intermediate
// products extended and store each output once; the tensor is symmetric.
// Inputs are the current independently calculated actor pose and local tensor.
#include "ContactFrameScalar.h"
namespace motorrock { namespace inertiav37 {
namespace cf=contactframe;
inline cf::Matrix rotate(const cf::Matrix& rotation,const cf::Matrix& local) {
 cf::Matrix result;
 for(int i=0;i<3;++i)for(int j=i;j<3;++j) {
  double sum=0;
  for(int k=0;k<3;++k)for(int l=0;l<3;++l)
   sum+=double(rotation.a[i][k])*local.a[k][l]*rotation.a[j][l];
  result.a[i][j]=cf::store(sum);result.a[j][i]=result.a[i][j];
 }
 return result;
}

// Solver-body preparation at PhysXCore RVA 0x7c5a0 uses scalar SSE float
// operations, distinct from the public extended-precision tensor accessor.
// The native body stores principal inverse inertia; a non-diagonal tensor
// needs its independently matched mass-frame orientation before using this.
inline bool diagonal(const cf::Matrix& local) {
 for(int i=0;i<3;++i)for(int j=0;j<3;++j)if(i!=j && local.a[i][j]!=0)return false;
 return true;
}
inline float smul(float a,float b){return cf::store(double(a)*b);}
inline float sadd(float a,float b){return cf::store(double(a)+b);}
inline float ssub(float a,float b){return cf::store(double(a)-b);}
inline cf::Matrix solver(cf::Quaternion q,const cf::Matrix& local) {
 const float x=q[0],y=q[1],z=q[2],w=q[3];
 const float dx=smul(x,2),dy=smul(y,2),dz=smul(z,2);
 const float xx=smul(x,dx),yy=smul(y,dy),zz=smul(z,dz);
 const float xy=smul(y,dx),xz=smul(z,dx),yz=smul(z,dy);
 const float wx=smul(w,dx),wy=smul(w,dy),wz=smul(w,dz);
 const float r00=ssub(1,sadd(zz,yy)),r11=ssub(1,sadd(zz,xx)),r22=ssub(1,sadd(yy,xx));
 const float r01=ssub(xy,wz),r10=sadd(xy,wz),r02=sadd(xz,wy),r20=ssub(xz,wy),r12=ssub(yz,wx),r21=sadd(yz,wx);
 const float a=local.a[0][0],b=local.a[1][1],c=local.a[2][2];
 const auto weighted=[](float d,float l,float r){return smul(smul(d,l),r);};
 cf::Matrix out;
 out.a[0][0]=sadd(sadd(weighted(b,r01,r01),weighted(c,r02,r02)),weighted(a,r00,r00));
 out.a[0][1]=out.a[1][0]=sadd(sadd(weighted(a,r00,r10),weighted(c,r12,r02)),weighted(b,r11,r01));
 out.a[0][2]=out.a[2][0]=sadd(sadd(weighted(a,r00,r20),weighted(c,r22,r02)),weighted(b,r21,r01));
 out.a[1][1]=sadd(sadd(smul(smul(r12,r12),c),weighted(a,r10,r10)),weighted(b,r11,r11));
 out.a[1][2]=out.a[2][1]=sadd(sadd(smul(smul(r22,r12),c),weighted(a,r10,r20)),weighted(b,r21,r11));
 out.a[2][2]=sadd(sadd(smul(smul(r22,r22),c),weighted(a,r20,r20)),weighted(b,r21,r21));
 return out;
}
}}
