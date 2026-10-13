#include "NativeWheelMotorV92.h"
#include <iostream>
#include <cstring>
#include <cstdint>
#include <stdexcept>
static float input(){std::uint32_t w=0;std::cin>>w;float v;std::memcpy(&v,&w,4);return v;}
static std::uint32_t bits(float v){std::uint32_t w;std::memcpy(&w,&v,4);return w;}
int main(int argc,char**){try{unsigned count=0,changed=0;std::cin>>count;for(unsigned i=0;i<count;++i){const float axle=input(),torque=input(),delta=input(),inverse=input();unsigned expected;std::cin>>expected;float result=motorrock::wheelmotorv92::apply(axle,torque,delta,inverse);if(argc>1){volatile float inertia=1.0f/inverse;volatile float product=torque*delta;volatile float increment=product/inertia;volatile float sum=axle+increment;result=sum;}if(bits(result)!=expected){++changed;if(argc==1)throw std::runtime_error("Native same-input motor result mismatch");}}if(!std::cin)throw std::runtime_error("Incomplete fixture");if(argc>1){if(!changed)throw std::runtime_error("Division control unexpectedly exact");std::cout<<changed<<" old-division word differences\n";}else std::cout<<count<<" Native motor results exact\n";}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
