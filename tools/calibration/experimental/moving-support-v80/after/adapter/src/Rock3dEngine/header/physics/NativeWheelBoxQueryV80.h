#pragma once
#include "NativeWheelBoxRayV59.h"
#include <cstring>

// Offline reconstruction of PhysXCore RVA1ae390 and RVA1ea750.
// Default-off research switch; Windows captures are test oracles only.
namespace motorrock::wheelboxqueryv80 {
namespace f = motorrock::wheelrayv52;
using Vector = f::Vector;
using Box = motorrock::wheelboxrayv59::Box;
struct Result { bool hit=false; float distance=0; Vector localStart{}, localDirection{}, localPoint{}, point{}, normal{}; };
inline bool positiveZero(float v) { unsigned w;std::memcpy(&w,&v,4);return w==0; }
inline Result cast(const Box &box,const Vector &start,const Vector &direction,float length) {
    Result r;
    const auto delta=f::subtract(start,box.position);
    for(unsigned i=0;i<3;++i) {
        const auto &c=box.columns[i];
        // The two dot products have different original SSE associations.
        r.localStart[i]=f::add(f::add(f::mul(c[1],delta[1]),f::mul(c[2],delta[2])),f::mul(c[0],delta[0]));
        r.localDirection[i]=f::add(f::add(f::mul(direction[0],c[0]),f::mul(direction[2],c[2])),f::mul(direction[1],c[1]));
    }
    Vector candidates{{-1,-1,-1}};
    bool inside=true;
    for(unsigned i=0;i<3;++i) {
        if(r.localStart[i]<-box.half[i]) { r.localPoint[i]=-box.half[i];inside=false; }
        else if(r.localStart[i]>box.half[i]) { r.localPoint[i]=box.half[i];inside=false; }
        else continue;
        if(!positiveZero(r.localDirection[i]))candidates[i]=f::div(f::sub(r.localPoint[i],r.localStart[i]),r.localDirection[i]);
    }
    if(inside)return r;
    unsigned selected=0;
    if(candidates[1]>candidates[selected])selected=1;
    if(candidates[2]>candidates[selected])selected=2;
    unsigned sign;std::memcpy(&sign,&candidates[selected],4);
    if(sign&0x80000000u)return r;
    r.distance=candidates[selected];
    for(unsigned i=0;i<3;++i)if(i!=selected) {
        r.localPoint[i]=f::add(f::mul(r.localDirection[i],r.distance),r.localStart[i]);
        if(r.localPoint[i]<f::sub(-box.half[i],1.0e-5f)||r.localPoint[i]>f::add(box.half[i],1.0e-5f))return r;
    }
    // Original transforms the local face point, rather than interpolating the
    // world ray. Preserve the distinct X/Y/Z accumulation orders.
    for(unsigned i=0;i<3;++i) {
        const auto &a=box.columns[0], &b=box.columns[1], &c=box.columns[2];
        float projected;
        if(i==0)projected=f::add(f::add(f::mul(b[i],r.localPoint[1]),f::mul(a[i],r.localPoint[0])),f::mul(c[i],r.localPoint[2]));
        else if(i==1)projected=f::add(f::add(f::mul(b[i],r.localPoint[1]),f::mul(c[i],r.localPoint[2])),f::mul(a[i],r.localPoint[0]));
        else projected=f::add(f::add(f::mul(b[i],r.localPoint[1]),f::mul(c[i],r.localPoint[2])),f::mul(a[i],r.localPoint[0]));
        r.point[i]=f::add(box.position[i],projected);
        // Normal projection also retains the original zero products.
        Vector axis{{0,0,0}};axis[selected]=r.localPoint[selected]>0?1.0f:-1.0f;
        r.normal[i]=f::add(f::add(f::mul(b[i],axis[1]),f::mul(c[i],axis[2])),f::mul(a[i],axis[0]));
    }
    if(r.distance>length)return r;
    r.hit=true;return r;
}
}
