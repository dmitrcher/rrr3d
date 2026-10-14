#include "NativeStaticWheelBoxQueryV107.h"
#include "NativeWheelPoseV54.h"
#include <iostream>
int main(){namespace w=motorrock::wheelposev54;namespace q=motorrock::staticwheelboxv107;unsigned tick;float x,y,z,qx,qy,qz,qw;const q::Box box{{{.5f,10,.15f}},{{8,0,10.15f}},{{{{1,0,0}},{{0,1,0}},{{0,0,1}}}}};while(std::cin>>tick>>x>>y>>z>>qx>>qy>>qz>>qw){std::cout<<tick;for(unsigned wheel=0;wheel<4;++wheel){auto ray=w::ray(w::pose(w::compose({{x,y,z,qx,qy,qz,qw}},{{wheel<2?1.1f:-1.1f,wheel%2==0?.75f:-.75f,0,.5f,.5f,.5f,.5f}})),.4f,.5f);std::cout<<' '<<q::candidate(box,ray.start,ray.end);}std::cout<<'\n';}}
