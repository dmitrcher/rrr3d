#pragma once
#include "NativeWheelPoseV54.h"
#include <cstdlib>
#include <cstring>
namespace motorrock::tirev55 {
using namespace motorrock::wheelrayv52;
inline bool enabled(const char* n) { const char* s=std::getenv(n);return s&&std::strcmp(s,"1")==0; }
inline float dotXYZ(Vector a,Vector b) {return add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));}
struct Basis { Vector lateral, longitudinal, suspension; };
inline Basis basisZeroSteer(motorrock::wheelposev54::Pose p,Vector normal) {
    const Vector shapeX{p[0],p[3],p[6]};const float projection=dotZYX(shapeX,normal);
    Vector lateral;
    for(int i=0;i<3;++i)lateral[i]=sub(shapeX[i],mul(normal[i],projection));
    const float length=std::sqrt(dotXYZ(lateral,lateral));
    if(length>0){const float inverse=div(1,length);for(auto& v:lateral)v=mul(v,inverse);}
    return {lateral,cross(lateral,normal),{p[1],p[4],p[7]}};
}
inline float lateralAngle(Vector relative,Vector lateral) {
    const float length=std::sqrt(dotZYX(relative,relative));
    if(!(length>9.999999747378752e-6F))return 1.5707963705062866F;
    const float cosine=div(dotZYX(lateral,relative),length);
    if(cosine>=1)return 1.5707963705062866F;
    if(cosine<=-1)return -1.5707963705062866F;
    return std::asin(cosine);
}
inline float curve(float slip,float extremumSlip,float extremumValue,float asymptoteSlip,float asymptoteValue) {
    const float magnitude=std::abs(slip),sign=slip>=0?1.0F:-1.0F;
    if(magnitude<extremumSlip){const float a=div(magnitude,extremumSlip),a2=mul(a,a),a3=mul(a2,a);return mul(mul(add(sub(a2,a3),a),extremumValue),sign);}
    if(magnitude<asymptoteSlip){const float a=div(sub(magnitude,extremumSlip),sub(asymptoteSlip,extremumSlip)),a2=mul(a,a),a3=mul(a2,a);return mul(add(mul(sub(mul(a2,3),mul(a3,2)),sub(asymptoteValue,extremumValue)),extremumValue),sign);}
    return mul(asymptoteValue,sign);
}
struct AnchorResult { bool update; float lateral_error,longitudinal_error; };
inline AnchorResult anchor(Vector point,Vector previous,Vector lateral,Vector longitudinal,float skin,float axle) {
    const auto d=subtract(point,previous);
    if(dotXZY(d,d)>mul(skin,skin))return {true,0,0};
    return {false,dotZYX(lateral,d),axle==0?dotZYX(longitudinal,d):0};
}
inline float impulseScale(float force,float projection,float dt) {return mul(mul(force,projection),dt);}
} // namespace motorrock::tirev55
