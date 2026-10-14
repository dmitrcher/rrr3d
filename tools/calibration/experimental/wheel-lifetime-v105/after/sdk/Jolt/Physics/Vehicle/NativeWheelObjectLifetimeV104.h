#pragma once
// Object lifetime is distinct from active solver mass or spring coefficients.
namespace NativeWheelObjectLifetimeV104 {
inline bool springConfigured(float originalSuspensionTravel) {
    return originalSuspensionTravel > 0.0f;
}
inline bool resetActorRows(unsigned queryMode, bool springObjectPresent) {
    return queryMode == 0 && !springObjectPresent;
}
}
