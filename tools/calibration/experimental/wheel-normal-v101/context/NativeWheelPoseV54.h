#pragma once
#include "NativeWheelRayV52.h"
namespace motorrock::wheelposev54 {
using namespace motorrock::wheelrayv52;
using Transform = std::array<float, 7>; // position XYZ, quaternion XYZW
using Pose = std::array<float, 12>; // row-major rotation, position XYZ
inline Transform compose(Transform parent, Transform local) {
    const float x=local[0], y=local[1], z=local[2];
    const float X=parent[3], Y=parent[4], Z=parent[5], W=parent[6];
    const float dot=add(add(mul(z,Z),mul(x,X)),mul(y,Y));
    const Vector cross{mul(sub(mul(z,Y),mul(y,Z)),W),
                       mul(sub(mul(x,Z),mul(z,X)),W),
                       mul(sub(mul(y,X),mul(x,Y)),W)};
    const float scalar=sub(mul(W,W),0.5F);
    Transform out{};
    for (int i=0;i<3;++i)
        out[i]=add(parent[i],mul(add(add(mul(local[i],scalar),cross[i]),mul(parent[3+i],dot)),2.0F));
    const float a=local[3],b=local[4],c=local[5],d=local[6];
    out[3]=sub(add(add(mul(a,W),mul(X,d)),mul(Y,c)),mul(Z,b));
    out[4]=sub(add(add(mul(Z,a),mul(b,W)),mul(d,Y)),mul(X,c));
    out[5]=sub(add(add(mul(b,X),mul(Z,d)),mul(W,c)),mul(a,Y));
    out[6]=sub(sub(sub(mul(W,d),mul(a,X)),mul(b,Y)),mul(Z,c));
    return out;
}
inline Pose pose(Transform t) {
    const float x=t[3],y=t[4],z=t[5],w=t[6];
    const float xx=mul(mul(x,x),2), yy=mul(mul(y,y),2), zz=mul(mul(z,z),2);
    const float xy=mul(mul(y,x),2), zw=mul(mul(z,w),2), xz=mul(mul(z,x),2);
    const float yw=mul(mul(y,w),2), xw=mul(mul(x,w),2), yz=mul(mul(z,y),2);
    return {sub(sub(1,yy),zz),sub(xy,zw),add(yw,xz),
            add(zw,xy),sub(sub(1,xx),zz),sub(yz,xw),
            sub(xz,yw),add(xw,yz),sub(sub(1,xx),yy), t[0],t[1],t[2]};
}
struct Ray { Vector start, end, axis, unit; float length; };
inline Ray ray(Pose p, float radius, float travel) {
    Ray r{{p[9],p[10],p[11]},{},{p[1],p[4],p[7]},{},0};
    const float extent=add(travel,radius);
    Vector delta;
    for(int i=0;i<3;++i) {
        r.end[i]=sub(r.start[i],mul(r.axis[i],extent));
        delta[i]=sub(r.end[i],r.start[i]);
    }
    r.length=std::sqrt(dotZYX(delta,delta));
    const float inv=r.length>0?div(1,r.length):0;
    for(int i=0;i<3;++i)r.unit[i]=mul(delta[i],inv);
    return r;
}
inline float distance(Vector axis, Vector start, Vector point) {
    return -dotZYX(axis,subtract(point,start));
}
} // namespace motorrock::wheelposev54
