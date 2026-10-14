#pragma once
#include <algorithm>
#include "NativeWheelBoxQueryV80.h"
#include "NativeWheelPairLifetimeV106.h"
// Original static wheel queries require a raw suspension-line/box broadphase
// overlap before the local face kernel's epsilon bounds can be consulted.
namespace motorrock::staticwheelboxv107 {
namespace f=motorrock::wheelrayv52;
using Box=motorrock::wheelboxqueryv80::Box;
using Vector=f::Vector;
inline bool candidate(const Box& box,const Vector& start,const Vector& end) {
    motorrock::wheelpairv106::Bounds line{},bounds{};
    for(unsigned axis=0;axis<3;++axis) {
        line[0][axis]=std::min(start[axis],end[axis]);line[1][axis]=std::max(start[axis],end[axis]);
        const float extent=f::add(f::add(f::mul(std::abs(box.columns[0][axis]),box.half[0]),f::mul(std::abs(box.columns[1][axis]),box.half[1])),f::mul(std::abs(box.columns[2][axis]),box.half[2]));
        bounds[0][axis]=f::sub(box.position[axis],extent);bounds[1][axis]=f::add(box.position[axis],extent);
    }
    return motorrock::wheelpairv106::overlap(line,bounds);
}
inline motorrock::wheelboxqueryv80::Result cast(const Box& box,const Vector& start,const Vector& end,const Vector& direction,float length) {
    return candidate(box,start,end)?motorrock::wheelboxqueryv80::cast(box,start,direction,length):motorrock::wheelboxqueryv80::Result{};
}
}
