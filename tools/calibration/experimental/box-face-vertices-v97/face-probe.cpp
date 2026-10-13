#include <cstdio>
#include "SourceMeshManifold.h"
int main(int argc,char**argv){if(argc>1){printf("%d\n",r3d::physics::sourcemesh::nativeboxfacesv97::enabled()?1:0);return 0;}float x,y,z,sign;int axis;while(scanf("%f%f%f%d%f",&x,&y,&z,&axis,&sign)==5){auto v=r3d::physics::sourcemesh::nativeboxfacesv97::vertices({x,y,z},axis,sign);for(int i=0;i<4;++i)for(int j=0;j<3;++j)printf("%.9g ",v[i][j]);puts("");}}
