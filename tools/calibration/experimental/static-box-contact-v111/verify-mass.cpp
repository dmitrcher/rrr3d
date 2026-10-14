#include "NativePhysicalPoseV42.h"
#include "NativeContactCache.h"
#include "NativeInertiaRotation.h"
#include "NativePairPreparationV74.h"
#include <iostream>
#include <cstring>
float f(unsigned w){float v;std::memcpy(&v,&w,4);return v;}unsigned bits(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
int main(){unsigned a[32];while(std::cin>>a[0]){for(int i=1;i<32;++i)std::cin>>a[i];namespace cf=motorrock::contactframe;namespace nc=motorrock::nativecache;namespace P=motorrock_native_pair_v73;auto q=cf::Quaternion{{f(a[0]),f(a[1]),f(a[2]),f(a[3])}};nc::Pose p{motorrock::physicalposev42::rotation(q),{{f(a[4]),f(a[5]),f(a[6])}}};cf::Matrix local,jolt;for(int c=0;c<3;++c)for(int r=0;r<3;++r){local.a[r][c]=f(a[10+c*3+r]);jolt.a[r][c]=f(a[19+c*3+r]);}auto native=motorrock::inertiav37::solver(q,local);auto point=cf::Vector{{f(a[7]),f(a[8]),f(a[9])}};auto plain=nc::subtract(point,p.position),lever=nc::solverLever(p,nc::local(p,point));for(int c=0;c<3;++c)for(int r=0;r<3;++r)std::cout<<bits(native.a[r][c])<<' ';for(auto v:{plain,lever})for(int i=0;i<3;++i)std::cout<<bits(v[i])<<' ';for(auto tensor:{jolt,native})for(auto arm:{plain,lever}){std::array<P::Body,2> bodies{};bodies[1].lever={{arm[0],arm[1],arm[2]}};bodies[1].inverse_mass=f(a[31]);for(int c=0;c<3;++c)for(int r=0;r<3;++r)bodies[1].inverse_inertia[c*3+r]=tensor.a[r][c];std::cout<<bits(motorrock_native_pair_v74::effectiveMass(bodies,{{f(a[28]),f(a[29]),f(a[30])}},0))<<' ';}std::cout<<'\n';}}
