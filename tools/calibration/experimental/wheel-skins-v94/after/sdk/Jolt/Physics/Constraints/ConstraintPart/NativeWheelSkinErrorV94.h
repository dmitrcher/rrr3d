#pragma once
namespace motorrock_native_wheel_skin_v94 {
inline float error(float height,float radius,float contact_skin,float wheel_skin) {
    volatile float displacement=height-radius;
    volatile float contact=displacement+contact_skin;
    volatile float result=contact+wheel_skin;
    return result;
}
}
