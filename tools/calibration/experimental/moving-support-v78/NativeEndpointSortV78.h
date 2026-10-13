#pragma once
#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

// Research reconstruction of PhysXCore.dll RVA 0x327e0. The original unstable
// tie swaps are observable in subsequent contact-manager creation order.
// Input coordinates must already be encoded as unsigned sortable float keys.
// Descending output is consumed backwards by the original endpoint merge.
namespace NativeEndpointSortV78 {
struct Endpoint { std::uint32_t coordinate, ownerAndType; };
inline bool less(const Endpoint &a, const Endpoint &b) {
    return a.coordinate < b.coordinate ||
        (a.coordinate == b.coordinate && (a.ownerAndType & 0x80000000u) &&
         !(b.ownerAndType & 0x80000000u));
}
inline void sort(std::vector<Endpoint> &a) {
    if (a.empty()) return;
    std::vector<std::pair<int,int>> pending{{0, static_cast<int>(a.size()) - 1}};
    while (!pending.empty()) {
        auto bounds = pending.back(); pending.pop_back();
        int lo = bounds.first, hi = bounds.second;
        while (lo < hi) {
            if (hi - lo < 5) {
                for (int start = lo; start < hi; ++start) {
                    int largest = start;
                    for (int i = start + 1; i <= hi; ++i)
                        if (less(a[largest], a[i])) largest = i;
                    std::swap(a[start], a[largest]);
                }
                break;
            }
            int mid = (lo + hi) / 2;
            if (less(a[lo], a[mid])) std::swap(a[lo], a[mid]);
            if (less(a[lo], a[hi])) std::swap(a[lo], a[hi]);
            if (less(a[mid], a[hi])) std::swap(a[mid], a[hi]);
            Endpoint pivot = a[mid];
            std::swap(a[mid], a[hi - 1]);
            int left = lo, right = hi - 1;
            for (;;) {
                do { ++left; } while (less(pivot, a[left]));
                do { --right; } while (less(a[right], pivot));
                if (left >= right) break;
                std::swap(a[left], a[right]);
            }
            std::swap(a[left], a[hi - 1]);
            if (left - lo > hi - left) {
                pending.emplace_back(lo, left - 1); lo = left + 1;
            } else {
                pending.emplace_back(left + 1, hi); hi = left - 1;
            }
        }
    }
}
}
