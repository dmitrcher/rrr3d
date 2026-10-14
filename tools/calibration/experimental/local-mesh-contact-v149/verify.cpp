#include "NativeMeshPartsV149.h"
#include <cstdio>
#include <cstring>
float read(){unsigned w;if(scanf("%u",&w)!=1)throw 1;float f;memcpy(&f,&w,4);return f;}
int main(){try{unsigned count;if(scanf("%u",&count)!=1)return 2;std::vector<motorrock::meshpartsv149::Triangle> ts(count);for(auto& t:ts)for(auto& v:t){v.x=read();v.y=read();v.z=read();}auto r=motorrock::meshpartsv149::build(ts);for(unsigned i=0;i<count;++i)printf("%u %u\n",r.convex[i],r.flat[i]);return 0;}catch(int){return 2;}}
