#pragma once
#include <algorithm>
#include <map>
#include <utility>
#include <vector>
// A bitmap position is storage, not pair identity. Replacing a neighbor in a
// recycled slot must not erase retirement of the previously published pair.
// Publish net graph changes; crossing out and back within one step cancels.
namespace motorrock::pairpublicationv165 {
class State {
public:
    using Pair=std::pair<unsigned,unsigned>;
    using Slots=std::map<Pair,unsigned>;
    struct Events{std::vector<Pair> created,removed;};
private:
    Slots previous_;
public:
    void clear(){previous_.clear();}
    Events update(const Slots& current,const std::vector<Pair>& createdOrder) {
        Events e;std::vector<std::pair<unsigned,Pair>> removed;
        for(const auto& entry:previous_)if(!current.count(entry.first))removed.push_back({entry.second,entry.first});
        std::sort(removed.begin(),removed.end());
        for(const auto& entry:removed)e.removed.push_back(entry.second);
        for(const auto& pair:createdOrder) {
            const Pair key={std::min(pair.first,pair.second),std::max(pair.first,pair.second)};
            if(!previous_.count(key))e.created.push_back(pair);
        }
        previous_=current;return e;
    }
};
}
