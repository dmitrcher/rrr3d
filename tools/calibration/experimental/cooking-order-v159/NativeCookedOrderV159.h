#pragma once
#include <array>
#include <numeric>
#include <vector>

// Geometry-only primitive remapping used by Opcode's hybrid tree builder.
// Algorithm reference: OPCODE, Copyright (C) 2001 Pierre Terdiman,
// https://www.codercorner.com/Opcode.htm (free commercial/non-commercial use).
// References: OPC_AABBTree::Split/Subdivide and OPC_TreeBuilders; the original
// PhysXCooking DLL sets the hybrid leaf limit to 8 at RVA 29164 (public 1.3:16).
// This does not read measured triangle permutations, tracks, frames or states.
namespace motorrock::cookedorderv159 {
using Vertex=std::array<float,3>;
using Triangle=std::array<Vertex,3>;
inline float store(double x){volatile float v=static_cast<float>(x);return v;}
inline float add(float a,float b){return store(double(a)+b);}
inline float sub(float a,float b){return store(double(a)-b);}
inline float mul(float a,float b){return store(double(a)*b);}
inline float div(float a,float b){return store(double(a)/b);}
inline std::vector<unsigned> build(const std::vector<Triangle>& triangles) {
 const unsigned count=static_cast<unsigned>(triangles.size());
 std::vector<unsigned> order(count);std::iota(order.begin(),order.end(),0);
 std::vector<Vertex> centers(count);
 for(unsigned i=0;i<count;++i)for(unsigned a=0;a<3;++a)
  centers[i][a]=mul(add(add(triangles[i][0][a],triangles[i][1][a]),triangles[i][2][a]),1.0f/3.0f);
 const auto recurse=[&](auto&& self,unsigned lo,unsigned hi)->void {
  const unsigned n=hi-lo;if(n<=8)return;
  Vertex means{{0,0,0}},variances{{0,0,0}};
  for(unsigned i=lo;i<hi;++i)for(unsigned a=0;a<3;++a)means[a]=add(means[a],centers[order[i]][a]);
  for(auto& v:means)v=div(v,static_cast<float>(n));
  for(unsigned i=lo;i<hi;++i)for(unsigned a=0;a<3;++a){const float d=sub(centers[order[i]][a],means[a]);variances[a]=add(variances[a],mul(d,d));}
  for(auto& v:variances)v=div(v,static_cast<float>(n-1));
  unsigned axis=0;if(variances[1]>variances[axis])axis=1;if(variances[2]>variances[axis])axis=2;
  float split=0;
  for(unsigned i=lo;i<hi;++i)for(unsigned v=0;v<3;++v)split=add(split,triangles[order[i]][v][axis]);
  split=div(split,static_cast<float>(n*3));
  unsigned mid=lo;
  for(unsigned i=lo;i<hi;++i)if(centers[order[i]][axis]>split){const auto t=order[i];order[i]=order[mid];order[mid++]=t;}
  if(mid==lo || mid==hi)return;
  self(self,lo,mid);self(self,mid,hi);
 };
 recurse(recurse,0,count);return order;
}
}
