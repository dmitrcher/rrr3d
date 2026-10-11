#pragma once
// v33 experimental Native material error transform. First identity-body flat
// mesh contacts verified against actual private Native preparation. Arbitrary
// raw seed construction and tilted actor/frame combinations remain unaccepted.
// No recorded errors, case identifiers or expected outputs are loaded here.
#include "SourceMaterialFrameCandidate.h"
namespace motorrock { namespace derivederror {
namespace cf=motorrock::contactframe;
namespace mf=motorrock::materialcandidate;
inline float add(float a,float b){return cf::store(double(a)+b);}
inline float sub(float a,float b){return cf::store(double(a)-b);}
inline float mul(float a,float b){return cf::store(double(a)*b);}
inline cf::Vector rotate(cf::Quaternion q,cf::Vector v,bool inverse) {
 const float x=q[0],y=q[1],z=q[2],w=q[3];
 const cf::Vector cross={{sub(mul(y,v[2]),mul(z,v[1])),sub(mul(z,v[0]),mul(x,v[2])),sub(mul(x,v[1]),mul(y,v[0]))}};
 const float dot=add(add(mul(q[inverse?2:1],v[inverse?2:1]),mul(q[inverse?1:2],v[inverse?1:2])),mul(x,v[0]));
 const float scale=sub(mul(w,w),.5F);cf::Vector out;
 for(int i=0;i<3;++i){const float a=mul(scale,v[i]),b=mul(w,cross[i]);out[i]=mul(add(inverse?sub(a,b):add(a,b),mul(q[i],dot)),2.0F);}return out;
}
struct Frames {cf::Quaternion raw,material;};
inline Frames frames(const cf::Matrix& actor,cf::Quaternion actorQuaternion,cf::Vector normal) {
 const auto n=cf::unit(normal);cf::Vector axis={{1,0,0}},tangent=cf::cross(n,axis);
 if(cf::length(tangent)<=.0001F){axis=cf::Vector{{0,0,1}};tangent=cf::cross(n,axis);}
 tangent=cf::unit(tangent);const auto rawWorld=cf::encode(cf::columns(n,tangent,cf::cross(n,tangent)));
 auto inverseActor=actorQuaternion;for(int i=0;i<3;++i)inverseActor[i]=-inverseActor[i];
 const auto rawLocal=mf::multiply(inverseActor,rawWorld);
 Frames f;f.raw=mf::multiply(actorQuaternion,rawLocal);
 const auto candidate=mf::compute(actor,actorQuaternion,n);f.material=mf::multiply(actorQuaternion,candidate.seed);return f;
}
inline float error(float rawError,const Frames& f) {
 const cf::Vector initial={{rawError,0,0}};return rotate(f.material,rotate(f.raw,initial,false),true)[0];
}
}}
