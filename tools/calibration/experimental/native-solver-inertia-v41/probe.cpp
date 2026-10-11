#include <cstdio>
#include "NativeInertiaRotation.h"
int main(){namespace cf=motorrock::contactframe;float v[13];while(scanf("%f",v)==1){for(int i=1;i<13;++i)if(scanf("%f",v+i)!=1)return 2;
 cf::Quaternion q={{v[0],v[1],v[2],v[3]}};cf::Matrix local;for(int i=0;i<9;++i)local.a[i/3][i%3]=v[4+i];
 const auto tensor=motorrock::inertiav37::solver(q,local);for(int col=0;col<3;++col)for(int row=0;row<3;++row)printf("%.9g ",tensor.a[row][col]);puts("");
}}
