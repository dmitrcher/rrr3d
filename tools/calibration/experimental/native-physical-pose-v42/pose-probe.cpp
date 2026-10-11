#include <cstdio>
#include "NativePhysicalPoseV42.h"
int main(){float v[7];while(scanf("%f",v)==1){for(int i=1;i<7;++i)if(scanf("%f",v+i)!=1)return 2;auto r=motorrock::physicalposev42::rotation({{v[3],v[4],v[5],v[6]}});for(int col=0;col<3;++col)for(int row=0;row<3;++row)printf("%.9g ",r.a[row][col]);printf("%.9g %.9g %.9g\n",v[0],v[1],v[2]);}}
