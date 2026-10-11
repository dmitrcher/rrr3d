#include "ContactFrameScalar.h"
#include <iostream>
#include <iomanip>
#include <xmmintrin.h>
namespace cf=motorrock::contactframe;
static float rsqrt(float x,int refine) {
 __m128 a=_mm_set_ss(x),r=_mm_rsqrt_ss(a);
 if(refine)r=_mm_mul_ss(_mm_mul_ss(_mm_set_ss(.5f),r),_mm_sub_ss(_mm_set_ss(3.f),_mm_mul_ss(a,_mm_mul_ss(r,r))));
 return _mm_cvtss_f32(r);
}
static cf::Vector cross(cf::Vector a,cf::Vector b,int mode) {
 if(mode==0)return cf::cross(a,b);
 cf::Vector r={{a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]}};return r;
}
static float dot(cf::Vector a,cf::Vector b,int mode) {
 if(mode==0)return cf::store(double(a[0])*b[0]+double(a[1])*b[1]+double(a[2])*b[2]);
 return (a[0]*b[0]+a[1]*b[1])+a[2]*b[2];
}
static cf::Vector unit(cf::Vector a,int mode) {
 if(mode==0)return cf::unit(a);
 const float square=dot(a,a,mode);
 if(square==0)return a;
 const float inv=mode==1?1.f/_mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(square))):rsqrt(square,mode==3);
 for(int i=0;i<3;++i)a[i]*=inv;
 return a;
}
static cf::Quaternion encode(const cf::Matrix& m,int mode) {
 if(mode==0)return cf::encode(m);
 cf::Quaternion q={{0,0,0,0}};
 const float trace=(m.a[0][0]+m.a[1][1])+m.a[2][2];
 int i=3,j=0,k=0;float x;
 if(trace>=0)x=trace+1.f;
 else {i=m.a[1][1]>m.a[0][0]?1:0;if(m.a[2][2]>m.a[i][i])i=2;j=(1<<i)&3;k=(1<<j)&3;x=((1.f+m.a[i][i])-m.a[j][j])-m.a[k][k];}
 const float inv=rsqrt(x,0);
 const float root=mode==1?_mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x))):mode==4?x*inv:1.f/inv;
 q[i]=.5f*root;const float s=mode==3?.5f*inv:.5f/root;
 if(i==3){q[0]=(m.a[2][1]-m.a[1][2])*s;q[1]=(m.a[0][2]-m.a[2][0])*s;q[2]=(m.a[1][0]-m.a[0][1])*s;}
 else {q[j]=(m.a[i][j]+m.a[j][i])*s;q[k]=(m.a[k][i]+m.a[i][k])*s;q[3]=(m.a[k][j]-m.a[j][k])*s;}
 return q;
}
int main() {
 std::cout<<std::setprecision(9);cf::Vector v[3],axes[3];int car;
 while(std::cin>>v[0][0]) {
  for(int j=1;j<3;++j)std::cin>>v[0][j];
  for(int i=1;i<3;++i)for(int j=0;j<3;++j)std::cin>>v[i][j];
  for(int i=0;i<3;++i)for(int j=0;j<3;++j)std::cin>>axes[i][j];
  std::cin>>car;if(!std::cin)return 2;
  cf::Vector e1,e2;for(int j=0;j<3;++j){e1[j]=v[1][j]-v[0][j];e2[j]=v[2][j]-v[0][j];}
  for(int norm=0;norm<4;++norm)for(int projection=0;projection<4;++projection) {
   cf::Vector n=unit(cross(e1,e2,norm),norm),second,first;
   if(car==1)for(int j=0;j<3;++j)n[j]=-n[j];
   cf::Vector u=axes[2];
   if(projection<2) {
    float d=dot(u,n,projection==0?1:0);
    for(int j=0;j<3;++j)second[j]=projection==0?u[j]-d*n[j]:cf::store(double(u[j])-double(d)*n[j]);
    if(dot(second,second,norm)<=1.e-4f) {u[0]=1;u[1]=u[2]=0;d=dot(u,n,projection==0?1:0);for(int j=0;j<3;++j)second[j]=projection==0?u[j]-d*n[j]:cf::store(double(u[j])-double(d)*n[j]);}
    second=unit(second,norm);first=unit(cross(second,n,norm),norm);
   } else {
    first=cross(u,n,projection==2?1:0);
    if(dot(first,first,norm)<=1.e-4f) {u[0]=1;u[1]=u[2]=0;first=cross(u,n,projection==2?1:0);}
    first=unit(first,norm);second=unit(cross(n,first,projection==2?1:0),norm);
   }
   const cf::Matrix frame=cf::columns(n,first,second);
   for(int mode=0;mode<5;++mode) {
    const cf::Quaternion q=encode(frame,mode);
    std::cout<<norm<<' '<<projection<<' '<<mode;
    for(int j=0;j<4;++j)std::cout<<' '<<q[j];
    for(int j=0;j<3;++j)for(int k=0;k<3;++k)std::cout<<' '<<frame.a[k][j];
    std::cout<<'\n';
   }
  }
 }
}
