#pragma once
// Original contact kernel first-column decode: double quaternion components
// before products, round scalar operations, subtract the combined diagonal.
#include "ContactFrameScalar.h"
namespace motorrock { namespace contactnormalv38 {
namespace cf=contactframe;
inline cf::Vector decode(cf::Quaternion q) {
 const float x2=cf::store(double(q[0])*2),y2=cf::store(double(q[1])*2),z2=cf::store(double(q[2])*2);
 const float yy=cf::store(double(q[1])*y2),zz=cf::store(double(q[2])*z2);
 const float xy=cf::store(double(q[1])*x2),wz=cf::store(double(q[3])*z2);
 const float xz=cf::store(double(q[2])*x2),wy=cf::store(double(q[3])*y2);
 return {{cf::store(1.0-cf::store(double(zz)+yy)),cf::store(double(wz)+xy),cf::store(double(xz)-wy)}};
}
}}
