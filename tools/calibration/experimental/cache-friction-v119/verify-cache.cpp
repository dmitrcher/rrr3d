#include "NativeContactCache.h"
#include <iostream>
#include <cstring>
unsigned bits(float v){unsigned w;std::memcpy(&w,&v,4);return w;}float f(unsigned w){float v;std::memcpy(&v,&w,4);return v;}
int main(){unsigned a[43];while(std::cin>>a[0]){for(unsigned i=1;i<43;++i)std::cin>>a[i];namespace N=motorrock::nativecache;N::Pose p[2];for(unsigned b=0;b<2;++b){for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)p[b].rotation.a[r][c]=f(a[b*12+c*3+r]);for(unsigned k=0;k<3;++k)p[b].position[k]=f(a[b*12+9+k]);}auto error=N::transverse({f(a[30]),f(a[31]),f(a[32]),f(a[33])},N::subtract(N::cachedWorldPointV119(p[0],{f(a[24]),f(a[25]),f(a[26])}),N::cachedWorldPointV119(p[1],{f(a[27]),f(a[28]),f(a[29])})));motorrock::derivederror::Frames frames;for(unsigned k=0;k<4;++k){frames.raw[k]=f(a[35+k]);frames.material[k]=f(a[39+k]);}error[0]=f(a[34]);for(unsigned k=0;k<3;++k)std::cout<<bits(error[k])<<' ';std::cout<<bits(motorrock::derivederror::error(error,frames))<<'\n';}}
