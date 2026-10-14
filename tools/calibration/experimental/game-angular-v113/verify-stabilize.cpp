#include "NativeGameAngularV113.h"
#include "ContactFrameScalar.h"
#include <iostream>
#include <cstdint>
#include <cstring>
namespace P=motorrock::nativepublicangular;namespace C=motorrock::contactframe;
float f(uint32_t w){float v;std::memcpy(&v,&w,4);return v;}
void emit(float v){uint32_t w;std::memcpy(&w,&v,4);std::cout<<w<<' ';}
int main(){uint32_t w[13];double roll,pitch;
 while(std::cin>>w[0]){
  for(unsigned i=1;i<13;++i)std::cin>>w[i];std::cin>>roll>>pitch;
  std::array<float,4>q;std::array<float,3>d,omega,damp;
  for(unsigned i=0;i<4;++i)q[i]=f(w[i]);for(unsigned i=0;i<3;++i){d[i]=C::store(1.0/f(w[4+i]));omega[i]=f(w[7+i]);damp[i]=f(w[10+i]);}
  std::array<float,3> inverse;for(unsigned i=0;i<3;++i)inverse[i]=f(w[4+i]);
  for(float x:motorrock::gameangularv113::stabilize(q,inverse,omega,damp,roll,pitch,false))emit(x);
  std::cout<<'\n';
 }
}
