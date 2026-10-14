#pragma once
#include "NativeMeshPartsV149.h"
#include <numeric>

// Cooking249eb..24a99 sums concave boundary incidences for each flat part.
// Stable ascending radix ranks are consumed backwards at24c80..24cd4;
// the first unassigned face starts the next convex flood. Flat labels are
// numbered in original-face order within each resulting convex part.
namespace motorrock::meshpartsv152 {
namespace sm=r3d::physics::sourcemesh;
using Triangle=meshpartsv149::Triangle;
struct Parts {std::vector<unsigned> convex,flat,score;};
inline Parts build(const std::vector<Triangle>& triangles) {
 const auto base=meshpartsv149::build(triangles);
 using Vertex=std::array<float,3>;using Edge=std::array<Vertex,2>;
 struct Side {unsigned face,edge;};std::map<Edge,std::vector<Side>> edges;
 std::vector<sm::PxPlane> planes;
 for(unsigned i=0;i<triangles.size();++i){const auto& t=triangles[i];planes.push_back(sm::nativeplane::triangle(t[0],t[1],t[2]));for(unsigned e=0;e<3;++e){const auto a=t[e],b=t[(e+1)%3];Edge key{{{{a.x,a.y,a.z}},{{b.x,b.y,b.z}}}};if(key[1]<key[0])std::swap(key[0],key[1]);edges[key].push_back({i,e});}}
 std::map<unsigned,unsigned> scores;
 for(const auto& entry:edges){
  bool cut=entry.second.size()!=2;
  if(!cut){const auto a=entry.second[0],b=entry.second[1];const auto n=planes[a.face].n,m=planes[b.face].n,c=n.cross(m);
   const double sine=std::sqrt(double(c.x)*c.x+double(c.y)*c.y+double(c.z)*c.z);
   const double cosine=double(n.x)*m.x+double(n.y)*m.y+double(n.z)*m.z;
   const auto p=triangles[b.face][(b.edge+2)%3];
   const float d=sm::nativeedge::add(sm::nativeedge::dot(n,p),planes[a.face].d);
   cut=d>0 && std::abs(std::atan2(sine,cosine))>double(.01f);
  }
  if(cut)for(const auto& side:entry.second)++scores[base.flat[side.face]];
 }
 Parts result;const unsigned count=unsigned(triangles.size());result.convex.resize(count);result.flat.resize(count);result.score.resize(count);
 std::vector<unsigned> order(count);std::iota(order.begin(),order.end(),0);
 for(unsigned i=0;i<count;++i)result.score[i]=scores[base.flat[i]];
 std::sort(order.begin(),order.end(),[&](unsigned a,unsigned b){return result.score[a]!=result.score[b]?result.score[a]>result.score[b]:a>b;});
 std::map<unsigned,unsigned> convexLabels;
 for(unsigned i:order)if(!convexLabels.count(base.convex[i]))convexLabels.emplace(base.convex[i],unsigned(convexLabels.size()));
 std::map<unsigned,std::map<unsigned,unsigned>> flatLabels;
 for(unsigned i=0;i<count;++i){const auto convex=convexLabels.at(base.convex[i]);auto& flats=flatLabels[convex];if(!flats.count(base.flat[i]))flats.emplace(base.flat[i],unsigned(flats.size()));result.convex[i]=convex;result.flat[i]=flats.at(base.flat[i]);}
 return result;
}
}
