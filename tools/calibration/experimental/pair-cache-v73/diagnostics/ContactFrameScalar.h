#pragma once
// Scalar reproduction of the public NxMat33/NxVec3 contact-callback math.
// Float stores and extended intermediate products are explicit: do not replace
// them with Jolt vector/quaternion operations. This does not generate the SDK
// material seed or implement the contact solver.
#include <cmath>
namespace motorrock { namespace contactframe {
inline float store(double x) { volatile float value = static_cast<float>(x); return value; }
struct Vector { float a[3]; float& operator[](int i) { return a[i]; } float operator[](int i) const { return a[i]; } };
struct Quaternion { float a[4]; float& operator[](int i) { return a[i]; } float operator[](int i) const { return a[i]; } };
struct Matrix { float a[3][3]; };
inline Vector cross(Vector x, Vector y) {
 Vector r = {{store(double(x[1])*y[2]-double(x[2])*y[1]),
  store(double(x[2])*y[0]-double(x[0])*y[2]), store(double(x[0])*y[1]-double(x[1])*y[0])}}; return r;
}
inline float length(Vector v) {
 return store(std::sqrt(store(double(v[0])*v[0]+double(v[1])*v[1]+double(v[2])*v[2])));
}
inline Vector unit(Vector v) {
 const float m=length(v); if(m!=0) { const float inv=store(1.0/m); for(int i=0;i<3;++i)v[i]=store(double(v[i])*inv); } return v;
}
inline Vector column(const Matrix& m, int i) { Vector r={{m.a[0][i],m.a[1][i],m.a[2][i]}};return r; }
inline Matrix columns(Vector x,Vector y,Vector z) {
 Matrix r;for(int i=0;i<3;++i){r.a[i][0]=x[i];r.a[i][1]=y[i];r.a[i][2]=z[i];}return r;
}
inline Matrix product(const Matrix& a,const Matrix& b) {
 Matrix r;for(int i=0;i<3;++i)for(int j=0;j<3;++j)r.a[i][j]=store(double(a.a[i][0])*b.a[0][j]+double(a.a[i][1])*b.a[1][j]+double(a.a[i][2])*b.a[2][j]);return r;
}
inline Matrix inverse(const Matrix& m) {
 Matrix r;
 for(int i=0;i<3;++i)for(int j=0;j<3;++j){
  const int row1=(j+1)%3,row2=(j+2)%3,col1=(i+1)%3,col2=(i+2)%3;
  r.a[i][j]=store(double(m.a[row1][col1])*m.a[row2][col2]-double(m.a[row1][col2])*m.a[row2][col1]);
 }
 const float determinant=store(double(r.a[0][0])*m.a[0][0]+double(r.a[0][1])*m.a[1][0]+double(r.a[0][2])*m.a[2][0]);
 const float scale=store(1.0/determinant);
 for(int i=0;i<3;++i)for(int j=0;j<3;++j)r.a[i][j]=store(double(r.a[i][j])*scale);return r;
}
inline Matrix decode(Quaternion q) {
 const double x=q[0],y=q[1],z=q[2],w=q[3];Matrix r;
 r.a[0][0]=store(1-y*y*2-z*z*2);r.a[0][1]=store(x*y*2-w*z*2);r.a[0][2]=store(x*z*2+w*y*2);
 r.a[1][0]=store(x*y*2+w*z*2);r.a[1][1]=store(1-x*x*2-z*z*2);r.a[1][2]=store(y*z*2-w*x*2);
 r.a[2][0]=store(x*z*2-w*y*2);r.a[2][1]=store(y*z*2+w*x*2);r.a[2][2]=store(1-x*x*2-y*y*2);return r;
}
inline Quaternion encode(const Matrix& m) {
 Quaternion q={{0,0,0,0}};const float trace=store(double(m.a[0][0])+m.a[1][1]+m.a[2][2]);
 if(trace>=0) {
  float s=store(std::sqrt(store(double(trace)+1)));q[3]=store(.5*s);s=store(.5/s);
  q[0]=store((double(m.a[2][1])-m.a[1][2])*s);q[1]=store((double(m.a[0][2])-m.a[2][0])*s);q[2]=store((double(m.a[1][0])-m.a[0][1])*s);
 } else {
  int i=m.a[1][1]>m.a[0][0]?1:0;if(m.a[2][2]>m.a[i][i])i=2;const int j=(1<<i)&3,k=(1<<j)&3;
  float s=store(std::sqrt(store(1.0+m.a[i][i]-m.a[j][j]-m.a[k][k])));q[i]=store(.5*s);s=store(.5/s);
  q[j]=store((double(m.a[i][j])+m.a[j][i])*s);q[k]=store((double(m.a[k][i])+m.a[i][k])*s);q[3]=store((double(m.a[k][j])-m.a[j][k])*s);
 }return q;
}
struct Result { Matrix before,after;Quaternion local0,local1;Vector decodedLocalNormal;bool modified; };
inline Result modify(Quaternion seed,const Matrix& actor0,const Matrix& actor1,Vector triangleNormal,bool carFirst) {
 Result r;r.before=decode(seed);if(carFirst)r.before=product(actor0,r.before);r.modified=false;
 if(!carFirst)for(int i=0;i<3;++i)triangleNormal[i]=-triangleNormal[i];
 Vector second=column(r.before,2),first=cross(second,triangleNormal);
 if(length(first)<.5F){first=column(r.before,1);second=cross(triangleNormal,first);}
 if(length(second)<=.1F)return r;
 first=unit(first);second=unit(second);Vector normal=unit(cross(first,second));r.after=columns(normal,first,second);
 r.local0=encode(carFirst?product(inverse(actor0),r.after):r.after);
 r.local1=encode(carFirst?r.after:product(inverse(actor1),r.after));
 r.decodedLocalNormal=column(decode(r.local0),0);r.modified=true;return r;
}
}}
