#include "SourceMeshManifold.h"
#include "NativeRelativeContactV147.h"
#include "NativePlacedMeshV126.h"
#include "NativeMeshPartsV149.h"
#include <cstdio>
#include <cstring>
#include <map>
namespace sm=r3d::physics::sourcemesh;namespace rc=motorrock::relativecontactv147;
namespace pm=motorrock::placedmeshv126;namespace cf=motorrock::contactframe;
float rd(){unsigned w;if(std::scanf("%u",&w)!=1)throw 1;float f;std::memcpy(&f,&w,4);return f;}
unsigned word(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
sm::PxVec3 vec(){float x=rd(),y=rd(),z=rd();return {x,y,z};}
sm::Cm::Matrix34 convert(const rc::Transform& t){return {{{t.rotation.a[0][0],t.rotation.a[1][0],t.rotation.a[2][0]},{t.rotation.a[0][1],t.rotation.a[1][1],t.rotation.a[2][1]},{t.rotation.a[0][2],t.rotation.a[1][2],t.rotation.a[2][2]}},{t.position[0],t.position[1],t.position[2]}};}
int main(){try{
 auto half=vec(),a=vec(),b=vec(),c=vec(),position=vec();sm::Cm::Matrix34 worldBox{{a,b,c},position};
 cf::Quaternion bodyQ,meshQ;for(unsigned i=0;i<4;++i)bodyQ[i]=rd();for(unsigned i=0;i<4;++i)meshQ[i]=rd();auto meshP=vec();const auto mesh=pm::pose(meshQ,{{meshP.x,meshP.y,meshP.z}});
 rc::Pose bodyPose{{{position.x,position.y,position.z}},bodyQ},meshPose{{{meshP.x,meshP.y,meshP.z}},mesh.quaternion};
 const auto box=convert(rc::relative(meshPose,bodyPose)),inverse=convert(rc::relative(bodyPose,meshPose));
 std::vector<motorrock::meshpartsv149::Triangle> ts(32);for(auto& t:ts)for(auto& v:t)v=vec();
 const auto parts=motorrock::meshpartsv149::build(ts);std::array<unsigned,32> masks;masks.fill(7);
 using Vertex=std::array<float,3>;using Edge=std::array<Vertex,2>;std::map<Edge,std::vector<std::pair<unsigned,unsigned>>> edges;
 for(unsigned i=0;i<32;++i)for(unsigned e=0;e<3;++e){const auto p=ts[i][e],q=ts[i][(e+1)%3];Edge key{{{{p.x,p.y,p.z}},{{q.x,q.y,q.z}}}};if(key[1]<key[0])std::swap(key[0],key[1]);edges[key].push_back({i,e});}
 for(const auto& e:edges)if(e.second.size()==2){const auto a=e.second[0],b=e.second[1];if(sm::trianglePlane(ts[a.first]).n.dot(sm::trianglePlane(ts[b.first]).n)>.999999f){masks[a.first]&=~(1u<<a.second);masks[b.first]&=~(1u<<b.second);}}
 sm::PxVec3 lo(FLT_MAX),hi(-FLT_MAX);for(unsigned mask=0;mask<8;++mask){auto v=box.transform({mask&1?half.x:-half.x,mask&2?half.y:-half.y,mask&4?half.z:-half.z});for(unsigned j=0;j<3;++j){lo[j]=std::min(lo[j],v[j]);hi[j]=std::max(hi[j],v[j]);}}
 for(unsigned cull=0;cull<2;++cull){
  sm::ContactBuffer out;std::map<unsigned,sm::PxPlane> patches;
  for(unsigned i=0;i<32;++i){bool overlap=true;for(unsigned j=0;j<3;++j){float mn=FLT_MAX,mx=-FLT_MAX;for(auto v:ts[i]){mn=std::min(mn,v[j]);mx=std::max(mx,v[j]);}overlap&=mn<=hi[j]+.05f&&mx>=lo[j]-.05f;}if(!overlap)continue;
   if(cull && sm::boxtrianglesatv60::select(half,box,ts[i],7).separated)continue;
   auto plane=patches.emplace(parts.flat[i],sm::trianglePlane(ts[i])).first;
   const auto before=out.count;sm::faceContacts(out,half,box,ts[i],i,true,masks[i],&inverse,&plane->second,&worldBox);
   for(unsigned j=before;j<out.count;++j){auto& p=out.contacts[j];if(!p.worldPointV151){auto v=pm::point(mesh,{{p.point.x,p.point.y,p.point.z}});p.point={v[0],v[1],v[2]};}auto n=pm::direction(mesh,{{p.normal.x,p.normal.y,p.normal.z}});p.normal={n[0],n[1],n[2]};}
  }
  std::printf("mode %u count %u\n",cull,out.count);for(unsigned i=0;i<out.count;++i){const auto& p=out.contacts[i];std::printf("%u %u %u %u %u %u %u %u\n",p.internalFaceIndex1,word(p.point.x),word(p.point.y),word(p.point.z),word(p.normal.x),word(p.normal.y),word(p.normal.z),word(p.separation));}
 }
 return 0;
}catch(int){return 2;}}
