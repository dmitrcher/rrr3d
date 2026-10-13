#pragma once
#include "NativeTireV55.h"
#include <algorithm>

namespace motorrock::normalforcev84 {
struct Result { float force, reaction; bool released; };
// Original Windows wheel callback operates on forces. Preserve that domain:
// multiplying by dt and dividing back changes both the clamp and tire bounds.
// The original callback uses x87 extended precision (control 0x37f) until
// storing load. Binary64 exactly retains these measured integer-mass/gravity
// products and reproduces all captured flat and slope loads; no f32 stores
// between the multiplications. Arbitrary unmeasured inputs are not certified.
inline float normalLoad(float mass, float normalZ, float gravityZ, float wheelCount) {
    volatile double normalMass = static_cast<double>(mass) * normalZ;
    volatile double projected = normalMass * gravityZ;
    volatile double load = projected / wheelCount;
    return std::abs(static_cast<float>(load));
}
inline Result modify(float force, float mass, float normalZ, float gravityZ,
                     float wheelCount, bool traction, float tireSpring) {
    namespace n = motorrock::tirev55;
    const float load = normalLoad(mass, normalZ, gravityZ, wheelCount);
    const float reaction = load > 0 ? n::div(force, load) : 0;
    const bool released = traction && tireSpring > 0 && force > n::mul(load, tireSpring);
    const float allowed = !traction || released ? 0 : std::min(force, n::mul(load, 1.5F));
    return {allowed, reaction, released};
}
} // namespace motorrock::normalforcev84
