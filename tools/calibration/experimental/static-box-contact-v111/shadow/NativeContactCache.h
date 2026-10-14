#pragma once
// Experimental reconstruction of PhysX 2.8.4 mesh-contact point persistence.
// Distances, normals and float operation order come from the native algorithm;
// no recorded state or per-scenario values are consumed by this implementation.
#include "DerivedContactErrorFrame.h"
#include <vector>
#include <cstdlib>
#include <cstring>
namespace motorrock { namespace nativecache {
namespace cf=motorrock::contactframe;
namespace de=motorrock::derivederror;
struct Pose { cf::Matrix rotation; cf::Vector position; };
inline cf::Vector subtract(cf::Vector a,cf::Vector b) {
 for(int i=0;i<3;++i)a[i]=de::sub(a[i],b[i]);return a;
}
inline cf::Vector local(const Pose& p,cf::Vector v) {
 v=subtract(v,p.position);cf::Vector out;
 for(int i=0;i<3;++i) {
  const int a=i==1?2:1,b=i==1?1:2;
  out[i]=de::add(de::add(de::mul(p.rotation.a[a][i],v[a]),de::mul(p.rotation.a[b][i],v[b])),de::mul(p.rotation.a[0][i],v[0]));
 }return out;
}
// Original fresh body-0 cache transport at RVA 0x25381c..0x2538ab
// sums local position X in XYZ order. Normal transforms are a separate path.
inline cf::Vector localBody0Point(const Pose& p,cf::Vector v) {
 auto out=local(p,v);
 const char* flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_CACHE_LOCAL_POINT_ORDER");
 if(flag && std::strcmp(flag,"1")==0) {
  v=subtract(v,p.position);
  out[0]=de::add(de::add(de::mul(p.rotation.a[0][0],v[0]),de::mul(p.rotation.a[1][0],v[1])),de::mul(p.rotation.a[2][0],v[2]));
 }
 return out;
}
inline cf::Vector world(const Pose& p,cf::Vector v) {
 cf::Vector out;
 for(int i=0;i<3;++i) {
  const int a=i==1?1:0,b=i==1?2:1,c=i==1?0:2;
  out[i]=de::add(p.position[i],de::add(de::add(de::mul(p.rotation.a[i][a],v[a]),de::mul(p.rotation.a[i][b],v[b])),de::mul(p.rotation.a[i][c],v[c])));
 }return out;
}
// Solver row body-0 lever reconstruction at RVA 0x74ac1..0x74b74
// uses XYZ sums for all three components, without world translation.
inline cf::Vector solverLever(const Pose& p,cf::Vector v) {
 const char* flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_CACHE_LEVER_ORDER");
 if(!flag || std::strcmp(flag,"1")!=0) {Pose rotation=p;rotation.position={{0,0,0}};return world(rotation,v);}
 cf::Vector out;
 for(unsigned i=0;i<3;++i)out[i]=de::add(de::add(de::mul(p.rotation.a[i][0],v[0]),de::mul(p.rotation.a[i][1],v[1])),de::mul(p.rotation.a[i][2],v[2]));
 return out;
}
inline float distanceSquared(cf::Vector a,cf::Vector b) {
 const auto d=subtract(a,b);return de::add(de::mul(d[2],d[2]),de::add(de::mul(d[1],d[1]),de::mul(d[0],d[0])));
}
inline float dot(cf::Vector a,cf::Vector b,bool reverse) {
 const int i=reverse?2:1,j=reverse?1:2;
 return de::add(de::add(de::mul(a[i],b[i]),de::mul(a[j],b[j])),de::mul(a[0],b[0]));
}
inline cf::Vector directionLocal(const Pose& p,cf::Vector v) {
 Pose r=p;r.position={{0,0,0}};return local(r,v);
}
// The cache uses a unit-quaternion matrix expression, while the subsequent
// material transform uses w*w - 0.5. Non-unit float quaternions distinguish them.
inline cf::Vector transverse(cf::Quaternion q,cf::Vector d) {
 const float x=q[0],y=q[1],z=q[2],w=q[3];cf::Vector out={{0,0,0}};
 const float xy=de::mul(x,y),zw=de::mul(z,w),xw=de::mul(x,w),yz=de::mul(y,z);
 const float xx=de::mul(x,x),yy=de::mul(y,y),zz=de::mul(z,z);
 out[1]=de::add(de::add(de::mul(de::mul(de::sub(xy,zw),2),d[0]),de::mul(de::mul(de::add(xw,yz),2),d[2])),de::mul(de::sub(1,de::mul(de::add(zz,xx),2)),d[1]));
 out[2]=de::add(de::add(de::mul(de::mul(de::add(de::mul(y,w),de::mul(x,z)),2),d[0]),de::mul(de::sub(1,de::mul(de::add(xx,yy),2)),d[2])),de::mul(de::mul(de::sub(yz,xw),2),d[1]));
 return out;
}
struct Point { cf::Vector on0,on1,normal0,normal1; };
struct Cache {
 std::vector<Point> points;
 cf::Quaternion storedFrame={{0,0,0,1}};
 std::vector<cf::Vector> prepare(const Pose& a,const Pose& b,const std::vector<cf::Vector>& current,cf::Vector normal,float threshold,cf::Quaternion frame,std::vector<cf::Vector>* levers=nullptr,bool persistentFrame=false,unsigned dynamicSlot=0) {
  // The ordered pair is the canonical Native pair; either body may be dynamic.
  // Static world points and body-1 local arithmetic must keep their slots.
  const auto n0=directionLocal(a,normal),n1=directionLocal(b,normal);
  std::vector<int> matched;std::vector<bool> used(points.size());bool reuse=points.size()==current.size();
  const float limit=de::mul(threshold,threshold);
  for(const auto& v:current) {
   const auto p0=localBody0Point(a,v),p1=local(b,v);int found=-1;
   if(reuse)for(unsigned j=0;j<points.size();++j)if(!used[j] && distanceSquared(points[j].on0,p0)<limit && distanceSquared(points[j].on1,p1)<limit && dot(points[j].normal0,n0,false)>.999F && dot(points[j].normal1,n1,true)>.999F){found=int(j);used[j]=true;break;}
   if(found<0)reuse=false;matched.push_back(found);
  }
  std::vector<cf::Vector> result(current.size(),cf::Vector{{0,0,0}});
  if(!reuse) {
   if(persistentFrame)storedFrame=frame;
   points.clear();for(const auto& v:current)points.push_back({localBody0Point(a,v),local(b,v),n0,n1});
  } else for(unsigned i=0;i<current.size();++i) {
   const auto& p=points[matched[i]];result[i]=transverse(persistentFrame?storedFrame:frame,subtract(world(a,p.on0),world(b,p.on1)));
  }
  if(levers){levers->clear();for(unsigned i=0;i<current.size();++i)levers->push_back(dynamicSlot==0?solverLever(a,points[reuse?matched[i]:i].on0):solverLever(b,points[reuse?matched[i]:i].on1));}
  return result;
 }
};
}}
