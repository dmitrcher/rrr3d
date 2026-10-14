#pragma once
#include <cmath>
namespace motorrock_native_spring_v53 {
inline float add(float a,float b){volatile float r=a+b;return r;}
inline float sub(float a,float b){volatile float r=a-b;return r;}
inline float mul(float a,float b){volatile float r=a*b;return r;}
inline float div(float a,float b){volatile float r=a/b;return r;}
struct State { bool enabled=false, native_jv=false; float unbiased=0.0f, position_term=0.0f, impulse_softness=0.0f, velocity_bias=0.0f; };
struct Coefficients { float mass, softness, position_coefficient; };
inline Coefficients prepare(float inverse_mass,float dt,float stiffness,float damping,float bias_factor) {
 float hard_mass=div(1.0f,inverse_mass);
 float damp=damping<9.999999747378752e-06f?9.999999747378752e-06f:damping;
 float dt_stiffness=mul(dt,stiffness);
 float denominator=add(dt_stiffness,damp);
 float beta=div(dt_stiffness,denominator);
 float gamma=div(1.0f,mul(denominator,dt));
 float scale=div(1.0f,add(mul(gamma,hard_mass),1.0f));
 return {mul(scale,hard_mass),div(hard_mass,add(div(1.0f,gamma),hard_mass)),mul(mul(mul(hard_mass,bias_factor),beta),scale)};
}
inline float body0_jv(const float *axis,const float *lever,const float *linear,const float *angular){
 float r=add(mul(lever[2],angular[2]),mul(lever[0],angular[0]));
 r=add(r,mul(axis[2],linear[2]));r=add(r,mul(axis[1],linear[1]));
 r=add(r,mul(lever[1],angular[1]));return add(r,mul(axis[0],linear[0]));
}
inline float body1_jv(const float *axis,const float *lever,const float *linear,const float *angular){
 float r=add(mul(lever[1],angular[1]),mul(lever[2],angular[2]));
 r=add(r,mul(lever[0],angular[0]));r=add(r,mul(axis[2],linear[2]));
 r=add(r,mul(axis[1],linear[1]));return add(r,mul(axis[0],linear[0]));
}
struct Result { float unbiased, biased, delta; };
// The original solver keeps an unclamped impulse without geometric correction
// alongside the clamped impulse applied to body velocity. Every operation is F32.
inline Result solve(float velocity_term,float position_term,float impulse_softness,
                    float unbiased,float biased,float scale,float minimum,float maximum) {
 float next_unbiased=add(unbiased,mul(sub(velocity_term,mul(impulse_softness,unbiased)),scale));
 float delta=mul(sub(sub(velocity_term,position_term),mul(impulse_softness,biased)),scale);
 float next_biased=add(biased,delta);
 if(next_biased>maximum){next_biased=maximum;delta=sub(maximum,biased);}
 else if(minimum>next_biased){next_biased=minimum;delta=sub(minimum,biased);}
 return {next_unbiased,next_biased,delta};
}
inline float final_softness(float impulse_softness,float unbiased,float biased){
 return std::fabs(biased)>1.000000013351432e-10f?mul(div(unbiased,biased),impulse_softness):impulse_softness;
}
}
