#include "physics/NativeGameAngularV113.h"
#include <iostream>
#include <cstring>
namespace G=motorrock::gameangularv113;namespace P=motorrock::nativepublicangular;
static float value(unsigned u){float v;std::memcpy(&v,&u,4);return v;}
static unsigned word(float v){unsigned u;std::memcpy(&u,&v,4);return u;}
template<class T>static void emit(const T&a){std::cout<<'[';for(unsigned i=0;i<a.size();++i){if(i)std::cout<<',';std::cout<<word(a[i]);}std::cout<<']';}
int main(){unsigned c,u;while(std::cin>>c){G::Quaternion q;G::Vector inv,omega,damp;for(auto&v:q){std::cin>>u;v=value(u);}for(auto&v:inv){std::cin>>u;v=value(u);}for(auto&v:omega){std::cin>>u;v=value(u);}for(auto&v:damp){std::cin>>u;v=value(u);}unsigned roll,pitch,contact;std::cin>>roll>>pitch>>contact;auto m=G::stabilize(q,inv,omega,damp,value(roll),value(pitch),contact!=0);if(value(roll)>0||value(pitch)>0)q=G::clampOrientation(q,value(roll),value(pitch));auto v=G::velocity(G::inverseTensor(q,inv),m);std::cout<<"{\"case\":"<<c<<",\"momentum\":";emit(m);std::cout<<",\"omega_after\":";emit(v);std::cout<<",\"rotation_after\":";emit(q);std::cout<<"}\n";}return 0;}
