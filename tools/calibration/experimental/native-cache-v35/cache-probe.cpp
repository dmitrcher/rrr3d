#include "NativeContactCache.h"
#include <iostream>
#include <iomanip>
int main(){motorrock::contactframe::Quaternion q;motorrock::contactframe::Vector d;while(std::cin>>q[0]>>q[1]>>q[2]>>q[3]>>d[0]>>d[1]>>d[2]){const auto v=motorrock::nativecache::transverse(q,d);std::cout<<std::setprecision(9)<<v[1]<<" "<<v[2]<<"\n";}return 0;}
