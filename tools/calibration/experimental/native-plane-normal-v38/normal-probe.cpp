#include <cstdio>
#include "NativeContactNormalV38.h"
int main(){namespace cf=motorrock::contactframe;float v[4];while(scanf("%f",v)==1){for(int i=1;i<4;++i)if(scanf("%f",v+i)!=1)return 2;const auto n=motorrock::contactnormalv38::decode({{v[0],v[1],v[2],v[3]}});printf("%.9g %.9g %.9g\n",n[0],n[1],n[2]);}}
