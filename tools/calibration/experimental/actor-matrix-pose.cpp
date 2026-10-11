#include <iostream>
#include <iomanip>
#include "ContactFrameScalar.h"
using namespace motorrock::contactframe;
int main(){Quaternion q;std::cout<<std::setprecision(9);while(std::cin>>q[0]>>q[1]>>q[2]>>q[3]){Matrix m=decode(q);Quaternion t=encode(m);Matrix after=decode(t);for(int c=0;c<3;++c)for(int r=0;r<3;++r)std::cout<<m.a[r][c]<<' ';for(int i=0;i<4;++i)std::cout<<t[i]<<' ';for(int c=0;c<3;++c)for(int r=0;r<3;++r)std::cout<<after.a[r][c]<<' ';std::cout<<'\n';}}
