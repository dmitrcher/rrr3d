// Original PhysX contact force response gate (RVA 1e83bf..1e83e3).
#pragma once
#include <array>
#include <cstdlib>
#include <cstring>
namespace motorrock_native_force_gate_v123 {
inline bool enabled() {
 const char* flag=std::getenv("RRR3D_EXPERIMENT_NATIVE_CONTACT_FORCE_GATE_V123");
 return flag && std::strcmp(flag,"1")==0;
}
inline float mul(float a,float b) { volatile float v=a*b;return v; }
inline float add(float a,float b) { volatile float v=a+b;return v; }
inline float squared_norm(const std::array<float,3>&force) {
 return add(add(mul(force[0],force[0]),mul(force[2],force[2])),mul(force[1],force[1]));
}
inline bool should_apply(const std::array<float,3>&force) { return squared_norm(force)!=0.0f; }
}
