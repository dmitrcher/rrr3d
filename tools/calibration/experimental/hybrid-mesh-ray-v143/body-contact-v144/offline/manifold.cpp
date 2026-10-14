#include "SourceMeshManifold.h"
#include "NativePlacedMeshV126.h"
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>
namespace sm=r3d::physics::sourcemesh;namespace pm=motorrock::placedmeshv126;namespace wr=motorrock::wheelrayv52;
float read(){unsigned w;if(scanf("%u",&w)!=1)throw 1;float f;memcpy(&f,&w,4);return f;}
unsigned word(float f){unsigned w;memcpy(&w,&f,4);return w;}
sm::PxVec3 vec(){float x=read(),y=read(),z=read();return {x,y,z};}
sm::PxVec3 invdir(const pm::Pose& p,sm::PxVec3 v){auto r=pm::inverseDirection(p,{{v.x,v.y,v.z}});return {r[0],r[1],r[2]};}
int main(){try{auto half=vec();auto c0=vec(),c1=vec(),c2=vec(),pos=vec();motorrock::contactframe::Quaternion q;for(unsigned j=0;j<4;++j)q[j]=read();auto p=vec();auto pose=pm::pose(q,{{p.x,p.y,p.z}});auto local=pm::inversePoint(pose,{{pos.x,pos.y,pos.z}});sm::Cm::Matrix34 box{{invdir(pose,c0),invdir(pose,c1),invdir(pose,c2)},{local[0],local[1],local[2]}};
std::array<std::array<sm::PxVec3,3>,32> ts;for(auto& t:ts)for(auto& v:t)v=vec();std::array<unsigned,32> masks; masks.fill(7);
for(unsigned a=0;a<32;++a)for(unsigned b=a+1;b<32;++b){if(sm::trianglePlane(ts[a]).n.dot(sm::trianglePlane(ts[b]).n)<.999999f)continue;for(unsigned e=0;e<3;++e)for(unsigned f=0;f<3;++f){auto a0=ts[a][e],a1=ts[a][(e+1)%3],b0=ts[b][f],b1=ts[b][(f+1)%3];auto eq=[](sm::PxVec3 x,sm::PxVec3 y){return x.x==y.x&&x.y==y.y&&x.z==y.z;};if((eq(a0,b0)&&eq(a1,b1))||(eq(a0,b1)&&eq(a1,b0))){masks[a]&=~(1u<<e);masks[b]&=~(1u<<f);}}}
sm::PxVec3 lo(FLT_MAX),hi(-FLT_MAX);for(int m=0;m<8;++m){auto x=box.transform({(m&1)?half.x:-half.x,(m&2)?half.y:-half.y,(m&4)?half.z:-half.z});for(int j=0;j<3;++j){lo[j]=std::min(lo[j],x[j]);hi[j]=std::max(hi[j],x[j]);}}
sm::ContactBuffer out;auto inverse=box.getInverseRT();for(unsigned t=0;t<32;++t){bool overlap=true;for(unsigned j=0;j<3;++j){float mn=FLT_MAX,mx=-FLT_MAX;for(auto x:ts[t]){mn=std::min(mn,x[j]);mx=std::max(mx,x[j]);}overlap&=mn<=hi[j]+.05f&&mx>=lo[j]-.05f;}if(overlap)sm::faceContacts(out,half,box,ts[t],t,true,masks[t],&inverse);}
printf("%u\n",out.count);for(unsigned i=0;i<out.count;++i){auto x=out.contacts[i];auto p=pm::point(pose,{{x.point.x,x.point.y,x.point.z}}),n=pm::direction(pose,{{x.normal.x,x.normal.y,x.normal.z}});printf("%u %u %u %u %u %u %u\n",word(p[0]),word(p[1]),word(p[2]),word(n[0]),word(n[1]),word(n[2]),word(x.separation));}return 0;}catch(int){return 2;}}
