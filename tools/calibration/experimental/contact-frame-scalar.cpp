#include "ContactFrameScalar.h"
#include <iostream>
#include <iomanip>
namespace cf=motorrock::contactframe;
int main() {
 std::cout<<std::setprecision(9);int first;
 while(std::cin>>first) {
  cf::Quaternion q;cf::Matrix a,b;cf::Vector n;
  for(int i=0;i<4;++i)std::cin>>q[i];
  for(int i=0;i<3;++i)for(int j=0;j<3;++j)std::cin>>a.a[i][j];
  for(int i=0;i<3;++i)for(int j=0;j<3;++j)std::cin>>b.a[i][j];
  for(int i=0;i<3;++i)std::cin>>n[i];if(!std::cin)return 2;
  const cf::Result r=cf::modify(q,a,b,n,first!=0);if(!r.modified)return 3;
  for(int j=0;j<3;++j)for(int i=0;i<3;++i)std::cout<<r.before.a[i][j]<<' ';
  for(int j=0;j<3;++j)for(int i=0;i<3;++i)std::cout<<r.after.a[i][j]<<' ';
  for(int i=0;i<4;++i)std::cout<<r.local0[i]<<' ';
  for(int i=0;i<4;++i)std::cout<<r.local1[i]<<' ';
  for(int i=0;i<3;++i)std::cout<<r.decodedLocalNormal[i]<<' ';
  std::cout<<'\n';
 }
}
