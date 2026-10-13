#include "NativeContactNodesV90.h"
#include <iostream>
#include <stdexcept>
using Pool=motorrock::contactnodesv90::Pool;
int main(int argc,char**) {
 try {
  unsigned frames=0;std::cin>>frames;Pool pool(argc==1);unsigned checks=0;
  for(unsigned frame=0;frame<frames;++frame) {
   unsigned count=0;std::cin>>count;
   for(unsigned i=0;i<count;++i) {
    unsigned a,b,head,nodes;std::cin>>a>>b>>head>>nodes;
    if(pool.freeHead()!=head || pool.nodeCount()!=nodes)throw std::runtime_error("Native pre-insert allocator state mismatch");
    ++checks;pool.insert(a,b);
   }
   std::cin>>count;std::vector<Pool::Pair> expected;
   for(unsigned i=0;i<count;++i){unsigned a,b;std::cin>>a>>b;expected.emplace_back(a,b);}
   std::cin>>count;for(unsigned i=0;i<count;++i){unsigned a,b;std::cin>>a>>b;pool.remove(a,b);}
   auto result=pool.publish();if(result!=expected)throw std::runtime_error("Native published pair order mismatch");
   ++checks;
  }
  if(!std::cin)throw std::runtime_error("Malformed fixture");
  std::cout<<checks<<" Native allocator/publication checkpoints exact\n";
 }catch(const std::exception &e){std::cerr<<e.what()<<"\n";return 1;}
}
