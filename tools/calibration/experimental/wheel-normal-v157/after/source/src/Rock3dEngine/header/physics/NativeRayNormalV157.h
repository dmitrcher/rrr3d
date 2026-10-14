#pragma once
#include "NativePlacedMeshV126.h"

namespace motorrock::raynormalv157 {
namespace wr=wheelrayv52;
// Core14e2a4..14e393: rotate the unnormalized cross, then normalize in world.
// Rotation sum order is Z,X,Y for X/Z and Y,Z,X for Y. Local normalization
// followed by actor rotation is a different operation on a rounded matrix.
inline wr::Vector compute(const placedmeshv126::Pose& pose,wr::Vector raw) {
 wr::Vector world;
 for(unsigned i=0;i<3;++i) {
  const unsigned a=i==1?1:2,b=i==1?2:0,c=i==1?0:1;
  world[i]=wr::add(wr::add(wr::mul(pose.matrix.a[i][a],raw[a]),wr::mul(pose.matrix.a[i][b],raw[b])),wr::mul(pose.matrix.a[i][c],raw[c]));
 }
 const float square=wr::add(wr::add(wr::mul(world[0],world[0]),wr::mul(world[1],world[1])),wr::mul(world[2],world[2]));
 const float length=static_cast<float>(std::sqrt(static_cast<double>(square)));
 if(length!=0) {const float inverse=wr::div(1.0f,length);for(auto& v:world)v=wr::mul(v,inverse);}
 return world;
}
}
