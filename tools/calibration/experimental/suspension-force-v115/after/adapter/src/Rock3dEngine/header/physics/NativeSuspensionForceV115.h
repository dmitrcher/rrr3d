#pragma once

namespace motorrock::suspensionforcev115 {
// Original wheel update RVA 148883/148890, then 1488bd/1488ca.
// Store the spring contribution before adding the stop contribution.
// The original x87 product and addition have no intervening float store.
inline float store(float springImpulse, float stopImpulse, float step) {
    volatile float inverseStep = 1.0f / step;
    volatile float springForce = static_cast<float>(double(springImpulse) * double(inverseStep));
    volatile double stopForce = double(stopImpulse) * double(inverseStep);
    volatile float result = static_cast<float>(stopForce + double(springForce));
    return result;
}
}
