#include <cstdio>
#include "NativeTriangleNormalV101.h"
int main(){float v[9];while(scanf("%f",v)==1){for(int i=1;i<9;i++)if(scanf("%f",v+i)!=1)return 2;
std::array<motorrock::wheelrayv52::Vector,3> vertices;
for(int p=0;p<3;p++)for(int a=0;a<3;a++)vertices[p][a]=v[3*p+a];
auto r=motorrock::trianglenormalv101::compute(vertices);for(auto x:r.raw)printf("%.9g ",x);printf("%.9g %.9g ",r.length,r.inverse);for(auto x:r.normal)printf("%.9g ",x);puts("");}}
