#include "NativeGameAngularV113.h"
#include <iostream>
#include <cstring>
#include <cstdint>
namespace C=motorrock::contactframe;
float f(uint32_t w){float v;std::memcpy(&v,&w,4);return v;}
void emit(float v){uint32_t w;std::memcpy(&w,&v,4);std::cout<<w<<' ';}
int main(){uint32_t words[4];while(std::cin>>words[0]){
for(int i=1;i<4;++i)std::cin>>words[i];std::array<float,4>q;for(int i=0;i<4;++i)q[i]=f(words[i]);
const double x=q[0],y=q[1],z=q[2],w=q[3],n=x*x+y*y+z*z+w*w,s=n>0?2/n:0,xs=x*s,ys=y*s,zs=z*s;
const float r00=C::store(1-(y*ys+z*zs)),r10=C::store(x*ys+w*zs),r20=C::store(x*zs-w*ys),r21=C::store(y*zs+w*xs),r22=C::store(1-(x*xs+y*ys));
const float cy=C::store(std::sqrt(double(C::store(double(r00)*r00+double(r10)*r10))));
const float ax=C::store(std::atan2(double(r21),double(r22))),ay=C::store(std::atan2(-double(r20),double(cy))),az=C::store(std::atan2(double(r10),double(r00)));
for(float a:{ax,ay,az})emit(a);
const double ci=std::cos(double(ax)*.5),cj=std::cos(double(ay)*.5),ch=std::cos(double(az)*.5),si=std::sin(double(ax)*.5),sj=std::sin(double(ay)*.5),sh=std::sin(double(az)*.5),cc=ci*ch,cs=ci*sh,sc=si*ch,ss=si*sh;
for(float a:motorrock::gameangularv113::clampOrientation(q,0,0))emit(a);
std::cout<<'\n';}}
