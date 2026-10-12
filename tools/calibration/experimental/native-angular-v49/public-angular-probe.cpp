#include "NativePublicAngular.h"
#include <iostream>
#include <iomanip>
int main(){std::array<float,4>q;std::array<float,3>d,v;std::cout<<std::setprecision(9);while(std::cin>>q[0]>>q[1]>>q[2]>>q[3]>>d[0]>>d[1]>>d[2]>>v[0]>>v[1]>>v[2]){auto r=motorrock::nativepublicangular::orientation(q);auto i=motorrock::nativepublicangular::inertia(r,d);auto m=motorrock::nativepublicangular::momentum(i,v);for(auto a:r)for(float x:a)std::cout<<x<<' ';for(auto a:i)for(float x:a)std::cout<<x<<' ';for(float x:m)std::cout<<x<<' ';std::cout<<'\n';}}
