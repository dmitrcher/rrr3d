#pragma once
#include <array>
#include <cmath>
namespace motorrock::nativeangular {
inline float add(float a,float b){volatile float v=a+b;return v;}
inline float sub(float a,float b){volatile float v=a-b;return v;}
inline float mul(float a,float b){volatile float v=a*b;return v;}
struct Step {std::array<float,4> rotation;float length=0,halfAngle=0,cosine=1,sineFactor=0;};
// Source coordinates, scalar-last quaternion. Reconstructed from original
// PhysX 2.8.4 CPU integration (RVA 0x7f1d0), not a fitted fixture formula.
// The caller supplies already-clamped, post-solver angular velocity.
inline Step integrate(std::array<float,4> q,std::array<float,3> v,float dt){
 Step r;r.rotation=q;
 const float square=add(add(mul(v[0],v[0]),mul(v[1],v[1])),mul(v[2],v[2]));
 if(square==0)return r;
 r.length=static_cast<float>(std::sqrt(static_cast<double>(square)));
 if(r.length==0)return r;
 r.halfAngle=static_cast<float>(static_cast<double>(r.length)*static_cast<double>(dt)*0.5);
 r.cosine=static_cast<float>(std::cos(static_cast<double>(r.halfAngle)));
 // The x87 FSIN result is divided before the float store: do not round sine
 // to float before division. Host double reproduces all captured intermediates;
 // arbitrary transcendental boundary cases are not yet certified.
 r.sineFactor=static_cast<float>(std::sin(static_cast<double>(r.halfAngle))/static_cast<double>(r.length));
 std::array<float,3>d={mul(v[0],r.sineFactor),mul(v[1],r.sineFactor),mul(v[2],r.sineFactor)};
 std::array<float,3>cross={sub(mul(d[1],q[2]),mul(q[1],d[2])),sub(mul(q[0],d[2]),mul(q[2],d[0])),sub(mul(q[1],d[0]),mul(q[0],d[1]))};
 for(unsigned i=0;i<3;++i)r.rotation[i]=add(mul(r.cosine,q[i]),add(add(cross[i],mul(q[i],0.0f)),mul(q[3],d[i])));
 const float dot=add(add(mul(q[0],d[0]),mul(q[2],d[2])),mul(q[1],d[1]));
 r.rotation[3]=add(mul(r.cosine,q[3]),sub(mul(q[3],0.0f),dot));
 return r;
}
}
