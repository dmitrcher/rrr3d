#pragma once
#include "NativeContactManagerOrderV108.h"
// Original actor-owned dense interaction list also contains body managers and
// suppressed collision markers. They do not occupy wheel-processor slots, but
// their retirement swaps change the order in which a sleeping actor reactivates
// its retained wheel managers. No measured wheel permutation is stored here.
namespace motorrock::actorinteractionsv163 {
class List {
    contactmanagerorderv108::List interactions_;
    bool wasActive_=true;
public:
    using Pair=contactmanagerorderv108::List::Pair;
    void clear(){interactions_.clear();wasActive_=true;}
    void update(const std::vector<Pair>& added,const std::vector<Pair>& removed,
                bool active,contactmanagerorderv108::List& wheels,
                const std::vector<Pair>& removedWheels) {
        interactions_.update(added,removed);
        if(active && !wasActive_) {
            std::vector<Pair> reactivated;
            for(const auto& pair:interactions_.entries())
                if(wheels.order(pair.first,pair.second))reactivated.push_back(pair);
            // Preserve the wheel processor's deferred broadphase retirements.
            wheels.clear();wheels.update(reactivated,removedWheels);
        }
        wasActive_=active;
    }
    const auto& entries() const{return interactions_.entries();}
};
}
