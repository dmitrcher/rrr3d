#include "NativeAngularIntegration.h"
#include <iostream>
#include <iomanip>
int main(){std::array<float,4>q;std::array<float,3>v;float dt;std::cout<<std::setprecision(9);while(std::cin>>q[0]>>q[1]>>q[2]>>q[3]>>v[0]>>v[1]>>v[2]>>dt){auto s=motorrock::nativeangular::integrate(q,v,dt);for(float x:s.rotation)std::cout<<x<<' ';std::cout<<s.length<<' '<<s.halfAngle<<' '<<s.cosine<<' '<<s.sineFactor<<'\n';}}
