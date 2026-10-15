#pragma once
#include "NativeWheelRayV52.h"
#include <array>
#include <cmath>
// +Z Map.cpp death plane at source height zero. Core RVA 813c0 plane
// branch starts at 8142b: center[normalAxis] = origin - normal*1e6;
// extent[normalAxis] += abs(center[normalAxis])*FLT_EPSILON (8151c..8154d).
// These are original finite SAP bounds, separate from infinite narrow phase.
namespace motorrock::planeboundsv164 {
inline std::array<std::array<float,3>,2> deathPlane() {
    namespace wr=motorrock::wheelrayv52;
    constexpr float extent=1000000.0F;
    const float center=wr::sub(0.0F,extent);
    const float normalExtent=wr::add(extent,wr::mul(std::abs(center),0x1p-23F));
    return {{{{-extent,-extent,wr::sub(center,normalExtent)}},
             {{extent,extent,wr::add(center,normalExtent)}}}};
}
}
