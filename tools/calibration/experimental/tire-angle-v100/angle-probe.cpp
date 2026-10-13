#include <cstdio>
#include "NativeTireV55.h"
int main(){float v[6];while(scanf("%f",v)==1){for(int i=1;i<6;i++)if(scanf("%f",v+i)!=1)return 2;
namespace nt=motorrock::tirev55;nt::Vector r={v[0],v[1],v[2]},n={v[3],v[4],v[5]};float c=nt::div(nt::dotZYX(n,r),std::sqrt(nt::dotZYX(r,r)));printf("%.9g %.9g\n",nt::lateralAngle(r,n),c);}}
