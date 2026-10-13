#include <cstdio>
#include <cstdlib>
#include "NativeContactCache.h"
int main(int argc,char**) {
    namespace nc=motorrock::nativecache; namespace cf=motorrock::contactframe;
    float input[39];
    while(std::scanf("%f",input)==1) {
        for(unsigned i=1;i<39;++i)if(std::scanf("%f",input+i)!=1)return 2;
        nc::Pose actor,ground{cf::Matrix{{{1,0,0},{0,1,0},{0,0,1}}},{{0,0,0}}};
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)actor.rotation.a[r][c]=input[3*c+r];
        for(unsigned i=0;i<3;++i)actor.position[i]=input[9+i];
        std::vector<cf::Vector> points;
        for(unsigned i=0;i<4;++i)points.push_back({{input[12+3*i],input[13+3*i],input[14+3*i]}});
        // The remaining input fields carry independently captured normal and
        // static pose; static pose is identity in this measured Native fixture.
        cf::Vector normal={{input[24],input[25],input[26]}};
        for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)ground.rotation.a[r][c]=input[27+3*c+r];
        for(unsigned i=0;i<3;++i)ground.position[i]=input[36+i];
        nc::Cache cache;std::vector<cf::Vector> levers;bool ordered=argc==1;
        if(!ordered)for(unsigned i=0;i<3;++i)normal[i]=-normal[i];
        cache.prepare(ordered?ground:actor,ordered?actor:ground,points,normal,.01F,{{0,0,0,1}},&levers,false,ordered?1U:0U);
        for(const auto& p:cache.points) {
            for(unsigned i=0;i<3;++i)std::printf("%.9g ",(ordered?p.on0:p.on1)[i]);
            for(unsigned i=0;i<3;++i)std::printf("%.9g ",(ordered?p.on1:p.on0)[i]);
        }
        for(const auto& lever:levers)for(unsigned i=0;i<3;++i)std::printf("%.9g ",lever[i]);
        std::puts("");
    }
}
