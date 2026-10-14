#include "NativeContactManagerOrderV108.h"
#include <iostream>
int main(){namespace n=motorrock::contactmanagerorderv108;n::List list;unsigned frames;std::cin>>frames;for(unsigned f=0;f<frames;++f){unsigned tick,added,removed,queries;std::cin>>tick>>added;std::vector<n::List::Pair> a(added);for(auto& p:a)std::cin>>p.first>>p.second;std::cin>>removed;std::vector<n::List::Pair> b(removed);for(auto& p:b)std::cin>>p.first>>p.second;list.update(a,b);std::cin>>queries;std::cout<<tick;for(unsigned i=0;i<queries;++i){unsigned x,y;std::cin>>x>>y;std::cout<<' '<<list.order(x,y);}std::cout<<'\n';}}
