#include "NativeRelativeContactV147.h"
#include <cstdio>
#include <initializer_list>
#include <cstring>
namespace rc=motorrock::relativecontactv147;
float read(){unsigned u;if(scanf("%u",&u)!=1)throw 1;float f;memcpy(&f,&u,4);return f;}
unsigned word(float f){unsigned u;memcpy(&u,&f,4);return u;}
int main(){try{rc::Pose a,b;for(auto p:{&a,&b}){for(unsigned i=0;i<3;++i)p->position[i]=read();for(unsigned i=0;i<4;++i)p->quaternion[i]=read();}for(auto pair:{0,1}){auto t=pair?rc::relative(b,a):rc::relative(a,b);for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)printf("%u ",word(t.rotation.a[r][c]));for(unsigned i=0;i<3;++i)printf("%u%c",word(t.position[i]),i==2?'\n':' ');}return 0;}catch(int){return 2;}}
