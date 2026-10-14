#pragma once
namespace motorrock::freewheelcachev112 {
// Original integration uses cached ray data before the new ray query.
// The contact distance sentinel is represented by the query-valid bit.
inline float axle(float current, float forward, float radius, float longitudinalStiffness,
                  bool cachedContact, unsigned flags) {
    if(cachedContact && (flags & 16U)==0U && (longitudinalStiffness!=0.0f || (flags & 64U)!=0U)) {
        volatile float result=forward/radius;
        return result;
    }
    return current;
}
}
