#include "NativeWheelObjectLifetimeV104.h"
#include <iostream>
#include <cstring>
int main(){unsigned mode,present,word;while(std::cin>>mode>>present>>word){float travel;std::memcpy(&travel,&word,4);std::cout<<NativeWheelObjectLifetimeV104::resetActorRows(mode,present!=0)<<" "<<NativeWheelObjectLifetimeV104::springConfigured(travel)<<"\n";}}
