#pragma once
#include <cstdint>
namespace NativeWheelCallbackQueueV105 {
inline void enqueue(bool currentContact, uint64_t &order, uint64_t &counter) {
    if(currentContact && !order)order=++counter;
}
inline bool before(uint64_t a, uint64_t b) { return a < b; }
}
