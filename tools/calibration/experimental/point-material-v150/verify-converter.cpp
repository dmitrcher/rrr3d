#include "NativeMaterialFrameV36.h"
#include <cstdio>
#include <cstring>
namespace cf=motorrock::contactframe;
unsigned word(float f){unsigned w;std::memcpy(&w,&f,4);return w;}
float read(){unsigned w;if(std::scanf("%u",&w)!=1)throw 1;float f;std::memcpy(&f,&w,4);return f;}
int main(){try{
 cf::Quaternion actor,raw;
 for(unsigned i=0;i<4;++i)actor[i]=read();
 for(unsigned i=0;i<4;++i)raw[i]=read();
 for(unsigned i=0;i<3;++i)actor[i]=-actor[i];
 // Core743c7..744fa: cross terms, actor.w * raw.xyz, raw.w * actor.xyz;
 // scalar subtracts the Z,Y,X sum. V98 solverMultiply has this exact order.
 const auto local=motorrock::framev36::solverMultiply(actor,raw);
 for(unsigned i=0;i<4;++i)std::printf("%u%c",word(local[i]),i==3?'\n':' ');
 return 0;
}catch(int){return 2;}}
