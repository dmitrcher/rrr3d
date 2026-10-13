#include "NativeWheelBoxQueryV80.h"
#include <iostream>
#include <cstring>
namespace q = motorrock::wheelboxqueryv80;
float value(unsigned v) {float f;std::memcpy(&f,&v,4);return f;}
unsigned bits(float f) {unsigned v;std::memcpy(&v,&f,4);return v;}
int main() {
 unsigned count;std::cin>>count;
 for(unsigned n=0;n<count;++n) {
  unsigned w[22];for(auto &v:w)std::cin>>v;
  q::Box b;
  for(unsigned i=0;i<3;++i)for(unsigned j=0;j<3;++j)b.columns[i][j]=value(w[j*3+i]);
  for(unsigned i=0;i<3;++i){b.position[i]=value(w[9+i]);b.half[i]=value(w[12+i]);}
  q::Vector p,d;for(unsigned i=0;i<3;++i){p[i]=value(w[15+i]);d[i]=value(w[18+i]);}
  auto r=q::cast(b,p,d,value(w[21]));
  std::cout<<r.hit;
  for(auto v:{r.localStart,r.localDirection,r.localPoint})for(float f:v)std::cout<<' '<<bits(f);
  std::cout<<' '<<bits(r.distance);
  for(auto v:{r.point,r.normal})for(float f:v)std::cout<<' '<<bits(f);
  std::cout<<'\n';
 }
}
