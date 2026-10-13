#include <cstdio>
#include "NativeMaterialFrameV36.h"
int main(){float v[8];while(scanf("%f",v)==1){for(int i=1;i<8;++i)if(scanf("%f",v+i)!=1)return 2;
auto q=motorrock::framev36::solverMultiply({{v[0],v[1],v[2],v[3]}},{{v[4],v[5],v[6],v[7]}});for(int i=0;i<4;++i)printf("%.9g ",q[i]);puts("");}}