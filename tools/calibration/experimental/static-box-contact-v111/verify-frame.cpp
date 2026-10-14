#include "NativeMaterialFrameV36.h"
#include "NativePhysicalPoseV42.h"
#include <cstring>
#include <iostream>
float f(unsigned w){float v;std::memcpy(&v,&w,4);return v;}unsigned bits(float v){unsigned w;std::memcpy(&w,&v,4);return w;}
int main(){unsigned a[7];while(std::cin>>a[0]){for(int i=1;i<7;++i)std::cin>>a[i];namespace cf=motorrock::contactframe;namespace fv=motorrock::framev36;auto q=cf::Quaternion{{f(a[0]),f(a[1]),f(a[2]),f(a[3])}};auto n=cf::Vector{{f(a[4]),f(a[5]),f(a[6])}};for(auto matrix:{cf::decode(q),motorrock::physicalposev42::rotation(q)}){auto raw=fv::rawWorld(n);auto materialNormal=motorrock::derivederror::rotate(raw,{{1,0,0}},false);auto material=fv::encode(fv::basis(materialNormal,cf::column(matrix,2)));for(int i=0;i<3;++i)material[i]=-material[i];for(int i=0;i<4;++i)std::cout<<bits(material[i])<<' ';for(auto v:{fv::solverTangent1(material),fv::solverTangent2(material)})for(int i=0;i<3;++i)std::cout<<bits(v[i])<<' ';std::cout<<'\n';}}}
