#include "SourceMaterialFrameCandidate.h"
#include <iostream>
#include <iomanip>
namespace cf=motorrock::contactframe;
int main() {
 std::cout<<std::setprecision(9);cf::Vector v[3],axes[3];cf::Quaternion q;
 while(std::cin>>v[0][0]) {
  for(int j=1;j<3;++j)std::cin>>v[0][j];
  for(int i=1;i<3;++i)for(int j=0;j<3;++j)std::cin>>v[i][j];
  for(int i=0;i<3;++i)for(int j=0;j<3;++j)std::cin>>axes[i][j];
  for(int j=0;j<4;++j)std::cin>>q[j];if(!std::cin)return 2;
  cf::Vector e1,e2;for(int j=0;j<3;++j){e1[j]=v[1][j]-v[0][j];e2[j]=v[2][j]-v[0][j];}
  const auto r=motorrock::materialcandidate::compute(cf::columns(axes[0],axes[1],axes[2]),q,cf::unit(cf::cross(e1,e2)));
  for(int j=0;j<4;++j)std::cout<<r.seed[j]<<' ';
  for(int j=0;j<4;++j)std::cout<<r.callback.local0[j]<<' ';
  for(int j=0;j<4;++j)std::cout<<r.callback.local1[j]<<' ';
  for(int j=0;j<3;++j)std::cout<<r.normal[j]<<' ';
  std::cout<<'\n';
 }
}
