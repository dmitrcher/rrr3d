#pragma once
#include <cmath>

namespace motorrock_combined_tire_v95 {
struct Result { float longitudinal, lateral, sigma; };
inline Result normalize(float longitudinal, float lateral, float maxLongitudinal, float maxLateral) {
    volatile float x = longitudinal / maxLongitudinal;
    volatile float y = lateral / maxLateral;
    volatile float xx = x * x;
    volatile float yy = y * y;
    volatile float sigma = yy + xx;
    if (!(sigma > 1.0f)) return {longitudinal, lateral, sigma};
    // Widen the scaling stage; actual Native uses x87 extended precision.
    // This experimental approximation requires independently bound proofs;
    // equivalence for all unmeasured float32 inputs is not established.
    const double scale = 1.0 / std::sqrt(static_cast<double>(sigma));
    volatile float outLongitudinal = static_cast<double>(longitudinal) * scale;
    volatile float outLateral = static_cast<double>(lateral) * scale;
    return {outLongitudinal, outLateral, sigma};
}
}
