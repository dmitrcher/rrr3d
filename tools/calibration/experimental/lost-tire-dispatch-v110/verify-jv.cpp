#include "NativeSpringSplitV53.h"
#include <cstring>
#include <iostream>
float f(unsigned w){float v;std::memcpy(&v,&w,4);return v;}
unsigned bits(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
int main(){unsigned words[12];while(std::cin>>words[0]){for(int i=1;i<12;++i)std::cin>>words[i];float a[3],j[3],v[3],w[3];for(int i=0;i<3;++i){a[i]=f(words[i]);j[i]=f(words[3+i]);v[i]=f(words[6+i]);w[i]=f(words[9+i]);}std::cout<<bits(motorrock_native_spring_v53::body0_jv(a,j,v,w))<<'\n';}}
