#pragma once
#include <array>
namespace motorrock::localangularv114 {
inline float mul(float a,float b){volatile float v=a*b;return v;}
inline float add(float a,float b){volatile float v=a+b;return v;}
inline float sub(float a,float b){volatile float v=a-b;return v;}
// Actual original PhysX local-vector rotation, RVA 0x1398d0.
// Keep the original norm-sensitive expression and every scalar float store.
inline std::array<float,3> world(std::array<float,4> q,std::array<float,3> v){
 const float dot=add(add(mul(v[0],q[0]),mul(v[1],q[1])),mul(v[2],q[2]));
 const std::array<float,3> projection{{mul(dot,q[0]),mul(dot,q[1]),mul(dot,q[2])}};
 const std::array<float,3> cross{{sub(mul(v[2],q[1]),mul(v[1],q[2])),sub(mul(v[0],q[2]),mul(v[2],q[0])),sub(mul(v[1],q[0]),mul(v[0],q[1]))}};
 const float scale=sub(mul(q[3],q[3]),.5f);
 std::array<float,3> result{};
 for(unsigned i=0;i<3;++i)result[i]=mul(add(add(mul(v[i],scale),mul(cross[i],q[3])),projection[i]),2.0f);
 return result;
}
}
