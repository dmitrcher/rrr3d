#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <set>
#include <utility>
#include <vector>

// Research reconstruction of original broadphase adjacency-node lifetime.
// Inputs are shape IDs and pair events. There is no clock, vehicle or case input.
namespace motorrock::contactnodesv90 {
class Pool {
public:
    struct Node {
        unsigned owner=0, next=0;
        std::array<unsigned,4> neighbor{};
        std::array<bool,4> active{};
    };
    using Pair=std::pair<unsigned,unsigned>;
private:
    std::vector<Node> nodes_{Node{}};
    std::map<unsigned,unsigned> heads_;
    std::map<Pair,unsigned> slots_;
    std::set<unsigned> created_, removed_;
    unsigned freeHead_=0;
    bool recycle_;
    static Pair key(unsigned a,unsigned b) { return {std::min(a,b),std::max(a,b)}; }
public:
    explicit Pool(bool recycle=true):recycle_(recycle) {}
    void insert(unsigned a,unsigned b) {
        if(a==b || !a || !b || slots_.count(key(a,b)))return;
        const unsigned owners[2]={a,b};unsigned counts[2]={0,0},available[2]={0,0};
        for(unsigned side=0;side<2;++side)
            for(unsigned node=heads_[owners[side]];node;node=nodes_[node].next) {
                ++counts[side];
                if(std::find(nodes_[node].active.begin(),nodes_[node].active.end(),false)!=nodes_[node].active.end())available[side]=node;
            }
        unsigned side=available[0]?0:available[1]?1:counts[0]<=counts[1]?0:1;
        unsigned node=available[side];
        if(!node) {
            if(recycle_ && freeHead_) {node=freeHead_;freeHead_=nodes_[node].next;nodes_[node]=Node{};}
            else {node=static_cast<unsigned>(nodes_.size());nodes_.push_back(Node{});}
            nodes_[node].owner=owners[side];nodes_[node].next=heads_[owners[side]];heads_[owners[side]]=node;
        }
        unsigned slot=0;while(nodes_[node].active[slot])++slot;
        nodes_[node].neighbor[slot]=owners[1-side];nodes_[node].active[slot]=true;
        unsigned bit=node*4+slot;slots_.emplace(key(a,b),bit);removed_.erase(bit);created_.insert(bit);
    }
    void remove(unsigned a,unsigned b) {
        auto it=slots_.find(key(a,b));if(it==slots_.end())return;
        unsigned bit=it->second;nodes_[bit/4].active[bit%4]=false;
        created_.erase(bit);removed_.insert(bit);slots_.erase(it);
    }
    std::vector<Pair> publish() {
        std::vector<Pair> result;
        for(unsigned bit:created_) {const auto &node=nodes_[bit/4];if(node.active[bit%4])result.emplace_back(node.owner,node.neighbor[bit%4]);}
        created_.clear();
        // Original removed-bitmap traversal is ascending; empty owner nodes
        // are unlinked and pushed on the free-head chain, after publication.
        std::set<unsigned> empty;
        if(recycle_)for(unsigned bit:removed_)
            if(std::none_of(nodes_[bit/4].active.begin(),nodes_[bit/4].active.end(),[](bool v){return v;}))empty.insert(bit/4);
        for(unsigned node:empty) {
            unsigned *link=&heads_[nodes_[node].owner];
            while(*link && *link!=node)link=&nodes_[*link].next;
            if(*link==node)*link=nodes_[node].next;
            nodes_[node].next=freeHead_;freeHead_=node;
        }
        removed_.clear();return result;
    }
    unsigned slot(unsigned a,unsigned b) const {return slots_.at(key(a,b));}
    unsigned freeHead() const { return freeHead_; }
    unsigned nodeCount() const { return static_cast<unsigned>(nodes_.size()); }
    const Node& node(unsigned index) const { return nodes_.at(index); }
};
}
