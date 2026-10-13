#include <cstdio>
#include "NativeTireV55.h"
int main(){float v[6];while(scanf("%f",v)==1){for(int i=1;i<6;i++)if(scanf("%f",v+i)!=1)return 2;auto a=motorrock::tirev55::lateralAngle({v[0],v[1],v[2]},{v[3],v[4],v[5]});auto r=motorrock::tirev55::Vector{v[0],v[1],v[2]};auto n=motorrock::tirev55::Vector{v[3],v[4],v[5]};auto cosine=motorrock::tirev55::div(motorrock::tirev55::dotZYX(n,r),std::sqrt(motorrock::tirev55::dotZYX(r,r)));float b=static_cast<float>(std::asin(static_cast<double>(cosine)));printf("%.9g %.9g %.9g\n",a,b,cosine);}}
