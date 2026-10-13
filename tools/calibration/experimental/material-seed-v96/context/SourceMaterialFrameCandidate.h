#pragma once
// v27 experimental material-frame arithmetic. This is a general candidate,
// not an exact reconstruction of the private PhysX seed generator. The
// NEON estimate with two hardware refinement steps matches observed SSE
// rsqrt estimates in this Windows ARM VM. Material seed construction itself
// remains unverified on tilted bodies. No cases or reference values are loaded.
#include "ContactFrameScalar.h"
#if defined(__aarch64__)
#include <arm_neon.h>
#elif defined(_MSC_VER) && defined(_M_IX86)
#include <xmmintrin.h>
#endif
namespace motorrock { namespace materialcandidate {
namespace cf=motorrock::contactframe;
inline float inverseSquareRootEstimate(float x) {
#if defined(__aarch64__)
 const float32x2_t a=vdup_n_f32(x);float32x2_t r=vrsqrte_f32(a);
 r=vmul_f32(r,vrsqrts_f32(a,vmul_f32(r,r)));
 r=vmul_f32(r,vrsqrts_f32(a,vmul_f32(r,r)));
 return vget_lane_f32(r,0);
#elif defined(_MSC_VER) && defined(_M_IX86)
 return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x)));
#else
 // Diagnostic fallback only; parity is not certified for other architectures.
 return cf::store(1.0/cf::store(std::sqrt(double(x))));
#endif
}
inline cf::Quaternion encode(const cf::Matrix& m) {
 cf::Quaternion q={{0,0,0,0}};
 const float trace=cf::store(double(cf::store(double(m.a[0][0])+m.a[1][1]))+m.a[2][2]);
 int i=3,j=0,k=0;float x;
 if(trace>=0)x=cf::store(double(trace)+1);
 else {
  i=m.a[1][1]>m.a[0][0]?1:0;if(m.a[2][2]>m.a[i][i])i=2;j=(1<<i)&3;k=(1<<j)&3;
  x=cf::store(double(cf::store(double(cf::store(1.0+m.a[i][i]))-m.a[j][j]))-m.a[k][k]);
 }
 const float inverseRoot=inverseSquareRootEstimate(x);
 const float root=cf::store(1.0/inverseRoot);
 q[i]=cf::store(.5*root);const float s=cf::store(.5/root);
 if(i==3) {
  q[0]=cf::store(double(cf::store(double(m.a[2][1])-m.a[1][2]))*s);
  q[1]=cf::store(double(cf::store(double(m.a[0][2])-m.a[2][0]))*s);
  q[2]=cf::store(double(cf::store(double(m.a[1][0])-m.a[0][1]))*s);
 } else {
  q[j]=cf::store(double(cf::store(double(m.a[i][j])+m.a[j][i]))*s);
  q[k]=cf::store(double(cf::store(double(m.a[k][i])+m.a[i][k]))*s);
  q[3]=cf::store(double(cf::store(double(m.a[k][j])-m.a[j][k]))*s);
 }return q;
}
inline cf::Quaternion multiply(cf::Quaternion a,cf::Quaternion b) {
 const float x=a[0],y=a[1],z=a[2],w=a[3],X=b[0],Y=b[1],Z=b[2],W=b[3];
 const double terms[4][4]={{double(w)*X,double(x)*W,double(y)*Z,-double(z)*Y},
  {double(w)*Y,-double(x)*Z,double(y)*W,double(z)*X},
  {double(w)*Z,double(x)*Y,-double(y)*X,double(z)*W},
  {double(w)*W,-double(x)*X,-double(y)*Y,-double(z)*Z}};
 cf::Quaternion q;
 for(int i=0;i<4;++i)q[i]=cf::store(double(cf::store(double(cf::store(double(cf::store(terms[i][0]))+cf::store(terms[i][1])))+cf::store(terms[i][2])))+cf::store(terms[i][3]));
 return q;
}
inline cf::Vector solverNormal(cf::Quaternion q) {
 const float yy=cf::store(double(q[1])*q[1]),zz=cf::store(double(q[2])*q[2]);
 const float xy=cf::store(double(q[0])*q[1]),wz=cf::store(double(q[3])*q[2]);
 const float xz=cf::store(double(q[0])*q[2]),wy=cf::store(double(q[3])*q[1]);
 cf::Vector n={{cf::store(double(cf::store(1.0-2.0*yy))-2.0*zz),
  cf::store(2.0*xy+2.0*wz),cf::store(2.0*xz-2.0*wy)}};return n;
}
struct Result {cf::Quaternion seed;cf::Result callback;cf::Vector normal;};
inline Result compute(const cf::Matrix& actor,cf::Quaternion actorQuaternion,cf::Vector triangleNormal) {
 const cf::Vector n=cf::unit(triangleNormal);cf::Vector u=cf::column(actor,2),first=cf::cross(u,n);
 if(cf::length(first)<=.01F) {u[0]=1;u[1]=u[2]=0;first=cf::cross(u,n);if(cf::length(first)<=.0001F){u[0]=u[1]=0;u[2]=1;first=cf::cross(u,n);}}
 first=cf::unit(first);const cf::Vector second=cf::unit(cf::cross(n,first));
 const cf::Quaternion worldSeed=motorrock::materialcandidate::encode(cf::columns(n,first,second));
 for(int i=0;i<3;++i)actorQuaternion[i]=-actorQuaternion[i];
 Result r;r.seed=multiply(actorQuaternion,worldSeed);
 cf::Matrix identity={{{1,0,0},{0,1,0},{0,0,1}}};
 r.callback=cf::modify(r.seed,actor,identity,n,true);
 r.normal=r.callback.modified?solverNormal(r.callback.local1):n;
 return r;
}
}}
