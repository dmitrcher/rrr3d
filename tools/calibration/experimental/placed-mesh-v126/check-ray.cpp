#include "NativePlacedMeshV126.h"
#include <cstdio>
#include <cstring>
#include <cstdint>
namespace pm=motorrock::placedmeshv126;namespace wr=motorrock::wheelrayv52;
float value(uint32_t u){float f;std::memcpy(&f,&u,4);return f;}uint32_t bits(float f){uint32_t u;std::memcpy(&u,&f,4);return u;}
int main(){uint32_t u[27];while(std::scanf("%u",u)==1){for(int i=1;i<27;++i)if(std::scanf("%u",u+i)!=1)return 2;wr::Vector a,b,c,start,unit;pm::Pose p;for(unsigned i=0;i<3;++i){a[i]=value(u[i]);b[i]=value(u[3+i]);c[i]=value(u[6+i]);p.position[i]=value(u[18+i]);start[i]=value(u[21+i]);unit[i]=value(u[24+i]);for(unsigned j=0;j<3;++j)p.matrix.a[j][i]=value(u[9+i*3+j]);}auto local=pm::inversePoint(p,start),dir=pm::inverseDirection(p,unit);auto h=wr::intersect(local,dir,a,b,c);auto point=wr::interpolate(a,b,c,h.u,h.v),world=pm::point(p,point);float data[]={local[0],local[1],local[2],dir[0],dir[1],dir[2],h.distance,h.u,h.v,point[0],point[1],point[2],world[0],world[1],world[2]};for(unsigned i=0;i<15;++i)std::printf("%s%u",i?" ":"",bits(data[i]));std::puts("");}}
