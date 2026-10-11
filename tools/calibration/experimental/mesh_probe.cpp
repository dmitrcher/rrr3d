#include "SourceMeshManifold.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace r3d::physics::sourcemesh;
int main() {
    std::string surface; float x,y,z,roll,pitch,hx,hy,hz;
    while (std::cin>>surface>>x>>y>>z>>roll>>pitch>>hx>>hy>>hz) {
        Cm::Matrix34 box;
        box.p={x,y,z};
        box.m={{std::cos(pitch),0,-std::sin(pitch)},
               {std::sin(pitch)*std::sin(roll),std::cos(roll),std::cos(pitch)*std::sin(roll)},
               {std::sin(pitch)*std::cos(roll),-std::sin(roll),std::cos(pitch)*std::cos(roll)}};
        std::array<std::array<PxVec3,3>,2> triangles;
        if (surface=="floor") triangles={{{{{-500,-500,10},{500,-500,10},{500,500,10}}},{{{-500,-500,10},{500,500,10},{-500,500,10}}}}};
        else if(surface=="front") triangles={{{{{8,-10,10},{8,10,16},{8,10,10}}},{{{8,-10,10},{8,-10,16},{8,10,16}}}}};
        else if(surface=="back") triangles={{{{{8,-10,10},{8,10,10},{8,10,16}}},{{{8,-10,10},{8,10,16},{8,-10,16}}}}};
        else return 2;
        ContactBuffer out;
        for(unsigned i=0;i<2;++i)faceContacts(out,{hx,hy,hz},box,triangles[i],i);
        std::cout<<out.count<<'\n'<<std::setprecision(9);
        for(unsigned i=0;i<out.count;++i){const auto& c=out.contacts[i];std::cout<<c.point.x<<' '<<c.point.y<<' '<<c.point.z<<' '<<c.separation<<' '<<c.internalFaceIndex1<<'\n';}
    }
    return std::cin.eof()?0:2;
}
