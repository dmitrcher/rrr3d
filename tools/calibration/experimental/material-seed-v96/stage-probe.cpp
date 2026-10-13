#include <cstdio>
#include <initializer_list>
#include "NativeMaterialFrameV36.h"
int main(){namespace cf=motorrock::contactframe;namespace f=motorrock::framev36;
float v[16];while(scanf("%f",v)==1){for(int i=1;i<16;++i)if(scanf("%f",v+i)!=1)return 2;
cf::Matrix m;for(int c=0;c<3;++c)for(int r=0;r<3;++r)m.a[r][c]=v[4+c*3+r];
auto result=f::compute(m,{{v[0],v[1],v[2],v[3]}},{{v[13],v[14],v[15]}},{{v[13],v[14],v[15]}},false);
for(auto q:{result.rawLocal,result.errors.raw,result.errors.material,result.seed,result.callback.local0,result.callback.local1,result.solverQuaternion})for(int i=0;i<4;++i)printf("%.9g ",q[i]);
for(int i=0;i<3;++i)printf("%.9g ",result.normal[i]);puts("");}}