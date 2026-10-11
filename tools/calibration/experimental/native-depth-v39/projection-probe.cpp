#include <cstdio>
#include "SourceMeshManifold.h"
int main(){namespace s=r3d::physics::sourcemesh;float v[19];while(scanf("%f",v)==1){for(int i=1;i<19;++i)if(scanf("%f",v+i)!=1)return 2;s::Cm::Matrix34 transform({v[0],v[1],v[2]},{v[3],v[4],v[5]},{v[6],v[7],v[8]},{v[9],v[10],v[11]});const auto rot=s::findRotationMatrixFromZ({v[12],v[13],v[14]});const auto flat=s::transformTranspose(rot,transform);float z=s::transformZ({v[15],v[16],v[17]},flat);printf("%.9g %.9g %.9g %.9g %.9g\n",flat.p.x,flat.p.y,flat.p.z,z,z-v[18]);}}
