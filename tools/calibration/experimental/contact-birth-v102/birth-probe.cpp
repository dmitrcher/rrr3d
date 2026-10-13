#include <iostream>
#include <stdexcept>
#include "NativeContactOrderV79.h"
int main(){using namespace motorrock::contactorderv79;unsigned count;
while(std::cin>>count){std::vector<Shape> shapes(count);for(auto& s:shapes){std::cin>>s.actor>>s.wheel;for(unsigned a=0;a<3;++a)std::cin>>s.bounds[0][a]>>s.bounds[1][a];}
Scene scene;scene.useRecycledNodesV90(true);scene.initialize(shapes);
for(unsigned a=1;a<=count;++a)for(unsigned b=a+1;b<=count;++b)if(auto rank=scene.order(a,b))std::cout<<a<<' '<<b<<' '<<rank<<' ';std::cout<<'\n';}}
