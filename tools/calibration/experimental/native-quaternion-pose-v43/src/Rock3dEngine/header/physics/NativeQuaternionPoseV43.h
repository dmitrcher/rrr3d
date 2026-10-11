#pragma once
// Experimental original Nx quaternion translation, distinct from unit-matrix
// translation. Every operation follows the independently traced scalar stores.
#include "DerivedContactErrorFrame.h"
namespace motorrock { namespace quaternionposev43 {
namespace cf=contactframe; namespace de=derivederror;
inline cf::Vector center(cf::Quaternion q,cf::Vector position,cf::Vector offset) {
 const auto rotated=de::rotate(q,offset,false);
 for(int i=0;i<3;++i)position[i]=de::add(position[i],rotated[i]);return position;
}
inline cf::Vector origin(cf::Quaternion q,cf::Vector center,cf::Vector offset) {
 const auto rotated=de::rotate(q,offset,false);
 for(int i=0;i<3;++i)center[i]=de::sub(center[i],rotated[i]);return center;
}
}}
