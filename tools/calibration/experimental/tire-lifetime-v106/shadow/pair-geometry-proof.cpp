#include "NativeWheelPoseV54.h"
#include "NativeWheelPairLifetimeV106.h"
#include <iostream>
#include <algorithm>
int main(){namespace p=motorrock::wheelposev54;namespace l=motorrock::wheelpairv106;l::Lifetime history;unsigned step;float x,y,z,qx,qy,qz,qw;const l::Bounds floor{{{{-500,-500,10}},{{500,500,10}}}},bump{{{{7.5f,-10,10.15f-.15f}},{{8.5f,10,10.15f+.15f}}}};while(std::cin>>step>>x>>y>>z>>qx>>qy>>qz>>qw){std::vector<std::set<std::uint64_t>> pairs(4);for(unsigned w=0;w<4;++w){auto r=p::ray(p::pose(p::compose({{x,y,z,qx,qy,qz,qw}},{{w<2?1.1f:-1.1f,w%2==0?.75f:-.75f,0,.5f,.5f,.5f,.5f}})),.4f,.5f);l::Bounds b;for(unsigned a=0;a<3;++a){b[0][a]=std::min(r.start[a],r.end[a]);b[1][a]=std::max(r.start[a],r.end[a]);}if(l::overlap(b,floor))pairs[w].insert(1);if(l::overlap(b,bump))pairs[w].insert(7);}auto reset=history.update(pairs);for(unsigned w=0;w<4;++w)if(reset[w])std::cout<<step<<' '<<w<<'\n';}}
