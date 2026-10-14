#include "SourceMeshManifold.h"
#include <cstdio>
#include <cstring>
#include <map>
namespace sm=r3d::physics::sourcemesh;
float read(){unsigned w;if(scanf("%u",&w)!=1)throw 1;float f;memcpy(&f,&w,4);return f;}
int main(){try{
 unsigned count;if(scanf("%u",&count)!=1)return 2;using T=std::array<sm::PxVec3,3>;std::vector<T> ts(count);for(auto& t:ts)for(auto& v:t){v.x=read();v.y=read();v.z=read();}
 using V=std::array<float,3>;using E=std::array<V,2>;std::map<E,std::vector<std::pair<unsigned,unsigned>>> edges;
 for(unsigned i=0;i<count;++i)for(unsigned e=0;e<3;++e){auto a=ts[i][e],b=ts[i][(e+1)%3];E k{{{{a.x,a.y,a.z}},{{b.x,b.y,b.z}}}};if(k[1]<k[0])std::swap(k[0],k[1]);edges[k].push_back({i,e});}
 std::vector<unsigned> old(count,7),next(count,7);
 for(const auto& entry:edges){if(entry.second.size()!=2)continue;auto a=entry.second[0],b=entry.second[1];auto p=sm::trianglePlane(ts[a.first]),q=sm::trianglePlane(ts[b.first]);auto opposite=ts[b.first][(b.second+2)%3];bool flat=p.n.dot(q.n)>.999999f;float side=sm::nativeedge::add(sm::nativeedge::dot(p.n,opposite),p.d);
  if(flat){old[a.first]&=~(1u<<a.second);old[b.first]&=~(1u<<b.second);}
  if(flat||side>0){next[a.first]&=~(1u<<a.second);next[b.first]&=~(1u<<b.second);}
 }
 for(unsigned i=0;i<count;++i)printf("%u %u\n",old[i],next[i]);return 0;
}catch(int){return 2;}}
