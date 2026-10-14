#pragma once
#include "NativePhysicalPoseV42.h"

// Core 22c8a0..22d39f: relative transforms use quaternion products and
// quaternion translation, each scalar operation stored as float32.
// Matrix transpose/multiplication is a distinct arithmetic path.
namespace motorrock::relativecontactv147 {
namespace cf=contactframe;
namespace fp=physicalposev42;
struct Pose { cf::Vector position; cf::Quaternion quaternion; };
struct Transform { cf::Matrix rotation; cf::Vector position; };
inline cf::Quaternion conjugate(cf::Quaternion q) {
    return {{-q[0],-q[1],-q[2],q[3]}};
}
inline cf::Quaternion product(cf::Quaternion a,cf::Quaternion b) {
    const float x=a[0],y=a[1],z=a[2],w=a[3];
    const float X=b[0],Y=b[1],Z=b[2],W=b[3];
    return {{
        fp::add(fp::add(fp::sub(fp::mul(y,Z),fp::mul(z,Y)),fp::mul(w,X)),fp::mul(x,W)),
        fp::add(fp::add(fp::sub(fp::mul(z,X),fp::mul(x,Z)),fp::mul(w,Y)),fp::mul(y,W)),
        fp::add(fp::add(fp::sub(fp::mul(x,Y),fp::mul(y,X)),fp::mul(w,Z)),fp::mul(z,W)),
        fp::sub(fp::mul(w,W),fp::add(fp::add(fp::mul(y,Y),fp::mul(z,Z)),fp::mul(x,X)))
    }};
}
inline cf::Vector inverseRotate(cf::Quaternion q,cf::Vector v) {
    const float x=-q[0],y=-q[1],z=-q[2],w=q[3];
    const float dot=fp::add(fp::add(fp::mul(v[0],x),fp::mul(v[2],z)),fp::mul(v[1],y));
    const cf::Vector axis={{x,y,z}};
    const cf::Vector cross={{
        fp::sub(fp::mul(y,v[2]),fp::mul(z,v[1])),
        fp::sub(fp::mul(z,v[0]),fp::mul(x,v[2])),
        fp::sub(fp::mul(x,v[1]),fp::mul(y,v[0]))}};
    const float scale=fp::sub(fp::mul(fp::mul(w,w),2.0f),1.0f);
    cf::Vector out;
    for(unsigned i=0;i<3;++i)
        out[i]=fp::add(fp::add(fp::mul(scale,v[i]),fp::mul(fp::mul(cross[i],w),2.0f)),fp::mul(fp::mul(dot,axis[i]),2.0f));
    return out;
}
inline Transform relative(const Pose& frame,const Pose& target) {
    cf::Vector delta;
    for(unsigned i=0;i<3;++i)delta[i]=fp::sub(target.position[i],frame.position[i]);
    return {fp::rotation(product(conjugate(frame.quaternion),target.quaternion)),inverseRotate(frame.quaternion,delta)};
}
}
