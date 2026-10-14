#pragma once
#include "NativeTireV55.h"

namespace motorrock::steeringtirev118 {
inline tirev55::Basis basis(wheelposev54::Pose pose, tirev55::Vector normal, float angle) {
    const auto base=tirev55::basisZeroSteer(pose,normal);
    if (angle==0.0F) return base;
    // Original wheel preparation uses x87 fsincos, with float stores before SSE rotation.
    const float sine=static_cast<float>(std::sin(double(angle)));
    const float cosine=static_cast<float>(std::cos(double(angle)));
    tirev55::Basis result=base;
    for (unsigned i=0;i<3;++i) {
        result.lateral[i]=tirev55::sub(tirev55::mul(cosine,base.lateral[i]),tirev55::mul(sine,base.longitudinal[i]));
        result.longitudinal[i]=tirev55::add(tirev55::mul(sine,base.lateral[i]),tirev55::mul(cosine,base.longitudinal[i]));
    }
    return result;
}
}
