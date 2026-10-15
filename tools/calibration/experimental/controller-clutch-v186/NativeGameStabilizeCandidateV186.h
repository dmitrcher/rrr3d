#pragma once
#include "physics/NativeGameStabilizeV185.h"
namespace motorrock::stabilizev186 {
namespace S=stabilizev185;
using Vector=S::Vector;using Quaternion=S::Quaternion;
inline Vector stabilize(const Quaternion&q,const Vector&inv,const Vector&omega,
 const Vector&damping,double roll,double pitch,bool contact,bool clutch,float strength,float mass){
 Vector inertia{};for(unsigned i=0;i<3;++i){volatile float v=1.f/inv[i];inertia[i]=v;}
 const auto mat=S::P::orientation(q);const auto world=S::P::momentum(S::P::inertia(mat,inertia),omega);
 if(damping==Vector{{-1,-1,-1}} && roll<=0 && pitch<=0 && !clutch)return world;
 auto local=S::product(S::inverse(mat),world);
 if(!contact){if(roll>0){const float limit=S::P::mul(S::P::mul(inertia[0],float(roll)),2.f);local[0]=std::max(-limit,std::min(limit,local[0]));}if(pitch>0){const float limit=S::P::mul(S::P::mul(inertia[1],float(pitch)),2.f);local[1]=std::max(-limit,std::min(limit,local[1]));}}
 const float z=clutch?(strength!=0.f?S::P::mul(strength,mass):local[2]):S::P::mul(local[2],damping[2]);
 local={{S::P::mul(local[0],damping[0]),S::P::mul(local[1],damping[1]),z}};return S::product(mat,local);
}
}
