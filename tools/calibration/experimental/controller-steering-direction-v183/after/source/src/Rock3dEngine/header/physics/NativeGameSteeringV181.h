#pragma once
#include "NativePublicAngular.h"
#include "NativeMaterialFrameV36.h"
#include "NativePhysicalPoseV42.h"
#include <cmath>

namespace motorrock::gamesteeringv181 {
namespace p = nativepublicangular;
namespace cf = contactframe;
using Vector = cf::Vector;
using Quaternion = cf::Quaternion;
using Matrix = p::Matrix;
struct Pose { Vector position; Quaternion rotation; };
inline float dot(const std::array<float, 3>& a, const std::array<float, 3>& b) {
    return p::add(p::add(p::mul(a[0],b[0]),p::mul(a[1],b[1])),p::mul(a[2],b[2]));
}
inline Matrix multiply(const Matrix& a, const Matrix& b) {
    Matrix r{};
    for(unsigned i=0;i<3;++i) for(unsigned j=0;j<3;++j)
        r[i][j]=dot(a[i],{{b[0][j],b[1][j],b[2][j]}});
    return r;
}
inline Vector point(const Matrix& r, const Vector& t, const Vector& v) {
    Vector out{};
    for(unsigned i=0;i<3;++i) out[i]=p::add(dot(r[i],{{v[0],v[1],v[2]}}),t[i]);
    return out;
}
// Original NxQuat::rot(X), then GLM dot: explicit PC24 operations.
// Keep this separate from the graph/motor speed carried by the existing bridge.
inline float physicalForwardSpeed(const Quaternion& q, const Vector& velocity) {
    const float a=p::sub(p::mul(q[3],q[3]),.5F);
    const Vector direction{{p::mul(p::add(a,p::mul(q[0],q[0])),2),
        p::mul(p::add(p::mul(q[2],q[3]),p::mul(q[1],q[0])),2),
        p::mul(p::add(-p::mul(q[1],q[3]),p::mul(q[2],q[0])),2)}};
    return p::add(p::add(p::mul(direction[0],velocity[0]),
        p::mul(direction[1],velocity[1])),p::mul(direction[2],velocity[2]));
}
inline Pose apply(const Matrix& original, const Vector& position, float yaw, float rearX, const Quaternion* previousRotation = nullptr) {
    // Preserve the three matrix products, including the two pivot translations.
    // Even yaw zero executes this path in original WheelsProgress.
    const float half=p::mul(yaw,.5F);
    const Quaternion delta{{0,0,cf::store(std::sin(double(half))),cf::store(std::cos(double(half)))}};
    const auto rotated=multiply(original,p::orientation({{delta[0],delta[1],delta[2],delta[3]}}));
    const auto pivot=point(original,position,{{rearX,0,0}});
    const auto shifted=point(rotated,pivot,{{-rearX,0,0}});
    bool unchanged = previousRotation != nullptr;
    for(unsigned i=0;i<3;++i) {
        unchanged = unchanged && shifted[i] == position[i];
        for(unsigned j=0;j<3;++j) unchanged = unchanged && rotated[i][j] == original[i][j];
    }
    // The original pose setter retains its stored quaternion for an equal pose.
    if(unchanged) return {shifted,*previousRotation};
    cf::Matrix matrix;
    for(unsigned i=0;i<3;++i) for(unsigned j=0;j<3;++j) matrix.a[i][j]=rotated[i][j];
    auto q=framev36::encode(matrix);
    const auto trace=p::add(p::add(rotated[0][0],rotated[1][1]),rotated[2][2]);
    if(trace>=0) for(unsigned i=0;i<3;++i)q[i]=-q[i]; else q[3]=-q[3];
    // NxActor::setGlobalPose normalizes the quaternion produced from its matrix.
    // The public pose setter normalizes at PC24: square each component and
    // accumulate X,Y,Z,W with float stores, rather than the solver's PC53 sum.
    float square=p::mul(q[0],q[0]);
    for(unsigned i=1;i<4;++i)square=p::add(square,p::mul(q[i],q[i]));
    const float magnitude=cf::store(std::sqrt(double(square)));
    if(magnitude!=0) {
        const float inverse=cf::store(1.0/double(magnitude));
        for(unsigned i=0;i<4;++i)q[i]=p::mul(q[i],inverse);
    }
    return {shifted,q};
}
}
