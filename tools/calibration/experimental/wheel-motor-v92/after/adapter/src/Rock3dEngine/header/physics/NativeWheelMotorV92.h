#pragma once
namespace motorrock::wheelmotorv92 {
inline float mul(float a,float b) {volatile float result=a*b;return result;}
inline float add(float a,float b) {volatile float result=a+b;return result;}
// Actual scalar SSE: motor torque times step, times stored inverse wheel mass,
// then add the incoming axle speed. Each operation rounds separately.
inline float apply(float axle,float torque,float delta,float inverseWheelMass) {
    return add(axle,mul(mul(torque,delta),inverseWheelMass));
}
}
