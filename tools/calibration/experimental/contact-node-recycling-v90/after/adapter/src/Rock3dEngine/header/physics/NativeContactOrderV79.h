#pragma once
#include "NativeEndpointSortV78.h"
#include "NativeContactNodesV90.h"
#include <array>
#include <cstring>
#include <map>
#include <set>

// Default-off research: source broadphase ordering for vehicle boxes and
// suspension line bounds. This derives order from geometry and pair lifetime;
// no source samples, wheel permutations or vehicle parameter changes.
namespace motorrock::contactorderv79 {
using Endpoint = NativeEndpointSortV78::Endpoint;
using Bounds = std::array<std::array<float,3>,2>;
struct Shape { Bounds bounds{}; unsigned actor=0; bool wheel=false, enabled=true; };
inline std::uint32_t encode(float value) {
    std::uint32_t word; std::memcpy(&word,&value,sizeof(word));
    return word & 0x80000000u ? ~word : word | 0x80000000u;
}
class Scene {
    struct Node { unsigned owner=0; std::array<unsigned,4> neighbor{}; std::array<bool,4> active{}; };
    struct Pair { unsigned slot=0; std::uint64_t serial=0; };
    std::vector<Shape> shapes;
    std::array<std::vector<Endpoint>,3> axes;
    std::vector<Node> nodes{Node{}};
    std::map<std::pair<unsigned,unsigned>,Pair> pairs;
    std::set<unsigned> created;
    std::uint64_t counter=0;
    bool recycleNodesV90_=false;
    motorrock::contactnodesv90::Pool poolV90_;
    static unsigned owner(const Endpoint &e) { return e.ownerAndType & 0x7fffffffu; }
    static bool minimum(const Endpoint &e) { return e.ownerAndType & 0x80000000u; }
    static std::pair<unsigned,unsigned> key(unsigned a,unsigned b) { return {std::min(a,b),std::max(a,b)}; }
    bool allowed(unsigned a,unsigned b) const {
        return a && b && a!=b && shapes[a-1].enabled && shapes[b-1].enabled &&
            shapes[a-1].actor!=shapes[b-1].actor && !(shapes[a-1].wheel && shapes[b-1].wheel);
    }
    unsigned position(unsigned axis,unsigned id,bool min) const {
        const auto &v=axes[axis];
        for(unsigned i=0;i<v.size();++i) if(owner(v[i])==id && minimum(v[i])==min) return i;
        return static_cast<unsigned>(v.size());
    }
    bool overlap(unsigned a,unsigned b,unsigned except=3) const {
        for(unsigned axis=0;axis<3;++axis) if(axis!=except)
            if(position(axis,a,true)>position(axis,b,false) ||
               position(axis,b,true)>position(axis,a,false)) return false;
        return true;
    }
    void add(unsigned a,unsigned b) {
        if(!allowed(a,b) || pairs.count(key(a,b))) return;
        if(recycleNodesV90_) {poolV90_.insert(a,b);pairs.emplace(key(a,b),Pair{poolV90_.slot(a,b),0});return;}
        unsigned counts[2]={0,0},available[2]={0,0}; const unsigned owners[2]={a,b};
        // Native walks newest node to oldest, retaining the last node with a
        // free slot. Prefer the first argument's free node, then the second.
        for(unsigned i=1;i<nodes.size();++i) for(unsigned side=0;side<2;++side)
            if(nodes[i].owner==owners[side]) {
                ++counts[side];
                if(!available[side] && std::find(nodes[i].active.begin(),nodes[i].active.end(),false)!=nodes[i].active.end()) available[side]=i;
            }
        unsigned side=available[0]?0:available[1]?1:counts[0]<=counts[1]?0:1;
        unsigned node=available[side];
        if(!node) { node=static_cast<unsigned>(nodes.size()); nodes.push_back(Node{}); nodes.back().owner=owners[side]; }
        unsigned slot=0;while(nodes[node].active[slot])++slot;
        nodes[node].neighbor[slot]=owners[1-side];nodes[node].active[slot]=true;
        const unsigned bit=node*4+slot;
        pairs.emplace(key(a,b),Pair{bit,0});created.insert(bit);
    }
    void remove(unsigned a,unsigned b) {
        auto it=pairs.find(key(a,b));if(it==pairs.end())return;
        if(recycleNodesV90_) {poolV90_.remove(a,b);pairs.erase(it);return;}
        unsigned bit=it->second.slot;nodes[bit/4].active[bit%4]=false;
        created.erase(bit);pairs.erase(it);
    }
    void cross(unsigned id,bool min,const Endpoint &other,unsigned axis,bool right) {
        if(minimum(other)==min || !allowed(id,owner(other)))return;
        const bool entering=right?!min:min;
        if(entering) { if(overlap(id,owner(other),axis))add(id,owner(other)); }
        else remove(id,owner(other));
    }
    void move(unsigned id,unsigned axis,bool min,std::uint32_t coordinate) {
        auto &v=axes[axis];unsigned p=position(axis,id,min);
        const std::uint32_t old=v[p].coordinate; if(old==coordinate)return;
        Endpoint e=v[p];e.coordinate=coordinate;
        if(coordinate<old) {
            while(p && (min?v[p-1].coordinate>=coordinate:v[p-1].coordinate>coordinate)) {
                Endpoint other=v[p-1];v[p]=other;--p;v[p]=e;cross(id,min,other,axis,false);
            }
        } else {
            while(p+1<v.size() && (min?v[p+1].coordinate<coordinate:v[p+1].coordinate<=coordinate)) {
                Endpoint other=v[p+1];v[p]=other;++p;v[p]=e;cross(id,min,other,axis,true);
            }
        }
        v[p]=e;
    }
    void publish() {
        if(recycleNodesV90_) {for(const auto &entry:poolV90_.publish()){auto it=pairs.find(key(entry.first,entry.second));if(it!=pairs.end() && !it->second.serial)it->second.serial=++counter;}return;}
        for(unsigned bit:created) {
            const auto &node=nodes[bit/4];auto it=pairs.find(key(node.owner,node.neighbor[bit%4]));
            if(it!=pairs.end() && it->second.slot==bit && !it->second.serial)it->second.serial=++counter;
        }
        created.clear();
    }
public:
    bool initialized() const { return !shapes.empty(); }
    void useRecycledNodesV90(bool value) {if(value!=recycleNodesV90_){clear();recycleNodesV90_=value;}}
    void clear() { poolV90_=motorrock::contactnodesv90::Pool{};shapes.clear();for(auto &a:axes)a.clear();nodes.assign(1,Node{});pairs.clear();created.clear();counter=0; }
    void initialize(const std::vector<Shape> &input) {
        clear();shapes=input;
        for(unsigned axis=0;axis<3;++axis) {
            auto &v=axes[axis];
            for(unsigned i=0;i<shapes.size();++i) {
                v.push_back({encode(shapes[i].bounds[0][axis]),(i+1)|0x80000000u});
                v.push_back({encode(shapes[i].bounds[1][axis]),i+1});
            }
            NativeEndpointSortV78::sort(v);std::reverse(v.begin(),v.end());
        }
        std::set<unsigned> open;
        for(const auto &e:axes[2]) {
            const unsigned id=owner(e);
            if(minimum(e)) { for(unsigned other:open) if(allowed(id,other) && overlap(id,other))add(id,other);open.insert(id); }
            else open.erase(id);
        }
        publish();
    }
    void update(const std::vector<Shape> &input) {
        if(!initialized() || input.size()!=shapes.size()) { initialize(input);return; }
        for(unsigned i=0;i<input.size();++i) {
            shapes[i].enabled=input[i].enabled;
            for(unsigned axis=0;axis<3;++axis) {
                const auto min=encode(input[i].bounds[0][axis]),max=encode(input[i].bounds[1][axis]);
                const bool left=min<axes[axis][position(axis,i+1,true)].coordinate;
                if(left) { move(i+1,axis,true,min);move(i+1,axis,false,max); }
                else { move(i+1,axis,false,max);move(i+1,axis,true,min); }
            }
            shapes[i].bounds=input[i].bounds;
        }
        publish();
    }
    std::uint64_t order(unsigned a,unsigned b) const { auto it=pairs.find(key(a,b));return it==pairs.end()?0:it->second.serial; }
    const auto &endpoints(unsigned axis) const { return axes[axis]; }
};
}
