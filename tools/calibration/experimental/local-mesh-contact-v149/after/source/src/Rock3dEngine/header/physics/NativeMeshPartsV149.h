#pragma once
#include "SourceMeshManifold.h"
#include <map>
#include <numeric>

namespace motorrock::meshpartsv149 {
namespace sm=r3d::physics::sourcemesh;
using Triangle=std::array<sm::PxVec3,3>;
struct Parts { std::vector<unsigned> convex,flat; };
struct Sets {
    std::vector<unsigned> parent;
    explicit Sets(unsigned count):parent(count){std::iota(parent.begin(),parent.end(),0);}
    unsigned root(unsigned x){while(parent[x]!=x){parent[x]=parent[parent[x]];x=parent[x];}return x;}
    void join(unsigned a,unsigned b){a=root(a);b=root(b);if(a>b)std::swap(a,b);parent[b]=a;}
};
inline Parts build(const std::vector<Triangle>& triangles) {
    using Vertex=std::array<float,3>;
    using Edge=std::array<Vertex,2>;
    struct Side { unsigned triangle,edge; };
    std::map<Edge,std::vector<Side>> edges;
    std::vector<sm::PxPlane> planes;
    for(unsigned i=0;i<triangles.size();++i){
        const auto& t=triangles[i];
        planes.push_back(sm::nativeplane::triangle(t[0],t[1],t[2]));
        for(unsigned e=0;e<3;++e){
            const auto a=t[e],b=t[(e+1)%3];
            Edge key{{Vertex{{a.x,a.y,a.z}},Vertex{{b.x,b.y,b.z}}}};
            if(key[1]<key[0])std::swap(key[0],key[1]);
            edges[key].push_back({i,e});
        }
    }
    Sets convex(static_cast<unsigned>(triangles.size())),flat(static_cast<unsigned>(triangles.size()));
    Parts out;
    for(const auto& entry:edges){
        if(entry.second.size()!=2)continue;
        const auto a=entry.second[0],b=entry.second[1];
        const auto n=planes[a.triangle].n,m=planes[b.triangle].n;
        const auto cross=n.cross(m);
        // Original Cooking RVA244f8..24510 and248aa..248c2 compare the
        // absolute atan2 angle to float32 0.01 at VA100515b4.
        const double sine=std::sqrt(double(cross.x)*cross.x+double(cross.y)*cross.y+double(cross.z)*cross.z);
        const double cosine=double(n.x)*m.x+double(n.y)*m.y+double(n.z)*m.z;
        const bool bent=std::abs(std::atan2(sine,cosine))>double(0.01f);
        const auto opposite=triangles[b.triangle][(b.edge+2)%3];
        const float distance=sm::nativeedge::add(sm::nativeedge::dot(n,opposite),planes[a.triangle].d);
        const bool concave=distance>0.0f && bent;
        if(!concave)convex.join(a.triangle,b.triangle);
        if(!bent)flat.join(a.triangle,b.triangle);
    }
    for(unsigned i=0;i<triangles.size();++i){out.convex.push_back(convex.root(i));out.flat.push_back(flat.root(i));}
    return out;
}
}
