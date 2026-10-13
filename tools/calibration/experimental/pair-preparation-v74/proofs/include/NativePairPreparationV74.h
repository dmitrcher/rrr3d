#pragma once
#include "NativePairContactArithmeticV73.h"
namespace motorrock_native_pair_v74 {
namespace P=motorrock_native_pair_v73;
inline float orderedDot(P::Vector a,P::Vector b,unsigned i,unsigned j,unsigned k){return P::add(P::add(P::mul(a[i],b[i]),P::mul(a[j],b[j])),P::mul(a[k],b[k]));}
// Original scalar SSE at RVA841e0..84c74 projects the force response
// cross(I*cross(r,axis),r); it does not square the angular Jacobian.
// The separate body dot associations are from the disassembly, not fitted masses.
inline float effectiveMass(const std::array<P::Body,2>& bodies,P::Vector axis,unsigned index){
 const unsigned orders[3][2][3]={{{2,0,1},{0,2,1}},{{0,2,1},{2,1,0}},{{1,2,0},{1,2,0}}};
 float contribution[2];
 for(unsigned b=0;b<2;++b){const auto response=P::cross(P::tensor(bodies[b].inverse_inertia,P::cross(bodies[b].lever,axis)),bodies[b].lever);const auto* order=orders[index][b];contribution[b]=P::add(orderedDot(response,axis,order[0],order[1],order[2]),bodies[b].inverse_mass);}
 const float inverse=P::add(contribution[0],contribution[1]);return inverse>0?P::div(1.0f,inverse):0.0f;
}
}
