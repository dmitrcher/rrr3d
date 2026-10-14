#include "NativePhysicalPoseV42.h"
#include "NativeContactCache.h"
#include <iostream>
#include <cstring>
float f(unsigned w){float v;std::memcpy(&v,&w,4);return v;}
unsigned bits(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
int main(){unsigned a[10];while(std::cin>>a[0]){for(int i=1;i<10;++i)std::cin>>a[i];namespace nc=motorrock::nativecache;nc::Pose p{motorrock::physicalposev42::rotation({{f(a[0]),f(a[1]),f(a[2]),f(a[3])}}),{{f(a[4]),f(a[5]),f(a[6])}}};auto local=nc::local(p,{{f(a[7]),f(a[8]),f(a[9])}});auto lever=nc::solverLever(p,local);for(int c=0;c<3;++c)for(int r=0;r<3;++r)std::cout<<bits(p.rotation.a[r][c])<<' ';for(auto v:{local,lever})for(int i=0;i<3;++i)std::cout<<bits(v[i])<<' ';std::cout<<'\n';}}
