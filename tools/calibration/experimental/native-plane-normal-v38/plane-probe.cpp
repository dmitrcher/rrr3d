#include <cstdio>
#include "SourceMeshManifold.h"
int main(){namespace sm=r3d::physics::sourcemesh;float v[9];while(scanf("%f",v)==1){for(int i=1;i<9;++i)if(scanf("%f",v+i)!=1)return 2;const auto p=sm::nativeplane::triangle({v[0],v[1],v[2]},{v[3],v[4],v[5]},{v[6],v[7],v[8]});printf("%.9g %.9g %.9g %.9g\n",p.n.x,p.n.y,p.n.z,p.d);}}
