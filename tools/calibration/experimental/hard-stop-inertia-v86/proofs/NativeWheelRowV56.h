#pragma once
#include <array>
#include "NativeSpringSplitV53.h"
namespace motorrock_native_wheel_row_v56 {
using namespace motorrock_native_spring_v53;
using Vector=std::array<float,3>;using Matrix=std::array<float,9>;
inline Vector multiply(Matrix matrix,Vector jacobian) {
 Vector out;for(unsigned i=0;i<3;++i)out[i]=add(add(mul(matrix[i*3],jacobian[0]),mul(matrix[i*3+1],jacobian[1])),mul(matrix[i*3+2],jacobian[2]));return out;
}
inline float inverse_mass(Vector axis,Vector jacobian,Vector inverse_jacobian,float inverse_body_mass) {
 float k=mul(add(add(mul(axis[0],axis[0]),mul(axis[2],axis[2])),mul(axis[1],axis[1])),inverse_body_mass);
 k=add(k,mul(jacobian[0],inverse_jacobian[0]));k=add(k,mul(jacobian[2],inverse_jacobian[2]));return add(k,mul(jacobian[1],inverse_jacobian[1]));
}
}
