#include "NativeMaterialFrameV36.h"
#include "NativePlacedMeshV126.h"
#include "SourceMeshManifold.h"
#include <cstdio>
#include <cstring>
namespace cf=motorrock::contactframe;namespace pm=motorrock::placedmeshv126;namespace sm=r3d::physics::sourcemesh;
float rd(){unsigned w;if(scanf("%u",&w)!=1)throw 1;float f;memcpy(&f,&w,4);return f;}unsigned word(float f){unsigned u;memcpy(&u,&f,4);return u;}
int main(){try{cf::Quaternion aq,mq;for(unsigned i=0;i<4;++i)aq[i]=rd();cf::Matrix actor;for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)actor.a[r][c]=rd();for(unsigned i=0;i<4;++i)mq[i]=rd();motorrock::wheelrayv52::Vector mp;for(auto& f:mp)f=rd();auto pose=pm::pose(mq,mp);std::array<std::array<sm::PxVec3,3>,2> ts;for(auto& t:ts)for(auto& v:t){v.x=rd();v.y=rd();v.z=rd();}auto pn=sm::nativeplane::triangle(ts[0][0],ts[0][1],ts[0][2]).n;auto wn=pm::direction(pose,{{pn.x,pn.y,pn.z}});cf::Vector plane={{wn[0],wn[1],wn[2]}};for(auto& t:ts){cf::Vector vs[3];for(unsigned i=0;i<3;++i){auto v=pm::point(pose,{{t[i].x,t[i].y,t[i].z}});vs[i]={{v[0],v[1],v[2]}};}cf::Vector e1,e2;for(unsigned i=0;i<3;++i){e1[i]=cf::store(double(vs[1][i])-vs[0][i]);e2[i]=cf::store(double(vs[2][i])-vs[0][i]);}auto pub=cf::unit(cf::cross(e1,e2));auto result=motorrock::framev36::compute(actor,aq,plane,pub,false);for(unsigned i=0;i<4;++i)printf("%u%c",word(result.solverQuaternion[i]),i==3?'\n':' ');}return 0;}catch(int){return 2;}}
