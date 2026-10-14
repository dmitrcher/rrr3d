#pragma once
#include <algorithm>
#include <utility>
#include <vector>
// Original wheel contact-manager processor uses dense slots: removing a
// manager moves the last registered wheel manager into its vacant position.
// Broadphase deletion is retired at the next processing step. Body contact
// managers have a separate processor; their pairs never occupy these slots.
namespace motorrock::contactmanagerorderv108 {
class List {
public:
    using Pair=std::pair<unsigned,unsigned>;
private:
    std::vector<Pair> active_,pending_;
    static Pair key(Pair p){return {std::min(p.first,p.second),std::max(p.first,p.second)};}
public:
    void clear(){active_.clear();pending_.clear();}
    void update(const std::vector<Pair>& added,const std::vector<Pair>& removed) {
        for(Pair pair:pending_) {auto it=std::find(active_.begin(),active_.end(),key(pair));if(it!=active_.end()){*it=active_.back();active_.pop_back();}}
        pending_.clear();
        for(Pair pair:added)if(std::find(active_.begin(),active_.end(),key(pair))==active_.end())active_.push_back(key(pair));
        pending_=removed;
    }
    unsigned order(unsigned a,unsigned b) const {auto it=std::find(active_.begin(),active_.end(),key({a,b}));return it==active_.end()?0:static_cast<unsigned>(it-active_.begin()+1);}
    const auto& entries() const {return active_;}
};
}
