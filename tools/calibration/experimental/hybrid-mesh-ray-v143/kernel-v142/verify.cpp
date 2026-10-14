#include "NativeHybridRayV142.h"
#include <cstdio>
#include <cstring>
#include <cstdint>
float read(){unsigned w;if(std::scanf("%u",&w)!=1)throw 1;float f;std::memcpy(&f,&w,4);return f;}
unsigned word(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
int main(){try{while(true){motorrock::wheelrayv52::Vector x[5];for(unsigned i=0;i<5;++i)for(unsigned j=0;j<3;++j)x[i][j]=read();auto r=motorrock::hybridrayv142::compute(x[0],x[1],x[2],x[3],x[4]);std::printf("%u %u %u %u\n",word(r.determinant),word(r.intersection.distance),word(r.intersection.u),word(r.intersection.v));}}catch(int){return 0;}}
