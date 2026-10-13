#include "NativeTireV55.h"
#include <cstring>
#include <cstdint>
#include <iostream>
#include <stdexcept>
static float input(){std::uint32_t w=0;std::cin>>w;float v;std::memcpy(&v,&w,4);return v;}
static std::uint32_t bits(float v){std::uint32_t w;std::memcpy(&w,&v,4);return w;}
int main(){try{unsigned count=0;std::cin>>count;for(unsigned i=0;i<count;++i){motorrock::tirev55::Vector p,old,lat,lng;for(auto* v:{&p,&old,&lat,&lng})for(float& x:*v)x=input();float skin=input(),axle=input();unsigned expectedUpdate,lateral,longitudinal;std::cin>>expectedUpdate>>lateral>>longitudinal;auto r=motorrock::tirev55::anchor(p,old,lat,lng,skin,axle);if(r.update!=bool(expectedUpdate)||bits(r.lateral_error)!=lateral||bits(r.longitudinal_error)!=longitudinal)throw std::runtime_error("Native same-input anchor result mismatch");}if(!std::cin)throw std::runtime_error("Incomplete fixture");std::cout<<count<<" Native same-input anchors exact\n";}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
