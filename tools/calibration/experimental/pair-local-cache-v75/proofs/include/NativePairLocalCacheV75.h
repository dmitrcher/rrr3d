#pragma once
#include "NativeContactCache.h"
#include "NativeMaterialFrameV36.h"
namespace motorrock { namespace paircachev75 {
namespace nc=nativecache;namespace cf=contactframe;namespace de=derivederror;
struct WorldPoint {cf::Vector position,normal;float separation;};
struct StoredPoint {
 cf::Vector position,on0,on1,normal0,normal1;
 float separation;cf::Vector transverse;cf::Quaternion raw;
};
inline float orderedDot(cf::Vector a,cf::Vector b,unsigned i,unsigned j,unsigned k){return de::add(de::add(de::mul(a[i],b[i]),de::mul(a[j],b[j])),de::mul(a[k],b[k]));}
inline cf::Vector localNormal(const nc::Pose& p,cf::Vector n,unsigned body,bool validation){
 const unsigned fresh[2][3][3]={{{1,2,0},{0,1,2},{0,1,2}},{{0,1,2},{1,2,0},{0,1,2}}};
 const unsigned check[2][3][3]={{{0,2,1},{1,2,0},{1,2,0}},{{0,1,2},{1,2,0},{1,2,0}}};
 cf::Vector out;for(unsigned c=0;c<3;++c){const auto* order=validation?check[body][c]:fresh[body][c];out[c]=orderedDot(cf::column(p.rotation,c),n,order[0],order[1],order[2]);}return out;
}
inline cf::Vector lever(const nc::Pose& p,cf::Vector local){cf::Vector out;for(unsigned r=0;r<3;++r)out[r]=orderedDot({{p.rotation.a[r][0],p.rotation.a[r][1],p.rotation.a[r][2]}},local,0,1,2);return out;}
inline cf::Vector worldPoint(const nc::Pose& p,cf::Vector local){auto out=lever(p,local);for(unsigned i=0;i<3;++i)out[i]=de::add(p.position[i],out[i]);return out;}
// Original RVA253ac0: match fresh world candidates against unused old local
// records; refresh all on any failed match, otherwise retain old record order.
struct Cache {
 std::vector<StoredPoint> points;std::vector<unsigned> currentForStored;
 bool prepare(const nc::Pose& a,const nc::Pose& b,const std::vector<WorldPoint>& current,float threshold,float skin){
  bool reuse=points.size()==current.size();std::vector<bool> used(points.size());std::vector<unsigned> matches;const float limit=de::mul(threshold,threshold);
  if(reuse)for(const auto& v:current){const auto p0=nc::local(a,v.position),p1=nc::local(b,v.position);const auto n0=localNormal(a,v.normal,0,true),n1=localNormal(b,v.normal,1,true);int found=-1;
   for(unsigned j=0;j<points.size();++j){const auto& p=points[j];if(!used[j] && nc::distanceSquared(p.on0,p0)<limit && nc::distanceSquared(p.on1,p1)<limit && orderedDot(p.normal0,n0,0,2,1)>.999F && orderedDot(p.normal1,n1,2,1,0)>.999F){found=int(j);used[j]=true;break;}}
   if(found<0){reuse=false;break;}matches.push_back(unsigned(found));
  }
  currentForStored.resize(current.size());
  if(!reuse){points.clear();for(unsigned i=0;i<current.size();++i){const auto& v=current[i];points.push_back({v.position,nc::localBody0Point(a,v.position),nc::local(b,v.position),localNormal(a,v.normal,0,false),localNormal(b,v.normal,1,false),de::add(v.separation,skin),{{0,0,0}},framev36::rawWorld(v.normal)});currentForStored[i]=i;}}
  else for(unsigned i=0;i<current.size();++i){auto& p=points[matches[i]];const auto& v=current[i];currentForStored[matches[i]]=i;p.position=v.position;p.separation=de::add(v.separation,skin);p.raw=framev36::rawWorld(v.normal);p.transverse=nc::transverse(p.raw,nc::subtract(worldPoint(a,p.on0),worldPoint(b,p.on1)));}
  return reuse;
 }
};
}}
