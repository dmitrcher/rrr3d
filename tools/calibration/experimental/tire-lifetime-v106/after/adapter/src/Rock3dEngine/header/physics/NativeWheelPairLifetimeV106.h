#pragma once
#include <array>
#include <cstdint>
#include <set>
#include <vector>
// Original wheel contact managers are processed even when another collider
// supplies the selected wheel hit. Their deferred destruction clears all four
// wheel objects (Native 206ef0 -> 148540), independently of public HasContact.
namespace motorrock::wheelpairv106 {
using Bounds=std::array<std::array<float,3>,2>;
inline bool overlap(const Bounds& a,const Bounds& b) {
    for(unsigned axis=0;axis<3;++axis)
        if(a[0][axis]>b[1][axis] || b[0][axis]>a[1][axis])return false;
    return true;
}
class Lifetime {
    std::vector<std::set<std::uint64_t>> previous;
    std::vector<bool> pending;
public:
    void clear() {previous.clear();pending.clear();}
    std::vector<bool> update(const std::vector<std::set<std::uint64_t>>& current) {
        if(previous.size()!=current.size()) {
            previous=current;pending.assign(current.size(),false);return pending;
        }
        auto reset=pending;
        for(unsigned wheel=0;wheel<current.size();++wheel) {
            pending[wheel]=false;
            for(std::uint64_t collider:previous[wheel])if(!current[wheel].count(collider))pending[wheel]=true;
        }
        previous=current;return reset;
    }
};
}
