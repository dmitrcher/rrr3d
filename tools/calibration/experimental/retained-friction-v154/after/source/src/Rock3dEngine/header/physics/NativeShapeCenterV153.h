#pragma once
#include "DerivedContactErrorFrame.h"

// SDK NxQuat::rot/transform (NxQuat.h774..792), with NxVec3 dotXYZ.
// Shape placement is distinct from the material-error rotation's Y,Z,X dot.
// The cached Core path must additionally be checked against live geometry.
namespace motorrock::shapecenterv153 {
namespace cf=contactframe;namespace de=derivederror;
inline cf::Vector center(cf::Quaternion q,cf::Vector position,cf::Vector v) {
 const cf::Vector cross={{de::sub(de::mul(q[1],v[2]),de::mul(q[2],v[1])),
     de::sub(de::mul(q[2],v[0]),de::mul(q[0],v[2])),
     de::sub(de::mul(q[0],v[1]),de::mul(q[1],v[0]))}};
 const float dot=de::add(de::add(de::mul(q[0],v[0]),de::mul(q[1],v[1])),de::mul(q[2],v[2]));
 const float scale=de::sub(de::mul(q[3],q[3]),.5f);
 for(unsigned i=0;i<3;++i)position[i]=de::add(position[i],de::mul(de::add(de::add(de::mul(scale,v[i]),de::mul(q[3],cross[i])),de::mul(q[i],dot)),2.f));
 return position;
}
}
