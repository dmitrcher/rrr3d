#pragma once
#include "NativeWheelRayV52.h"
#include <cmath>
// Offline reconstruction of PhysXCore RVA14b72e..14b78b. Not installed.
namespace motorrock::longitudinalslipv81 {
struct Result { float negativeRolling, rawSlip, normalizedSlip; };
inline Result calculate(float forward,float axle,float radius) {
    namespace f=motorrock::wheelrayv52;
    const float negative=-f::mul(radius,axle);
    const float raw=f::add(negative,forward);
    const float magnitude=std::abs(negative);
    return {negative,raw,magnitude>1?f::div(raw,magnitude):raw};
}
}
