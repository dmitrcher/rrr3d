#pragma once
// Experimental scalar reconstruction of PhysXCore 203979/20aad0/99860/ac00.
// No fixture identifiers, measured states, or expected outputs are used.
#include "DerivedContactErrorFrame.h"
#include "NativeContactNormalV38.h"
namespace motorrock { namespace framev36 {
namespace cf=contactframe; namespace de=derivederror;
using de::add; using de::sub; using de::mul;
inline cf::Quaternion multiply(cf::Quaternion a,cf::Quaternion b) {
 const float x=a[0],y=a[1],z=a[2],w=a[3],X=b[0],Y=b[1],Z=b[2],W=b[3];
 return {{sub(add(add(mul(w,X),mul(W,x)),mul(y,Z)),mul(Y,z)),
          sub(add(add(mul(W,y),mul(z,X)),mul(w,Y)),mul(Z,x)),
          sub(add(add(mul(Y,x),mul(w,Z)),mul(W,z)),mul(y,X)),
          sub(sub(sub(mul(w,W),mul(X,x)),mul(Y,y)),mul(Z,z))}};
}
inline cf::Quaternion normalize(cf::Quaternion q) {
 const float square=cf::store(double(q[0])*q[0]+double(q[1])*q[1]+double(q[2])*q[2]+double(q[3])*q[3]);
 const float root=cf::store(std::sqrt(double(square))),inv=cf::store(1.0/root);
 for(int i=0;i<4;++i)q[i]=mul(q[i],inv);return q;
}
inline cf::Quaternion rawWorld(cf::Vector n) {
 if(n[0]>.9999F)return {{0,0,0,1}};
 if(n[0]<-.9999F)return {{0,1,0,0}};
 const float length=cf::store(std::sqrt(double(add(mul(n[1],n[1]),mul(n[2],n[2])))));
 const float inverse=cf::store(1.0/length);
 float angle=cf::store(std::asin(std::fmax(-1.0,std::fmin(1.0,double(length)))));
 if(n[0]<0)angle=sub(3.1415927410125732F,angle);
 const float half=mul(angle,.5F),sine=cf::store(std::sin(double(half)));
 return {{mul(sine,mul(0,inverse)),mul(sine,mul(-n[2],inverse)),mul(sine,mul(n[1],inverse)),cf::store(std::cos(double(half)))}};
}
inline cf::Vector cross(cf::Vector a,cf::Vector b) {
 return {{sub(mul(a[1],b[2]),mul(a[2],b[1])),sub(mul(a[2],b[0]),mul(a[0],b[2])),sub(mul(a[0],b[1]),mul(a[1],b[0]))}};
}
inline float square(cf::Vector v){return add(add(mul(v[0],v[0]),mul(v[1],v[1])),mul(v[2],v[2]));}
inline cf::Vector unit(cf::Vector v) {
 const float length=cf::store(std::sqrt(double(square(v))));
 if(length!=0){const float inv=cf::store(1.0/length);for(int i=0;i<3;++i)v[i]=mul(v[i],inv);}return v;
}
inline cf::Matrix basis(cf::Vector n,cf::Vector direction) {
 cf::Vector first=framev36::cross(direction,n),second;
 const float length=cf::store(std::sqrt(double(square(first))));
 if(length>.01F){first=framev36::unit(first);second=framev36::cross(n,first);}
 else {
  if(std::abs(double(n[2]))>.7071067811865476) {
   const float sq=add(mul(n[1],n[1]),mul(n[2],n[2]));
   const double inv=1.0/std::sqrt(double(sq));const float invf=cf::store(inv);
   first={{0,-mul(n[2],invf),mul(n[1],invf)}};
   second={{cf::store(inv*sq),-mul(n[0],first[2]),mul(n[0],first[1])}};
  } else {
   const float sq=add(mul(n[1],n[1]),mul(n[0],n[0]));
   const double inv=1.0/std::sqrt(double(sq));const float invf=cf::store(inv);
   first={{cf::store(-double(n[1])*inv),mul(n[0],invf),0}};
   second={{-mul(n[2],first[1]),mul(n[2],first[0]),cf::store(inv*sq)}};
  }
  first=framev36::unit(first);second=framev36::unit(second);
 }
 return cf::columns(n,first,second);
}
inline cf::Quaternion encode(const cf::Matrix& m) {
 const float trace=add(add(m.a[0][0],m.a[1][1]),m.a[2][2]);
 int i=3,j=0,k=0;float square;
 if(trace>=0)square=add(trace,1);
 else {i=m.a[1][1]>m.a[0][0]?1:0;if(m.a[2][2]>m.a[i][i])i=2;j=(1<<i)&3;k=(1<<j)&3;square=sub(sub(add(m.a[i][i],1),m.a[j][j]),m.a[k][k]);}
 const double root=std::sqrt(double(square));const float scale=cf::store(.5/cf::store(root));
 cf::Quaternion q={{0,0,0,0}};q[i]=cf::store(.5*root);
 if(i==3){q[0]=mul(sub(m.a[2][1],m.a[1][2]),scale);q[1]=mul(sub(m.a[0][2],m.a[2][0]),scale);q[2]=mul(sub(m.a[1][0],m.a[0][1]),scale);}
 else {q[j]=mul(add(m.a[i][j],m.a[j][i]),scale);q[k]=mul(add(m.a[k][i],m.a[i][k]),scale);q[3]=mul(sub(m.a[k][j],m.a[j][k]),scale);}if(i==3){for(int axis=0;axis<3;++axis)q[axis]=-q[axis];}else q[3]=-q[3];return q;
}
struct Result {de::Frames errors;cf::Result callback;cf::Vector normal;cf::Quaternion rawLocal,seed,solverQuaternion;cf::Matrix materialBasis;};
inline Result compute(const cf::Matrix& actor,cf::Quaternion actorQuaternion,cf::Vector plane,cf::Vector callbackPlane) {
 // The caller supplies the actor pose quaternion used for this simulation.
 // Shape geometry keeps its separate cached transform.
 auto inverse=actorQuaternion;for(int i=0;i<3;++i)inverse[i]=-inverse[i];
 const auto rawLocal=framev36::multiply(inverse,rawWorld(plane));
 Result r;r.rawLocal=rawLocal;r.errors.raw=framev36::multiply(actorQuaternion,rawLocal);
 const auto materialNormal=de::rotate(r.errors.raw,{{1,0,0}},false);
 const auto matrix=basis(materialNormal,cf::column(actor,2));
 r.materialBasis=matrix;r.errors.material=framev36::encode(matrix);for(int i=0;i<3;++i)r.errors.material[i]=-r.errors.material[i];
 const auto seed=framev36::multiply(inverse,r.errors.material);r.seed=seed;r.solverQuaternion={{0,0,0,1}};
 r.callback=cf::modify(seed,actor,cf::Matrix{{{1,0,0},{0,1,0},{0,0,1}}},callbackPlane,true);
 if(r.callback.modified) {
  const auto solverQuaternion=framev36::multiply(actorQuaternion,r.callback.local0);r.solverQuaternion=solverQuaternion;
  const char* normalFlag=std::getenv("RRR3D_EXPERIMENT_NATIVE_SOLVER_NORMAL");
  r.normal=normalFlag && std::strcmp(normalFlag,"1")==0?contactnormalv38::decode(solverQuaternion):materialcandidate::solverNormal(solverQuaternion);
 } else r.normal=plane;
 return r;
}
inline Result compute(const cf::Matrix& actor,cf::Quaternion actorQuaternion,cf::Vector plane) {
 return compute(actor,actorQuaternion,plane,plane);
}
}}
