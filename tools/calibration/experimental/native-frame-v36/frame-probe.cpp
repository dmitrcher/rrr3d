#include <cstdio>
#include <initializer_list>

#include "NativeMaterialFrameV36.h"
int main(){namespace f=motorrock::framev36;namespace cf=motorrock::contactframe;namespace de=motorrock::derivederror;
 float v[26];while(scanf("%f",v)==1){for(int i=1;i<26;++i)if(scanf("%f",v+i)!=1)return 2;
 cf::Quaternion actor={{v[0],v[1],v[2],v[3]}},local={{v[4],v[5],v[6],v[7]}};
 cf::Vector direction={{v[8],v[9],v[10]}},error={{v[11],v[12],v[13]}},plane={{v[23],v[24],v[25]}};
 cf::Matrix rotation;for(int i=0;i<9;++i)rotation.a[i/3][i%3]=v[14+i];
 const auto raw=f::multiply(actor,local);const auto normal=de::rotate(raw,{{1,0,0}},false);
 const auto basis=f::basis(normal,direction);const auto material=f::encode(basis);
 auto inverse=actor;for(int i=0;i<3;++i)inverse[i]=-inverse[i];
 auto forward=material;for(int i=0;i<3;++i)forward[i]=-forward[i];const auto seed=f::multiply(inverse,forward);const auto callback=cf::modify(seed,rotation,cf::Matrix{{{1,0,0},{0,1,0},{0,0,1}}},plane,true);
 const auto solver=motorrock::materialcandidate::solverNormal(callback.local1),global=de::rotate(raw,error,false),out=de::rotate(material,global,false);
 for(int i=0;i<4;++i)printf("%.9g ",raw[i]);for(int j=0;j<3;++j)for(int i=0;i<3;++i)printf("%.9g ",basis.a[i][j]);
 for(int i=0;i<4;++i)printf("%.9g ",material[i]);for(int i=0;i<4;++i)printf("%.9g ",callback.local0[i]);for(int i=0;i<4;++i)printf("%.9g ",callback.local1[i]);
 for(auto x:{solver,global,out})for(int i=0;i<3;++i)printf("%.9g ",x[i]);const auto rawWorld=f::rawWorld(plane);const auto derivedLocal=f::multiply(inverse,rawWorld);for(int i=0;i<4;++i)printf("%.9g ",derivedLocal[i]);puts("");
 }}
