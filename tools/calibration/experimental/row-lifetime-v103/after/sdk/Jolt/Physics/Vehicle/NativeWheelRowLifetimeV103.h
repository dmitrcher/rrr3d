#pragma once
// Native wheel query with mode zero and no spring object releases vertical
// objects of every wheel belonging to the actor before queued preparation.
namespace NativeWheelRowLifetimeV103 {
inline bool resetActorRows(unsigned queryMode, bool springObjectPresent) {
    return queryMode == 0 && !springObjectPresent;
}
template<class Orders> inline void clearActorRows(Orders &springs, Orders &stops) {
    for(auto &order:springs)order=0;
    for(auto &order:stops)order=0;
}
}
