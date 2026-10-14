#include "NativeWheelCallbackQueueV105.h"
#include <iostream>
#include <array>
#include <algorithm>
int main(){unsigned frames;while(std::cin>>frames){std::array<uint64_t,4> orders{};uint64_t count=0;for(unsigned f=0;f<frames;++f){std::array<unsigned,4> hit{},visits{{0,1,2,3}};for(unsigned i=0;i<4;++i){std::cin>>hit[i];NativeWheelCallbackQueueV105::enqueue(hit[i]!=0,orders[i],count);}std::stable_sort(visits.begin(),visits.end(),[&](unsigned a,unsigned b){return NativeWheelCallbackQueueV105::before(orders[a],orders[b]);});bool first=true;for(auto i:visits)if(hit[i]){if(!first)std::cout<<" ";std::cout<<i;first=false;}std::cout<<"\n";}}}
