#pragma once
#include <array>
#include <cstdint>
#include "NativeWheelRayV52.h"
namespace motorrock::energysleepv127 {
namespace s=wheelrayv52;using Vector=s::Vector;using Quaternion=std::array<float,4>;
struct Input { Vector position,previousPosition,inverseInertia; Quaternion rotation,previousRotation;float inverseMass,threshold,dt;std::uint32_t interactionCount; };
struct State { Vector linear{{0,0,0}},angular{{0,0,0}};float wake=.3999999761581421F; };
struct Result { Vector linearIncrement{},angularIncrement{};float energy=0,threshold=0;bool evaluated=false,reset=false,sleep=false; };
inline Vector rotateInverse(Quaternion q,Vector v) {
 const float dot=s::add(s::add(s::mul(v[1],q[1]),s::mul(v[0],q[0])),s::mul(v[2],q[2]));
 Vector p{{s::mul(dot,q[0]),s::mul(dot,q[1]),s::mul(dot,q[2])}};
 const Vector cross{{s::sub(s::mul(v[2],q[1]),s::mul(v[1],q[2])),s::sub(s::mul(v[0],q[2]),s::mul(v[2],q[0])),s::sub(s::mul(v[1],q[0]),s::mul(v[0],q[1]))}};
 const float scale=s::sub(s::mul(q[3],q[3]),.5F);
 for(unsigned i=0;i<3;++i)p[i]=s::mul(s::add(s::sub(s::mul(v[i],scale),s::mul(cross[i],q[3])),p[i]),2);
 return p;
}
inline Result step(State& state,const Input& in) {
 Result r;
 if(state.wake<.19999998807907104F) {
  r.evaluated=true;const float inv=in.dt>0?s::div(1,in.dt):0;
  for(unsigned i=0;i<3;++i)r.linearIncrement[i]=s::mul(s::sub(in.position[i],in.previousPosition[i]),inv);
  const auto q=in.rotation;const auto p=in.previousRotation;const float X=-p[0],Y=-p[1],Z=-p[2],W=p[3];
  Vector a{{s::sub(s::add(s::add(s::mul(Z,q[1]),s::mul(W,q[0])),s::mul(X,q[3])),s::mul(Y,q[2])),
            s::sub(s::add(s::add(s::mul(W,q[1]),s::mul(Y,q[3])),s::mul(X,q[2])),s::mul(Z,q[0])),
            s::sub(s::add(s::add(s::mul(W,q[2]),s::mul(Y,q[0])),s::mul(Z,q[3])),s::mul(X,q[1]))}};
  // Multiplication orders follow Core1b29fc..1b2ace; quaternion hemisphere only.
  const float w=s::sub(s::sub(s::sub(s::mul(W,q[3]),s::mul(X,q[0])),s::mul(Y,q[1])),s::mul(Z,q[2]));
  if(w<0)for(float& v:a)v=-v;
  for(float& v:a)v=s::mul(s::mul(v,inv),2);
  r.angularIncrement=rotateInverse(q,a);
  for(unsigned i=0;i<3;++i){state.linear[i]=s::add(state.linear[i],r.linearIncrement[i]);state.angular[i]=s::add(state.angular[i],r.angularIncrement[i]);}
  const float contacts=float(double(in.interactionCount)+1);r.threshold=float(double(in.threshold)*contacts);
  float angular=s::add(s::add(s::mul(s::mul(state.angular[2],state.angular[2]),s::div(1,in.inverseInertia[2])),s::mul(s::mul(state.angular[1],state.angular[1]),s::div(1,in.inverseInertia[1]))),s::mul(s::mul(state.angular[0],state.angular[0]),s::div(1,in.inverseInertia[0])));
  angular=s::mul(angular,in.inverseMass);
  const float linear=s::add(s::add(s::mul(state.linear[0],state.linear[0]),s::mul(state.linear[1],state.linear[1])),s::mul(state.linear[2],state.linear[2]));
  r.energy=s::mul(s::add(angular,linear),.5F);
  if(r.energy>r.threshold) {
   const float factor=s::div(r.energy,r.threshold);state.wake=s::add(s::mul(contacts,in.dt),s::mul(factor<2?factor:2,.19999998807907104F));
   state.linear={{0,0,0}};state.angular={{0,0,0}};r.reset=true;
  }
 }
 if(state.wake!=0)state.wake=s::sub(state.wake,in.dt);
 if(state.wake<=0){state.wake=0;state.linear={{0,0,0}};state.angular={{0,0,0}};r.sleep=true;}
 return r;
}
}
