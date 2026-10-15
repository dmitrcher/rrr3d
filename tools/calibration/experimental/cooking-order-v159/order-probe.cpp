#include "NativeCookedOrderV159.h"
#include <cstdint>
#include <cstring>
#include <iostream>
int main(){unsigned n;if(!(std::cin>>n))return 1;std::vector<motorrock::cookedorderv159::Triangle> t(n);for(auto& a:t)for(auto& v:a)for(float& f:v){std::uint32_t u;if(!(std::cin>>u))return 2;std::memcpy(&f,&u,4);}for(auto i:motorrock::cookedorderv159::build(t))std::cout<<i<<' ';std::cout<<'\n';}
