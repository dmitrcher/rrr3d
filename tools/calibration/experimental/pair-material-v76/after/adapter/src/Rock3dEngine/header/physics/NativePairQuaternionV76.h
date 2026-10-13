#pragma once
#include "NativeMaterialFrameV36.h"
namespace motorrock { namespace pairquaternionv76 {
// Original local-pose getter RVA73e98..74015: cross term first, then
// the two scalar products; scalar component uses an XYZ dot product.
inline contactframe::Quaternion localMultiply(contactframe::Quaternion a,contactframe::Quaternion b) {
 namespace de=derivederror;
 const float x=a[0],y=a[1],z=a[2],w=a[3],X=b[0],Y=b[1],Z=b[2],W=b[3];
 return {{de::add(de::add(de::sub(de::mul(y,Z),de::mul(z,Y)),de::mul(w,X)),de::mul(x,W)),
          de::add(de::add(de::sub(de::mul(z,X),de::mul(x,Z)),de::mul(w,Y)),de::mul(y,W)),
          de::add(de::add(de::sub(de::mul(x,Y),de::mul(y,X)),de::mul(w,Z)),de::mul(z,W)),
          de::sub(de::mul(w,W),de::add(de::add(de::mul(x,X),de::mul(y,Y)),de::mul(z,Z)))}};
}
// Material conversion RVA203dea..203f38 has its own scalar association.
inline contactframe::Quaternion materialLocalMultiply(contactframe::Quaternion a,contactframe::Quaternion b) {
 namespace de=derivederror;
 const float x=a[0],y=a[1],z=a[2],w=a[3],X=b[0],Y=b[1],Z=b[2],W=b[3];
 return {{de::sub(de::add(de::add(de::mul(y,Z),de::mul(x,W)),de::mul(w,X)),de::mul(z,Y)),
          de::sub(de::add(de::add(de::mul(y,W),de::mul(w,Y)),de::mul(z,X)),de::mul(x,Z)),
          de::sub(de::add(de::add(de::mul(z,W),de::mul(w,Z)),de::mul(x,Y)),de::mul(y,X)),
          de::sub(de::sub(de::sub(de::mul(w,W),de::mul(x,X)),de::mul(y,Y)),de::mul(z,Z))}};
}
}}
