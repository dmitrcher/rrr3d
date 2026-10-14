#include "NativeGameAngularV113.h"
#include <iostream>
#include <cstring>
#include <cstdint>
namespace P=motorrock::nativepublicangular;
float f(uint32_t word){float v;std::memcpy(&v,&word,4);return v;}
void emit(float v){uint32_t word;std::memcpy(&word,&v,4);std::cout<<word<<' ';}
int main(){uint32_t words[10];while(std::cin>>words[0]){
 for(unsigned i=1;i<10;++i)std::cin>>words[i];
 std::array<float,4>q;std::array<float,3>d,v;
 for(unsigned i=0;i<4;++i)q[i]=f(words[i]);for(unsigned i=0;i<3;++i){d[i]=f(words[4+i]);v[i]=f(words[7+i]);}
 auto tensor=motorrock::gameangularv113::inverseTensor(q,d);
 for(auto row:tensor)for(auto x:row)emit(x);
 for(float x:motorrock::gameangularv113::velocity(tensor,v))emit(x);
 std::cout<<'\n';
}}
