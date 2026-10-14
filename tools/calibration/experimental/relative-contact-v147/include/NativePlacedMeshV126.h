#pragma once
#include "NativeMaterialFrameV36.h"
#include "NativePhysicalPoseV42.h"
#include "NativeWheelRayV52.h"

// Default-off placed-mesh experiment. Geometry comes only from WorldDescription.
// NxActorDesc public matrix -> Core actor quaternion -> physical shape matrix.
namespace motorrock::placedmeshv126 {
namespace wr=wheelrayv52;
struct Pose { contactframe::Quaternion quaternion; contactframe::Matrix matrix; wr::Vector position; };
inline Pose pose(contactframe::Quaternion input,wr::Vector position) {
    const double x=input[0],y=input[1],z=input[2],w=input[3];
    contactframe::Matrix m;
    m.a[0][0]=float(1-y*y*2-z*z*2);m.a[0][1]=float(x*y*2-w*z*2);m.a[0][2]=float(x*z*2+w*y*2);
    m.a[1][0]=float(x*y*2+w*z*2);m.a[1][1]=float(1-x*x*2-z*z*2);m.a[1][2]=float(y*z*2-w*x*2);
    m.a[2][0]=float(x*z*2-w*y*2);m.a[2][1]=float(y*z*2+w*x*2);m.a[2][2]=float(1-x*x*2-y*y*2);
    auto q=framev36::encode(m);
    const float trace=framev36::add(framev36::add(m.a[0][0],m.a[1][1]),m.a[2][2]);
    if(trace>=0) { for(unsigned i=0;i<3;++i)q[i]=-q[i]; } else q[3]=-q[3];
    return {q,physicalposev42::rotation(q),position};
}
inline float dotXYZ(wr::Vector a,wr::Vector b) {
    return wr::add(wr::add(wr::mul(a[0],b[0]),wr::mul(a[1],b[1])),wr::mul(a[2],b[2]));
}
inline wr::Vector inverseDirection(const Pose& p,wr::Vector v) {
    wr::Vector r;
    for(unsigned i=0;i<3;++i)r[i]=dotXYZ({{p.matrix.a[0][i],p.matrix.a[1][i],p.matrix.a[2][i]}},v);
    return r;
}
inline wr::Vector inversePoint(const Pose& p,wr::Vector v) {
    // Core RVA 1f0afd..1f0c71: inverse translation is stored before origin.
    wr::Vector r;
    for(unsigned i=0;i<3;++i) {
        const wr::Vector column{{p.matrix.a[0][i],p.matrix.a[1][i],p.matrix.a[2][i]}};
        const float translation=-(i==0?wr::dotZYX(column,p.position):dotXYZ(column,p.position));
        r[i]=wr::add(dotXYZ(column,v),translation);
    }
    return r;
}
inline wr::Vector direction(const Pose& p,wr::Vector v) {
    // Core RVA 14e1b6..14e245: all forward rows sum Z, Y, X.
    wr::Vector r;
    for(unsigned i=0;i<3;++i)r[i]=wr::dotZYX({{p.matrix.a[i][0],p.matrix.a[i][1],p.matrix.a[i][2]}},v);
    return r;
}
inline wr::Vector point(const Pose& p,wr::Vector v) {
    auto r=direction(p,v);for(unsigned i=0;i<3;++i)r[i]=wr::add(r[i],p.position[i]);return r;
}
}
