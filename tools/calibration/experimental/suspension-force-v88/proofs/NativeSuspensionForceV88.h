#pragma once
// Source solver stores spring impulse times a rounded inverse step, not
// impulse divided by the step. Default-off caller controls this experiment.
namespace motorrock { namespace suspensionforcev88 {
inline float store(float impulse,float step) {
    volatile float inverse=1.0f/step;
    volatile float result=impulse*inverse;
    return result;
}
}}
