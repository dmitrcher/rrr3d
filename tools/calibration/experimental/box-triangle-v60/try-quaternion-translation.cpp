#include "SourceMeshManifold.h"
#include "DerivedContactErrorFrame.h"
#include <cstring>
#include <cstdio>
float f(unsigned w){float v;std::memcpy(&v,&w,4);return v;} unsigned word(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
int main(){using namespace r3d::physics::sourcemesh;
Cm::Matrix34 box{PxVec3{f(1065353216u),f(3024306916u),f(3040735185u)},PxVec3{f(876740226u),f(1065353193u),f(3134885644u)},PxVec3{f(893256855u),f(987401996u),f(1065353193u)},PxVec3{f(1090658848u),f(1073748441u),f(1093246219u)}};
PxVec3 half{1.5f,.8f,.3f};std::array<PxVec3,3> triangle{{{8,-10,10},{8,10,10},{8,10,16}}};const auto plane=trianglePlane(triangle);Cm::Matrix34 world;auto toTriangle=box;auto toBox=box.getInverseRT();const PxU8 bi[4]={0,1,2,3},ti[3]={0,1,2};
motorrock::contactframe::Quaternion q{{-0.000833735568f,3.53860401e-07f,-9.06305999e-08f,1.0f}};
const auto tr=motorrock::derivederror::rotate(q,{{box.p.x,box.p.y,box.p.z}},true);toBox.p={-tr[0],-tr[1],-tr[2]};
for(auto v:triangle) { auto a=toBox.transform(v);printf("quaternion translation local %.9g %.9g %.9g\n",a.x,a.y,a.z); }
}
