#include "physics/NativeContactOrderV79.h"
#include <iostream>
int main(){unsigned n;std::cin>>n;std::vector<motorrock::contactorderv79::Shape> shapes(n);for(auto& s:shapes){std::cin>>s.actor>>s.wheel;for(auto& b:s.bounds)for(float& x:b){unsigned u;std::cin>>u;std::memcpy(&x,&u,4);}}motorrock::contactorderv79::Scene scene;scene.useRecycledNodesV90(true);scene.trackPairEventsV108(true);scene.update(shapes);for(auto p:scene.createdPairsV108())std::cout<<p.first<<" "<<p.second<<"\n";}
