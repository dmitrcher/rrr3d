#ifndef MR_V147_RELATIVE
#define MR_V147_RELATIVE 1
#endif
#ifndef MR_V147_SHARED
#define MR_V147_SHARED 1
#endif
#include "SourceMeshManifold.h"
#include "NativePlacedMeshV126.h"
#include "NativeRelativeContactV147.h"
#include <cstdio>
#include <cstring>
namespace rc=motorrock::relativecontactv147;namespace pp=motorrock::physicalposev42;
float read(){unsigned w;if(scanf("%u",&w)!=1)throw 1;float f;memcpy(&f,&w,4);return f;}
unsigned word(float f){unsigned w;memcpy(&w,&f,4);return w;}
namespace sm=r3d::physics::sourcemesh;namespace pm=motorrock::placedmeshv126;namespace wr=motorrock::wheelrayv52;


sm::PxVec3 vec(){float x=read(),y=read(),z=read();return {x,y,z};}
sm::PxVec3 invdir(const pm::Pose& p,sm::PxVec3 v){auto r=pm::inverseDirection(p,{{v.x,v.y,v.z}});return {r[0],r[1],r[2]};}
int main(int argc,char** argv){try{auto half=vec();auto c0=vec(),c1=vec(),c2=vec(),pos=vec();motorrock::contactframe::Quaternion q;for(unsigned j=0;j<4;++j)q[j]=read();auto p=vec();auto pose=pm::pose(q,{{p.x,p.y,p.z}});auto local=pm::inverseDirection(pose,wr::subtract({{pos.x,pos.y,pos.z}},{{p.x,p.y,p.z}}));sm::Cm::Matrix34 box{{invdir(pose,c0),invdir(pose,c1),invdir(pose,c2)},{local[0],local[1],local[2]}};
float bodyPosition[3];motorrock::contactframe::Quaternion bodyQ,meshQ;FILE* poses=fopen(argc>1?argv[1]:"work/relative-contact-v147/source-poses.words","r");if(!poses)return 3;unsigned u;auto rd=[&](){fscanf(poses,"%u",&u);float f;memcpy(&f,&u,4);return f;};for(auto& v:bodyPosition)v=rd();for(int j=0;j<4;++j)bodyQ[j]=rd();fclose(poses);meshQ=pose.quaternion;const rc::Pose bodyPose{{{bodyPosition[0],bodyPosition[1],bodyPosition[2]}},bodyQ},meshPose{{{p.x,p.y,p.z}},meshQ};const auto relative=rc::relative(meshPose,bodyPose);auto rotation=relative.rotation;
#if MR_V147_RELATIVE
box.m={{rotation.a[0][0],rotation.a[1][0],rotation.a[2][0]},{rotation.a[0][1],rotation.a[1][1],rotation.a[2][1]},{rotation.a[0][2],rotation.a[1][2],rotation.a[2][2]}};
#endif

std::array<std::array<sm::PxVec3,3>,32> ts;for(auto& t:ts)for(auto& v:t)v=vec();std::array<unsigned,32> masks; masks.fill(7);
for(unsigned a=0;a<32;++a)for(unsigned b=a+1;b<32;++b){if(sm::trianglePlane(ts[a]).n.dot(sm::trianglePlane(ts[b]).n)<.999999f)continue;for(unsigned e=0;e<3;++e)for(unsigned f=0;f<3;++f){auto a0=ts[a][e],a1=ts[a][(e+1)%3],b0=ts[b][f],b1=ts[b][(f+1)%3];auto eq=[](sm::PxVec3 x,sm::PxVec3 y){return x.x==y.x&&x.y==y.y&&x.z==y.z;};if((eq(a0,b0)&&eq(a1,b1))||(eq(a0,b1)&&eq(a1,b0))){masks[a]&=~(1u<<e);masks[b]&=~(1u<<f);}}}
sm::PxVec3 lo(FLT_MAX),hi(-FLT_MAX);for(int m=0;m<8;++m){auto x=box.transform({(m&1)?half.x:-half.x,(m&2)?half.y:-half.y,(m&4)?half.z:-half.z});for(int j=0;j<3;++j){lo[j]=std::min(lo[j],x[j]);hi[j]=std::max(hi[j],x[j]);}}
sm::ContactBuffer out;auto inverse=box.getInverseRT();auto inv=rc::relative(bodyPose,meshPose);const auto& im=inv.rotation;const auto& it=inv.position;
#if MR_V147_RELATIVE
inverse={{{im.a[0][0],im.a[1][0],im.a[2][0]},{im.a[0][1],im.a[1][1],im.a[2][1]},{im.a[0][2],im.a[1][2],im.a[2][2]}},{it[0],it[1],it[2]}};
#endif
const auto shared=sm::trianglePlane(ts[30]);fprintf(stderr,"plane %u %u %u %u\n",word(shared.n.x),word(shared.n.y),word(shared.n.z),word(shared.d));for(unsigned t=0;t<32;++t){bool overlap=true;for(unsigned j=0;j<3;++j){float mn=FLT_MAX,mx=-FLT_MAX;for(auto x:ts[t]){mn=std::min(mn,x[j]);mx=std::max(mx,x[j]);}overlap&=mn<=hi[j]+.05f&&mx>=lo[j]-.05f;}if(overlap)sm::faceContacts(out,half,box,ts[t],t,true,masks[t],&inverse,MR_V147_SHARED&&(t==30||t==31)?&shared:nullptr);}
printf("%u\n",out.count);for(unsigned i=0;i<out.count;++i){auto x=out.contacts[i];auto p=pm::point(pose,{{x.point.x,x.point.y,x.point.z}}),n=pm::direction(pose,{{x.normal.x,x.normal.y,x.normal.z}});printf("%u %u %u %u %u %u %u\n",word(p[0]),word(p[1]),word(p[2]),word(n[0]),word(n[1]),word(n[2]),word(x.separation));}return 0;}catch(int){return 2;}}
