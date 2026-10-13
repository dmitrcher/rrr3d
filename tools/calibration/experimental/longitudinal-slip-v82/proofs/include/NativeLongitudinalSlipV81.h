#pragma once
#include "NativeWheelRayV52.h"
#include <cmath>
// PhysXCore RVA14b72e..14b78b arithmetic, verified against immutable Native captures.
// Live use is gated by RRR3D_EXPERIMENT_NATIVE_LONGITUDINAL_SLIP_V82.
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
